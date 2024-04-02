/*
  MotorControl Module is used to control actuators like DC motors and Servos.

  The MotorShield from Adafruit V2 is supported on an UNOR4. As this board is using I2C
  communications instead of Arduino pins, this example is supporting the Adafruit board.
   
  The other sketch (AllMotors.ino) supports motor shields that are directly controlled 
  with UNOR4 GPIO pins (like ArduinoV3)
  
  You can reduce the size of library compiled by enabling only those modules that you want to
  use. For this first define CUSTOM_SETTINGS followed by defining INCLUDE_modulename.

  Explore more on: https://thestempedia.com/docs/dabble/motor-control-module/ 

  Parts taken from the DC-MotorTest and MotorParty sketches from Adafruit:
  This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
  It won't work with v1.x motor shields! Only for the v2's with built in PWM control

  For use with the Adafruit Motor Shield v2 :  http://www.adafruit.com/products/1438

  There are 6 PWM channels on an UNOR4 : D3, D5, D6, D9, D10, D11 that can be used for servo.
  Depending on when you use this sketch, the solution to the servo problem documented in 
  https://forum.arduino.cc/t/trouble-with-servos-on-r4-wifi/1151749 might included in a release
  
  Connect a DC motor to M1
  Connect a DC motor to M2 (uncomment INCLUDE_MOTOR2)
  Connect a hobby servo to SERVO1 pin (uncomment INCLUDE_SERVO1)
  Connect a hobby servo to SERVO2 pin (uncomment INCLUDE_SERVO2)
  
  created 2 April 2024
  by paulvha
	Warning : could not test as I do not have an AdafruitV2 board
  
  using UnoR4 library 1.1.0
*/
//========================================================================
// uncomment to ONLY see the Dabble Motor controller info that is returned
//========================================================================
//#define OnlyShowDabbleControlInfo

#define CUSTOM_SETTINGS
#define INCLUDE_MOTORCONTROL_MODULE
#include <DabbleUNOR4.h>

#ifndef OnlyShowDabbleControlInfo

  #include <Adafruit_MotorShield.h>
  
  // include second motor by uncommenting line below
  //#define INCLUDE_MOTOR2
  
  // include Servo1 by uncommenting line below (define SERVO1_PIN below)
  //#define INCLUDE_SERVO1
  
  // include Servo2 by uncommenting line below (define SERVO2_PIN below)
  //#define INCLUDE_SERVO2
  
  // Create the motor shield object with the default I2C address
  Adafruit_MotorShield AFMS = Adafruit_MotorShield();
  // Or, create it with a different I2C address (say for stacking)
  // Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);
  
  // Select which 'port' M1, M2, M3 or M4. In this case, M1
  Adafruit_DCMotor *myMotor = AFMS.getMotor(1);
  
  #ifdef INCLUDE_MOTOR2
   // You can also make another motor on port M2
   Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);
  #endif
  
  #ifdef INCLUDE_SERV01
   #include <Servo.h>
   Servo servo1;         // Also test out the built in Arduino Servo library
   #define SERVO1_PIN D10
  #endif
  
  #ifdef INCLUDE_SERV02
   #include <Servo.h>
   Servo servo2;       // Also test out the built in Arduino Servo library
   #define SERVO2_PIN D11
  #endif

#endif // OnlyShowDabbleControlInfo

/*
 * INFO :
 * struct M_I2C_info {
 *   float pwmMotor1;        // if > 0: forward motor1, if < 0 move backward motor1. PWM/speed value +/- 0 - 255 
 *   float pwmMotor2;        // if > 0: forward motor2, if < 0 move backward motor2. PWM/speed value +/- 0 - 255
 *   uint8_t angleServo1;    // if > 0, forward servo to pos. value 0 - 180 degrees
 *   uint8_t angleServo2;    // if > 0, forward servo to pos. value 0 - 180 degrees
};
*/
struct M_I2C_info Control_Info;
uint16_t cnt = 0;               // not to overload monitor

