/*
 * This is an update of the original MotorControls which will allow 
 * control on DC and Servo motor on an UNOR4 WiFi.
 * 
 * An UNOR4 has 6 PWM pins and each pin is assigned to a channel. 
 * In each channel the PWM information for the pin is stored.
 * 
 * All channels are then checked and pin is updated as necessary.
 * 
 * In case the BLE disconnects the Motors and servo's will be stopped.
 * 
 * ledcAttachPin,ledcSetup,ledcWrite are actually ESP32 calls and 
 * reworked for the UNOR4 WiFi.
 * 
 * updated 6 Mar 2024
 * by paulvha
 * 
 * April 2024 : added support to enable AdafruitV2 I2C type board
 * by paulvha
 * 
 * tested on UnoR4 library 1.1.0
 */
#ifndef motorControls_h
#define motorControls_h

#include "ModuleParent.h"
#include "pwm.h"

#define MAX_PWM_CHANNELS  6

// info about each channel
struct M_channel_config {
  uint8_t pin;
  float   freq;
  float   duty_perc;
  bool		initialized;
  bool    updateDuty;
  PwmOut  *pwm;
};

struct M_I2C_info{
	float pwmMotor1;				// if > 0: forward motor1, if < 0 move backward. PWM value 0 - 255 
	float pwmMotor2;				// if > 0: forward motor2, if < 0 move backward. PWM value 0 - 255
	uint8_t angleServo1;		// if > 0, forward servo to pos. value 0 - 180
	uint8_t angleServo2;		// if > 0, forward servo to pos. value 0 - 180
};

void OnDisconnect();

class motorControls : public ModuleParent
{

public:
	motorControls();
	//Pictobox and Arduino
	bool runMotor1(uint8_t pwm,uint8_t direction1,uint8_t direction2);
	bool runMotor2(uint8_t pwm,uint8_t direction1,uint8_t direction2);
	bool runServo1(uint8_t pin);
	bool runServo2(uint8_t pin);

	// enable / disable ArduinoV3 motorshield
	// will set the correct pins
	void SetArduinoV3(bool s);

	// enable AdafruitV2 support, by calling the controller latest info
	// return true if controls have changed since last call
	bool GetControllerInfo(M_I2C_info *info);

	// set different frequency than default
	void SetMotorFrequency(float mf, float sf);

public:
	uint8_t angleServo1=0;
	uint8_t angleServo2=0;
	uint8_t PrevangleServo1 = 0;
	uint8_t PrevangleServo2 = 0;
	float pwmMotor1=0;
	float pwmMotor2=0;
	float PrevPwmMotor1 = 0;
	float PrevPwmMotor2 = 0;

private:
	bool UseForStatusOnly;
	void processData();
	uint8_t valid_PWM_pins[6] = {D3,D5,D6,D9,D10,D11};				// valid PWM pins on UNOR4
	bool IsArduinoV3 = false;
	float Motorfreq = 100;
	float Servofreq = 50;
	byte functionId;
	byte byte1;
	byte byte2;
	int channel_A;
	int channel_B;
	int channel_C;
	int channel_D;

	uint8_t prevMotor1pin = 0;
	uint8_t prevMotor2pin = 0;
	uint8_t prevServo1pin = 0;
	uint8_t prevServo2pin = 0;

	static int minPulseWidth;
	static int maxPulseWidth;
	static int minAngle;
	static int maxAngle;

	int angleTomicroseconds(int degree);
	int microsecondsToDuty(int pulse);
	void writeServoAngle(int angle,uint8_t channel);
	void ledcAttachPin(uint8_t pin, uint8_t ch);
	void ledcSetup(uint8_t ch, float value, uint8_t resolution);
	void ledcWrite(uint8_t ch, float value);
	M_channel_config channels[MAX_PWM_CHANNELS] = {0};
	M_I2C_info prev_I2C_info = {0};
	void InitPWMChannel(int ch);
	void UpdateMotors();
};

extern motorControls Controls;

#endif 
