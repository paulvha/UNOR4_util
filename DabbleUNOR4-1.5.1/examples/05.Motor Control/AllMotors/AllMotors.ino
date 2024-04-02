/*
   MotorControl Module is used to control actuators like DC motors and Servos.
   
   NOTE1: There are 6 PWM channels on an UNOR4 : D3, D5, D6, D9, D10, D11, When using the
   ArduinoV3 motor shield D3 and D11 are used. 

   NOTE2: The other supported MotorShield is the AdafruitV2. As this board is using I2C
   communications instead of Arduino pins, this example is NOT supporting the Adafruit board.
   Look at I2c_servo_motors sketch.

   NOTE3: This example shows how to change the motor and servo frequency. The defaults
   are 100Hz motor and 50Hz servo.
   
   You can reduce the size of library compiled by enabling only those modules that you want to
   use. For this first define CUSTOM_SETTINGS followed by defining INCLUDE_modulename.

   Explore more on: https://thestempedia.com/docs/dabble/motor-control-module/ 

   updated 6 Mar 2024
   by paulvha
 
   tested on UnoR4 library 1.1.0
*/

/* When using the Arduino V3 shield, which is currently the only supported motor shield with pins on UNOR4, 
 * define USING_ARDUINO_SHIELD_V3 below.
 */
#define USING_ARDUINO_SHIELD_V3

/* define the frequencies for the motor, default frequency Motor:100., servo 50*/
#define MOTORFREQ  100
#define SERVOFREQ   50

#define CUSTOM_SETTINGS
#define INCLUDE_MOTORCONTROL_MODULE
#include <DabbleUNOR4.h>

#ifdef USING_ARDUINO_SHIELD_V3
  uint8_t pinServo1 =  D5;    // optional either D5, D6, D9, D10
  uint8_t pinServo2 =  D6;    // optional either D5, D6, D9, D10
  uint8_t pwmMotor1 =  D3;    // channel A PWM (work duty)
  uint8_t dir1Motor1 = D12;   // channel A Direction (HIGH forward, LOW backward)
  uint8_t dir2Motor1 = D9;    // Channel A Brake (HIGH released, LOW enabled)
  uint8_t pwmMotor2 =  D11;   // channel B PWM (work duty)
  uint8_t dir1Motor2 = D13;   // Channel B Direction (HIGH forward, LOW backward)
  uint8_t dir2Motor2 = D8;    // Channel B Brake (HIGH released, LOW enabled)
#else
  // ALL user defined pins
  uint8_t pinServo1 =  D5;    // either D3, D5, D6, D9, D10, D11
  uint8_t pinServo2 =  D6;    // either D3, D5, D6, D9, D10, D11
  uint8_t pwmMotor1 =  D3;    // either D3, D5, D6, D9, D10, D11
  uint8_t dir1Motor1 = D12;   // when HIGH: move forward
  uint8_t dir2Motor1 = D9;    // when HIGH: move backward  
  uint8_t pwmMotor2 =  D11;   // either D3, D5, D6, D9, D10, D11
  uint8_t dir1Motor2 = D13;   // when HIGH: move forward
  uint8_t dir2Motor2 = D8;    // when HIGH: move backward
#endif

void setup() {
  Serial.begin(115200);         // make sure your Serial Monitor is also set at this baud rate.
  while(!Serial);
  
  Dabble.begin("MyUNOR4");      // set bluetooth name of your device 
   
  Serial.println("All Motor control example");

#ifdef USING_ARDUINO_SHIELD_V3  // enable Arduino-V3 shield pin usage
  Controls.SetArduinoV3(true);
#endif

  // Only impacts before the first runMotorX or runServoX call
  Controls.SetMotorFrequency(MOTORFREQ, SERVOFREQ);
}

void loop() {
  Dabble.processInput();             //this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.

  if (!Controls.runServo1(pinServo1)) {
    Serial.println("Check your Servo1 pin");
    while(1);
  }

  if (!Controls.runServo2(pinServo2)) {
    Serial.println("Check your Servo2 pin");
    while(1);
  }

  if (! Controls.runMotor1(pwmMotor1,dir1Motor1,dir2Motor1)) {
    Serial.println("Check your Motor1 PWM pin");
    while(1);
  }   
       
  if (!Controls.runMotor2(pwmMotor2,dir1Motor2,dir2Motor2)) {
    Serial.println("Check your Motor2 PWM pin");
    while(1);
  }
}
