/*
    This block gives light intensity sensed by smartphone.

    You can reduce the size of library compiled by enabling only those modules that you want to
    use. For this first define CUSTOM_SETTINGS followed by defining INCLUDE_modulename.

    Explore more on: https://thestempedia.com/docs/dabble/phone-sensors-module/
*/
#define CUSTOM_SETTINGS
#define INCLUDE_SENSOR_MODULE
#include <DabbleUNOR4.h>


void setup() {
  Serial.begin(115200);     // make sure your Serial Monitor is also set at this baud rate.
  Dabble.begin("MyUNOR4");  // set bluetooth name of your device
}

void loop() {
  Dabble.processInput();             //this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.
  print_Light_data();
}

void print_Light_data()
{
  Serial.print("LIGHT: ");
  Serial.println(Sensor.getLightIntensity(), 7);
  Serial.println();
}
