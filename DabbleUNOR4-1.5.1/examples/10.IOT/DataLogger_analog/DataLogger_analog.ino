/*
   Data Logger module helps you in storing data in form of .csv file. 
   Later you can open this file to view your stored data.
   
   You can reduce the size of library compiled by enabling only those modules that you want to
   use.For this first define CUSTOM_SETTINGS followed by defining INCLUDE_modulename.
   
   Explore more on: https://thestempedia.com/docs/dabble/
   
   As the Phone Sensor module on my Samsung Smartphone crashes all the time, I have created this analog example.
   It will read ANALOGPIN (defined below). 
   
   Connect a wire to closeFileSignalPin. Once started you will see update of the values.
   When connect to GND the file will close and measurement halted.

   On my Samsung smartphone the Analog.csv was created, but could not open from the interface. I could share it to
   my email and then open it on the PC however. 

   updated 10 Mar 2024
   by paulvha
   
   tested on UnoR4 library 1.1.0
*/

#define CUSTOM_SETTINGS
#define INCLUDE_DATALOGGER_MODULE
#include <DabbleUNOR4.h>

bool isFileOpen = true;
uint8_t closeFileSignalPin = 2;   //this pin is internally pulled up and a push button grounded on one side is connected to pin so that pin detects low logic when push button is pressed.
#define ANALOGPIN A0

// called once connected
void initializeFile(){
  Serial.println("Initialize");
  DataLogger.createFile("Analog");
  DataLogger.createColumn("Value");
}

void setup() { 
  
  pinMode(closeFileSignalPin,INPUT_PULLUP);
  pinMode(ANALOGPIN,INPUT);
  
  Serial.begin(115200);       // make sure your Serial Monitor is also set at this baud rate.
  while(!Serial);
  Serial.println("Datalogger Analog");
  
  Dabble.begin("MyUNOR4");    // set bluetooth name of your device
  
  // set call back to initialize after connect
  DataLogger.sendSettings(&initializeFile);

  Serial.println("This example will log Analogvalues in a CSV file on your smartphone");
}

void loop() {
  Dabble.processInput();             //this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.
  
  if (isFileOpen == true)
  {
    float res = print_Analog_data();
    DataLogger.send("Value",res);
  }
  
  if((digitalRead(closeFileSignalPin) == LOW) && isFileOpen == true)
  {
    isFileOpen = false;
    Serial.println("File is closed.");
    DataLogger.stop();
  }

  delay(2000);
}

float print_Analog_data()
{
  Serial.print("Analog: ");
  float res = analogRead(ANALOGPIN);
  Serial.println(res);
  Serial.println();
  return res;
}
