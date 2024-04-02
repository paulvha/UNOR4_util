/*
   Pin State Monitor is made for monitoring status of analog and digital pins of your board.
   In this example bluetooth is to be connected to an UNOR4 Wifi Board.

   NOTE: State for only  following pins can be seen on Pin State Monitor:
   | Screen Name  | GPIO Pins                          |
   | Digital      | D0,D1,D2,D3,D4,D5,D6,D7,D8,D9,     |
   |              | D10,D11,D12,D13                    |
   | Analog       | A0, A1, A2, A3, A4, A5             |

   The digital status will been shown on position 30 (D0) - 43 (D13).

   The analog status will be shown as 
   Pin 31 A0
   pin 30 A1
   Pin 36 A2
   Pin 37 A3
   Pin 38 A4
   Pin 39 A5

   In order to see the update you MUST first make a switch between Analog and Digital screen. 
   Only then the app will send information about which screen is in front.
   
   You can reduce the size of library compiled by enabling only those modules that you want
   to use. For this first define CUSTOM_SETTINGS followed by defining INCLUDE_modulename.
  
   Explore more on: https://thestempedia.com/docs/dabble/pin-state-monitor-module/
   
   updated 3 Mar 2024
   by paulvha
   
   tested on UnoR4 library 1.1.0

*/

#define CUSTOM_SETTINGS
#define INCLUDE_PINMONITOR_MODULE
#include <DabbleUNOR4.h>


void setup() {
  /*
     NOTE: PinMonitor only displays status of the pins of your board. It does not configure pinMode of the pins.
     So if there is any necessity to define pinMode then declare it setup as per requirement.
  */
  Serial.begin(115200);    // make sure your Serial Monitor is also set at this baud rate.
  while(!Serial);

  Serial.println("PinStateMonitor");
  Dabble.begin("MyUNOR4"); // set bluetooth name of your device
}

void loop() {
    // don't hammer the update, but keep checking BLE
  for (int i =0 ; i < 10 ;i++){
    Dabble.processInput(); //this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.
    delay(100);
  }
           
  if (Dabble.isAppConnected()) {
    PinMonitor.sendAnalogData();
    PinMonitor.sendDigitalData();

  }
}
