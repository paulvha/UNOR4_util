/*
  SMS module uses your smartphone to send SMS based on events occuring in your hardware board.
  
  You can reduce the size of library compiled by enabling only those modules that you want to
  use. For this first define CUSTOM_SETTINGS followed by defining INCLUDE_modulename.

  Explore more on: https://thestempedia.com/docs/dabble/

  Tested on Samsung phone but the Notify app does not react and there is NO SMS APP. 
  Do NOT know what to use on the smartphone.

  updated 10 Mar 2024
  by paulvha
   
  tested on UnoR4 library 1.1.0
*/

#define CUSTOM_SETTINGS
#define INCLUDE_SMS_MODULE

#include <DabbleUNOR4.h>
uint8_t pushButtonPin = 2;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.begin("SMS module");
  
  Dabble.begin("MyUNOR4");                     //set bluetooth name of your device                  
  pinMode(pushButtonPin, INPUT_PULLUP);        //Since pin is internally pulled up hence connect one side of push button to ground so whenever button is pushed pin reads LOW logic.
 }

void loop() {
  Dabble.processInput();    //this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.
  
  if(digitalRead(pushButtonPin) == LOW)
  {
    SMS.sendMessage("0123456789","Buenas Noches");  //Contact Number, message content
    delay(1000);   //debounce delay
  }
  
}