void setup() {
  
  Serial.begin(115200);         // make sure your Serial Monitor is also set at this baud rate.
  while(!Serial);
   
  Serial.println("Adafruit Motorshield v2 - DC Motor test on UNOR4!");
  
  Dabble.begin("MyUNOR4");      // set bluetooth name of your device

  // wait for connect
  if (!Dabble.isAppConnected()) {
    if (cnt++ > 10000) {
      Serial.println("Waiting for connect");
      cnt = 0;
    }
    delay(100); 
  }
  
#ifndef OnlyShowDabbleControlInfo

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find then I2C Motor Shield. Check wiring.");
    while (1);
  }
  
  Serial.println("I2C Motor Shield found.");

  // Set the speed to start, from 0 (off) to 255 (max speed)
  myMotor->setSpeed(150);
  myMotor->run(FORWARD);
  
  // turn on motor
  myMotor->run(RELEASE);

 #ifdef INCLUDE_MOTOR2
  // Set the speed to start, from 0 (off) to 255 (max speed)
  myOtherMotor->setSpeed(150);
  myOtherMotor->run(FORWARD);
  
  // turn on motor
  myOtherMotor->run(RELEASE);
 #endif //INCLUDE_MOTOR2

 #ifdef INCLUDE_SERV01
  servo1.attach(SERVO1_PIN);
 #endif //INCLUDE_SERV01

 #ifdef INCLUDE_SERV02
  servo1.attach(SERVO2_PIN);
 #endif //INCLUDE_SERV02

#endif // OnlyShowDabbleControlInfo

  cnt = 0;
}

void loop() {

  Dabble.processInput();             // this function is used to refresh data obtained from smartphone. Hence calling this function is mandatory in order to get data properly from your mobile.

  // if a value changed since last time
  if (Controls.GetControllerInfo(&Control_Info)) {
    
    Serial.print("pwmMotor1: ");
    Serial.println(Control_Info.pwmMotor1);
    Serial.print("pwmMotor2: ");
    Serial.println(Control_Info.pwmMotor2);
    Serial.print("angleServo1: ");
    Serial.println(Control_Info.angleServo1);
    Serial.print("angleServo2: ");
    Serial.println(Control_Info.angleServo2);

    Handle_Adafruit_motors();

#ifdef INCLUDE_SERV01
    servo1.write(Control_Info.angleServo1);
#endif // INCLUDE_SERV01

#ifdef INCLUDE_SERV02
    servo1.write(Control_Info.angleServo2);
#endif // INCLUDE_SERV02

    cnt = 0;
  }
  else {
    if (cnt++ > 10000) {
      Serial.println("NO change in control status. No action needed");
      cnt = 0;
    }
  }
}

/**
 * Handle DC-motors on shield
 */
void Handle_Adafruit_motors()
{
  
#ifndef OnlyShowDabbleControlInfo

  if (Control_Info.pwmMotor1 > 0) {
    Serial.print("tick-M1");
    myMotor->run(FORWARD);
    myMotor->setSpeed(Control_Info.pwmMotor1);
  }

  else if (Control_Info.pwmMotor1 < 0) {
    Serial.print("tock-M1");
    myMotor->run(BACKWARD);
    myMotor->setSpeed(-Control_Info.pwmMotor1);  // turning negative to positive
  }

  else
    myMotor->setSpeed(0);
  
  Serial.print("tech-M1");
  myMotor->run(RELEASE);

 #ifdef INCLUDE_MOTOR2

  if (Control_Info.pwmMotor2 > 0) {
    Serial.print("tick-M2");
    myOtherMotor->run(FORWARD);
    myOtherMotor->setSpeed(Control_Info.pwmMotor2);
  }

  else if (Control_Info.pwmMotor2 < 0) {
    Serial.print("tock-M2");
    myOtherMotor->run(BACKWARD);
    myOtherMotor->setSpeed(-Control_Info.pwmMotor2);  // turning negative to positive
  }
 
  else
    myOtherMotor->setSpeed(0);

  Serial.print("tech-M2");
  myOtherMotor->run(RELEASE);
 #endif  //INCLUDE_MOTOR2

#endif //OnlyShowDabbleControlInfo
}
