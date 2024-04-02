/*
   This is Led brightness control example for UnoR4 WiFi that uses LED Brightness Control Module in Dabble app.
   This module allows you to control state of pin. You can make pin HIGH or LOW or can also assign any PWM
   value to it.

   The pins can be set by the remote Dabble app or with the keyboard in this sketch. The status can also
   be monitored from this sketch.

   NOTE: As in UNOR4 only 6 PWM pins (3, 5, 6, 9, 10, 11) can be configured as a PWM channel. 
   Another eight pins can be controlled by Module as normal digital pins.

   You can reduce the size of library compiled by enabling only those modules that you want to
   use.For this first define CUSTOM_SETTINGS followed by defining INCLUDE_modulename.

   Explore more on: https://thestempedia.com/docs/dabble/getting-started-with-dabble/

  Updated 28 Feb 2024 for UNOR4
  by paulvha
  Tested on UnoR4 library 1.1.0
*/
#define CUSTOM_SETTINGS
#define INCLUDE_LEDCONTROL_MODULE
#include <DabbleUNOR4.h>

// wait max 20 seconds for serial input
#define INPUTDELAY 20

void setup() {
  Serial.begin(115200);     // make sure your Serial Monitor is also set at this baud rate.
  while(!Serial);
  Serial.println("Example: Led Brightness Control");
  Dabble.begin("MyUNOR4");  //set bluetooth name of your device

  Serial.setTimeout(INPUTDELAY * 1000);
}

void loop() {

  // if any keyboard input: start interaction
  if (Serial.available()) handleRequest();
  
  Dabble.processInput();    //this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.
}

/**
 * enter was pressed user wants interactive action
 */
void handleRequest(){
  int val, ch;
  
  while(1) {
    Dabble.processInput();
    Serial.println("1. Update a pin");
    Serial.println("2. Display pin status");
    Serial.println("3. Display all pin status");
    Serial.println("4. exit");

    val = ReadSerial();

    if (val == INVALID || val == 4) {
      Serial.println("exit");
      return;
    }

    if (val == 1) {
      
      // ask for valid pin and obtain channel
      ch = GetValidPin();

      // if no input or no channel
      if (ch == INVALID) return;

      // update duty cycle or frequency
      AdjustSettings(ch); 
    }
    
    else if (val == 2){
      // ask for valid pin and obtain channel
      ch = GetValidPin();

      // if no input or not channel
      if (ch == INVALID) return;

      // display
      displayChannelStatus(ch);
    }
    else if (val == 3){
      
      for (int i = 0; i < MAX_TOTAL_CHANNELS; i++)
      {
        if ( LedControl.getpinNumber(i) != 0)
          displayChannelStatus(i);
      }
    }
    else {
      Serial.print ("Invalid selection: ");
      Serial.println(val);
    }
  }
}

/** 
 *  display channel status
 */
void displayChannelStatus(int ch)
{
  int ch_type = LedControl.ChannelType(ch);

  if (ch_type == 1)
  {
    Serial.print("PWM pin: ");
    Serial.print(LedControl.getpinNumber(ch));

    Serial.print(" Frequency: ");
    Serial.print(LedControl.getFrequency(ch));
    Serial.print(" Brightness: ");
    Serial.println(LedControl.readBrightness(ch));
  }
  else if (ch_type == 2) 
  {
    Serial.print("Digital pin: ");
    Serial.print(LedControl.getpinNumber(ch));

    Serial.print(" Current State: ");
    if (LedControl.getpinState(ch)) Serial.print("On");
    else Serial.print("Off");
    Serial.print(" Brightness: ");
    Serial.println(LedControl.readBrightness(ch));
  }
  else
  {
    Serial.print("Invalid channel: ");
    Serial.println(ch);
  }
}

/**
 * set (potential) new brightness, frequency
 */
void AdjustSettings(int ch){
  
  int val;
  bool DoUpdate = false;
  
  Serial.print("Adjusting pin: ");
  Serial.println(LedControl.getpinNumber(ch));

  while(1) {
    Dabble.processInput();   
    Serial.print("\nCurrent brightness: ");
    Serial.print(LedControl.readBrightness(ch));
    Serial.println(". Enter new brightness between 1 and 255. (0 = stop)");
    val = ReadSerial();

    if (val == INVALID) {
      Serial.println("No change to brightness");
      break;
    }
    
    if (val == 0) {
      LedControl.ChannelWrite(ch, 0);
      Serial.println("Stopped");  
      return;  
    }
    
    if (val < 1 || val > 255) {
      Serial.print("Incorrect brightness: ");
      Serial.println(val);
    }
    else {
      LedControl.ChannelWrite(ch, val);
      DoUpdate = true;
      
      Serial.print("Setting brightness: ");
      Serial.print(val);
      Serial.print(" or duty percentage ");
      Serial.print((float) (val*100)/255);
      Serial.println("%");
      break;
    }
  }

  while(1) {
    Dabble.processInput();   
    Serial.print("\nCurrent frequency: ");
    Serial.print(LedControl.getFrequency(ch));
    Serial.println("Hz. Enter new frequency between 1 and 10Khz");    // 10 Khz is an artifical limit
    val = ReadSerial();

    if (val == INVALID) {
      Serial.println("No change to frequency");
      break;
    }
    
    if (val < 1 || val > 10000) {
      Serial.print("Incorrect Frequency: ");
      Serial.println(val);
    }
    else {
      DoUpdate = true;
      LedControl.ChannelSetup(ch,val);
      Serial.print("Setting Frequency: ");
      Serial.println(val);
      break;
    }
  }

  if (DoUpdate) {
    Serial.print("Updating pin ");
    Serial.println(LedControl.getpinNumber(ch));
  }
}
/** 
 *  read a value from Serial.
 */
int ReadSerial(){
  
  // flush pending input
  while(Serial.available()) Serial.read();
  
  Serial.print("Only <enter> is return. (or wait ");
  Serial.print(INPUTDELAY);
  Serial.println(" seconds)");
  
  String Keyb = Serial.readStringUntil(0x0a);

  // flush any pending input
  while(Serial.available()) Serial.read();
  
  // only enter or timeout
  if (Keyb.length() < 2) return INVALID;
  
  return(Keyb.toInt());
}

/**
 * read pin to set from Serial / keyboard and check it is valid
 * 
 * return :
 *  INVALID only enter or timeout
 *  else channel nummer 
 */
int GetValidPin()
{
  while (1) {
    
    Dabble.processInput();
    Serial.print("Provide Pin < ");
    Serial.println(PINS_COUNT);
    int pin = ReadSerial();
    if (pin == INVALID) return pin;
    
    if (pin > PINS_COUNT){
      Serial.print("Invalid pin: ");
      Serial.println(pin);
    }
    else
      return LedControl.GetChannel(pin);;
  }
}
