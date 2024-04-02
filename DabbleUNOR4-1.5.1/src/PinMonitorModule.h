/* updated 3 Mar 2024
 * by paulvha
 * 
 * tested on UnoR4 library 1.1.0
 */

#ifndef PinMonitorModule_h
#define PinMonitorModule_h

#include "ModuleParent.h"
#include "Arduino.h"

class PinMonitorModule : public ModuleParent
{ 

public:
  byte function_id_d = 0x05;	// digital data
  byte function_id_a = 0x06;	// analog data
  uint8_t dd_size = 8;
  uint8_t an_size = 12;
  //start showing AS pin 30 (D0) - 43 (D13)
  uint8_t digital_pins[8][8]={{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,D0,D1,D2,D3,D4,D5},{D6,D7,D8,D9,D10,D11,D12,D13},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};
  uint8_t analog_pin[6]={PIN_A0,PIN_A1,PIN_A2,PIN_A3,PIN_A4,PIN_A5};

  PinMonitorModule();

  void sendDigitalData();
  void sendAnalogData();

private:
  void processData();
  byte* digitaldata=NULL;
  byte* analog_data=NULL;
};

extern PinMonitorModule PinMonitor;
#endif //PinMonitorModule_h
