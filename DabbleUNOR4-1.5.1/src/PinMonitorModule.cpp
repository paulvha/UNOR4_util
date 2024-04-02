/* updated 3 Mar 2024
 * by paulvha
 * 
 * tested on UnoR4 library 1.1.0
 */
 
#define FROM_DABBLE_LIBRARY
#include "DabbleUNOR4.h"
#include "PinMonitorModule.h"

// uncomment to enable debug messages
//#define DEBUG_PIN

#ifndef DEBUG_PIN
	#ifdef DEBUG_ALL
		#define DEBUG_PIN
	#endif
#endif

PinMonitorModule::PinMonitorModule() : ModuleParent(PINMONITOR_ID)
{
  digitaldata = new uint8_t[dd_size];
  analog_data = new uint8_t[an_size];
  
  for(int i=0; i < (an_size / 2) ;i++)
  {
    pinMode(analog_pin[i], INPUT);
  }
}

// nothing is send for the pinMonitor Module to process
// only dabble screen and function is send
void PinMonitorModule::processData()
{
  //Serial.println(getDabbleInstance().getFunctionId());
}

// update digital data information
void PinMonitorModule::sendDigitalData()
{
  // set each byte to zero
  for(int i=0;i < dd_size;i++)
  {
    digitaldata[i]=0;
  }

  int start;
  // to show starting on pin 30, only (part of) byte 3 and 4 are used
  for(int j=3; j < 5 ; j++)
  {
    if (j == 3) start = 2;
    else start = 0;

    for(int i=start;i < 8;i++)
    {
      // set as input
      pinMode(digital_pins[j][i],INPUT);
      
      // read the digital port and set bit 
      digitaldata[j] = digitaldata[j] + ((digitalRead(digital_pins[j][i]))<<i);
    }
  }
  
#ifdef DEBUG_PIN 
  for(int j=0;j < dd_size;j++)
  {
    Serial.print(digitaldata[j],BIN);
    Serial.print("    ");
  }
  Serial.println();
#endif

  // ScreenId 1 is digital screen is selected
  if(getDabbleInstance().getScreenId()== 1 && getDabbleInstance().readModuleId() == PINMONITOR_ID)
  {
    Dabble.sendModuleFrame(PINMONITOR_ID,0,function_id_d,1,new FunctionArg(dd_size,digitaldata));
  }
}

// sent analog data
void PinMonitorModule::sendAnalogData()
{
  uint8_t counter=0;

  // init
  for(int i=0; i < an_size ;i++)
  {
    analog_data[i]=0;
  }
  
  for (int i = 0; i < an_size; i=i+2)
  {
    // set pin number
    analog_data[i] = (analog_pin[counter]-10) << 4;
    
    // add analog value MSB
    analog_data[i] = analog_data[i] | ((analogRead(analog_pin[counter]) & 0x0F00) / 256); 
    
     // add analog value LSB
    analog_data[i+1]= (analogRead(analog_pin[counter]) & 0x00ff);

    // next analog pin offset
    counter++;  
  }

#ifdef DEBUG_PIN
  for(int j=0;j < an_size;j++)
  {
    Serial.print(analog_data[j],BIN);
    Serial.print("    ");
  }
  Serial.println();
#endif*/

  // ScreenId 1 is digital screen is selected
  if(getDabbleInstance().getScreenId()==3 && getDabbleInstance().readModuleId()==PINMONITOR_ID) 
  {
    Dabble.sendModuleFrame(PINMONITOR_ID,0,function_id_a,1,new FunctionArg(an_size,analog_data));
  }
}
