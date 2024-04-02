/*
  This code demonstrates how to use music module. Here a push button is used for playing and stopping music in smartphone.
  A push button is connected on an internally pulled up digital pin. And a change in state of pin is read.
  With every press on push button state of music module will be toggled. If no music is played by music module then
  command to play music module will be sent and if currently music is being played then it will be stopped.

  You can reduce the size of library compiled by enabling only those modules that you want to
  use. For this first define CUSTOM_SETTINGS followed by defining INCLUDE_modulename.

  Explore more on: https://thestempedia.com/docs/dabble/

  Make sure to connect to BLE in the main menu of the app and then select Music. Making a BLE selection on the music screen
  does not work.
  
  updated 10 Mar 2024
  by paulvha
   
  tested on UnoR4 library 1.1.0
*/

#define CUSTOM_SETTINGS
#define INCLUDE_MUSIC_MODULE
#include <DabbleUNOR4.h>

uint8_t musicSignalPin = 2;
bool currentStatusOfPin = 0, prevStatusOfPin = 0; //status of musicSignalPin
bool musicIsOn = false;
bool StopSendAlready = false;     // send stop only once 
bool playMusic = false;

void setup() {
  pinMode(musicSignalPin, INPUT_PULLUP);
  
  Serial.begin(115200);
  while(!Serial);
  Serial.begin ("Music example");
  
  Dabble.begin("MyUNOR4");        //set bluetooth name of your device
}

void loop() {
  Dabble.processInput(); //this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.
  
  currentStatusOfPin = digitalRead(musicSignalPin);
  
  if (currentStatusOfPin == 0 &&  prevStatusOfPin == 1) //detecting change in state of pin due to push button
  {
    delay(500); // debouncedelay
    playMusic = !(playMusic);
  }
  
  if (playMusic == true)
  {
    StopSendAlready = false;
    
    if (musicIsOn == false)
    {
      Music.play("A4");         //assigned key for Piano Note A4
      Music.addToQueue("B4");   //assigned key for Piano Note B4
      Music.addToQueue("C4");   //assigned key for Piano Note C4
      Music.addToQueue("D4");   //assigned key for Piano Note D4
      Music.addToQueue("E4");   //assigned key for Piano Note E4
      Music.addToQueue("F4");   //assigned key for Piano Note F4
      Music.addToQueue("G4");   //assigned key for Piano Note G4
      Music.addToQueue("C5");   //assigned key for Piano Note C5   //Select your own music files, assign them key and write this key as written here for various piano notes.
      Music.addToQueue("S1");   //assigned key for Piano scream
      musicIsOn = true;
    }
  }
  
  if (playMusic == false)
  {
    if (! StopSendAlready) Music.stop();
    musicIsOn = false;
    StopSendAlready = true;
  }
  
  prevStatusOfPin = currentStatusOfPin;
}
