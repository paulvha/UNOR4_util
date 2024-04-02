/*
   Camera Module allows you to click images and take videos from smartphone by sending commands from your hardware.
   This function demonstrates functions available in library for camera module. 

   Note1: on my Samsung smartphone it was asked for different permissions. Although I agreed, it complained that it 
   failed to get access to storage. However after closing the app and restarting the access was granted. 
   Else you can grant access in settings / apps / permissions
   
   Open Serial monitor and follow the instructions printed there to take images and videos in different cases.

   You can reduce the size of library compiled by enabling only those modules that you want
   to use. For this first define CUSTOM_SETTINGS followed by defining INCLUDE_modulename.

   Explore more on: https://thestempedia.com/docs/dabble/camera-module-photos-videos/

   updated 6 Mar 2024
   by paulvha
  
   tested on UnoR4 library 1.1.0
*/

#define CUSTOM_SETTINGS
#define INCLUDE_CAMERA_MODULE
#include <DabbleUNOR4.h>

void setup() {
  Serial.begin(115200);       // make sure your Serial Monitor is also set at this baud rate.
  while(!Serial);
  Serial.println("Camera example");
  Dabble.begin("MyUNOR4");     //set bluetooth name of your device
  printMessage();
}

void loop() {
  
  //upload code and open serial monitor (reset your board once if you cannot see any message printed on Serial Monitor)
  Dabble.processInput();  //this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.
  
  char a = processSerialdata();
  
  if (a == '1')
  {
    Camera.setParameters(FRONT,OFF,HIGH_QUALITY,0);   // Camera Direction, Flash, quality, Zoom(from 0 to 100%)
    Camera.captureImage();
  }
  else if (a == '2')
  {
    Camera.flipTo(REAR);
    Camera.flashMode(OFF);
    Camera.setQuality(LOW_QUALITY);
    Camera.captureImage();
  }
  else if (a == '3')
  {
    Camera.flipTo(REAR);
    Camera.setQuality(HIGH_QUALITY);
    Camera.zoom(50);
    Camera.captureImage();
  }
  else if (a == '4')
  {
    Camera.setParameters(REAR,OFF,HIGH_QUALITY,0);   // Camera Direction, Flash, quality, Zoom(from 0 to 100%)
    Camera.startRecording();
    delay(5000);
    Camera.stopRecording();
  }
  else if (a == '5')
  {
    Camera.setParameters(FRONT,OFF,HIGH_QUALITY,0);   // Camera Direction, Flash, quality, Zoom(from 0 to 100%)
    Camera.startRecording();
    delay(5000);
    Camera.stopRecording();
  }
}

void printMessage()
{
  Serial.println("Enter any number between 1 to 5 for executing task corresponding to that number: ");
  Serial.println("Tasks executed on sending different numbers are as followed: ");
  Serial.println("1 - Take a high quality image from FRONT camera with no flash and no zoom.");
  Serial.println("2 - Take a low quality image from REAR camera with Off flash and no zoom");
  Serial.println("3 - Take a 50% zoomed image from REAR camera with high quality"); 
  Serial.println("4 - Take 5 seconds video, high quality from REAR camera with no flash and no zoom.");
  Serial.println("5 - Take 5 seconds video, high quality from FRONT camera with no flash and no zoom.");
}

char processSerialdata()
{
  if(Serial.available())
  {
   printMessage();
   return Serial.read(); 
  }
  return '0';
}
