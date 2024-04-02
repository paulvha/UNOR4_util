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
 * updated 6 March 2024
 * by paulvha
 * 
 * tested on UnoR4 library 1.1.0
 * 
 * April 2024 : added support to enable AdafruitV2 I2C board
 * by paulvha
 * Warning : could not test as I do not have an AdafruitV2 board
 * 
 */

#define FROM_DABBLE_LIBRARY
#include "DabbleUNOR4.h"
#include "motorControls.h"

// comment out for debug info
//#define DEBUG_MOTOR

#ifndef DEBUG_MOTOR
	#ifdef DEBUG_ALL
		#define DEBUG_MOTOR
	#endif
#endif

int motorControls::minPulseWidth = 544;
int motorControls::maxPulseWidth = 2400;
int motorControls::minAngle = 0;
int motorControls::maxAngle = 180;

// default constructors.. only pin will be indicated.. nothing else
PwmOut M_pwm3(3);
PwmOut M_pwm5(5);
PwmOut M_pwm6(6);
PwmOut M_pwm9(9);
PwmOut M_pwm10(10);
PwmOut M_pwm11(11);

bool Motor_Disconnected = true;

// called when BLE disconnects after have been connected
// stop the engines !!
void OnDisconnect() {
	Motor_Disconnected = true;
}

motorControls::motorControls() : ModuleParent(CONTROLS_ID)
{
	channel_A = 0xff;
	channel_B = 0xff;

	for (int i = 0 ; i < MAX_PWM_CHANNELS ; i++) {
     InitPWMChannel(i);
	}

	// set callback when disconnected
	getDabbleInstance().SetDisconnectCallBack(OnDisconnect);

	// provide only StatusInfo
	UseForStatusOnly = false;
}

// enable / disable arduino V3 Motor pin control
void motorControls::SetArduinoV3(bool s) {
	IsArduinoV3 = s;
}

// setup different freqency than default
void motorControls::SetMotorFrequency(float mf, float sf) {
	Motorfreq = mf;
	Servofreq = sf;
}

// assign a pin to a channel
void motorControls::ledcAttachPin(uint8_t pin, uint8_t ch) {
	channels[ch].pin = pin;
}

// update frequency
void motorControls::ledcSetup(uint8_t ch, float value, uint8_t resolution /*ignored*/) {
	channels[ch].freq = value;
}

// update duty-cycle
void motorControls::ledcWrite(uint8_t ch, float value) {
	channels[ch].duty_perc = value;
	channels[ch].updateDuty = true;
}

// init a PWM channel
void motorControls::InitPWMChannel(int ch) {

	if (ch > MAX_PWM_CHANNELS) return;

  channels[ch].pin = 0x0;
  channels[ch].duty_perc = 0;
  channels[ch].initialized = false;
  channels[ch].updateDuty = false;

  switch (ch) {
    case 0: channels[ch].pwm = &M_pwm3;
      break;
    case 1: channels[ch].pwm = &M_pwm5;
      break;
    case 2: channels[ch].pwm = &M_pwm6;
      break;
    case 3: channels[ch].pwm = &M_pwm9;
      break;
    case 4: channels[ch].pwm = &M_pwm10;
      break;
    case 5: channels[ch].pwm = &M_pwm11;
      break;
  }
}

// process incoming data
void motorControls::processData()
{
#ifdef DEBUG_MOTOR
	Serial.println("Controls:processData");
#endif

	Motor_Disconnected=false;

	functionId = getDabbleInstance().getFunctionId();

	if(functionId == 0x01 || functionId == 0x02)
	{

		byte1=getDabbleInstance().getArgumentData(0)[0]; 
		byte2=getDabbleInstance().getArgumentData(0)[1];

#ifdef DEBUG_MOTOR
		Serial.print("FunctionID ");
		Serial.print(functionId);
		Serial.print(" byte1: ");
		Serial.print(byte1);
		Serial.print(" byte2: ");
		Serial.println(byte2);
#endif

		if(functionId == 0x01)
		{
			if(byte1 == 0xf0)
			{
				pwmMotor1 = byte2;
			}
			else if(byte1 == 0xff)
			{
				pwmMotor1 = -byte2;
			}
			else if(byte1 == 0x0f)
			{
				pwmMotor1 = 0;
			}
			else if(byte1 == 0x00)
			{
				pwmMotor1 = 0;
			}
		}

		if(functionId == 0x02)
		{
			if(byte1 == 0xf0)
			{
				pwmMotor2 = byte2;
			}
			else if(byte1 == 0xff)
			{
				pwmMotor2 = -byte2;
			}
			else if(byte1 == 0x0f)
			{
				pwmMotor2 = 0;
			}
			else if(byte1 == 0x00)
			{
				pwmMotor2 = 0;
			}
		}
	}
	else if(functionId == 0x03 || functionId == 0x04)
	{
		byte1 = getDabbleInstance().getArgumentData(0)[0];

#ifdef DEBUG_MOTOR
		Serial.print("FunctionID ");
		Serial.print(functionId);
		Serial.print(" byte1:");
		Serial.println(byte1);
#endif

		if(functionId == 0x03)
		{
			angleServo1=byte1;
		}
		else if(functionId == 0x04)
		{
			angleServo2=byte1;
		}
	}
}

bool motorControls::GetControllerInfo( M_I2C_info *info)
{
	bool ret = false; 	// default no change in status from controls

	// if not connected (anymore)
	if (Motor_Disconnected) {
		info->pwmMotor1 = 0;	// stop
		info->pwmMotor2 = 0;	// stop
		// servo NO change
	}
	else
	{
		info->pwmMotor1 = pwmMotor1;
		info->pwmMotor2 = pwmMotor2;
		info->angleServo1 = angleServo1;
		info->angleServo2 = angleServo2;
	}

	// check if different than previous
	if (info->pwmMotor1 != prev_I2C_info.pwmMotor1) ret = true;
	if (info->pwmMotor2 != prev_I2C_info.pwmMotor2) ret = true;
	if (info->angleServo1 != prev_I2C_info.angleServo1) ret = true;
	if (info->angleServo2 != prev_I2C_info.angleServo2) ret = true;

	// copy to previous
	prev_I2C_info.pwmMotor1 = info->pwmMotor1;
	prev_I2C_info.pwmMotor2 = info->pwmMotor2;
	prev_I2C_info.angleServo1 = info->angleServo1;
	prev_I2C_info.angleServo2 = info->angleServo2;

	// return
	return ret;
}

bool motorControls::runMotor1(uint8_t pwm,uint8_t direction1,uint8_t direction2)
{
	float T_pwmotor;

	// setup only first time
	if(prevMotor1pin != pwm)
	{
		pinMode(direction1,OUTPUT);
		pinMode(direction2,OUTPUT);

		for (int i = 0 ; i < 6; i++) {
			if (pwm == valid_PWM_pins[i])
				channel_A = i;
		}
		if (channel_A == 0xff) return false;

		ledcAttachPin(pwm,channel_A);
		ledcSetup(channel_A,Motorfreq,8);

		prevMotor1pin = pwm;
	}

	// STOP if no update received after have been disconnected 
	if (Motor_Disconnected) {
		digitalWrite(direction1,LOW);
		digitalWrite(direction2,LOW);					// V3 set brake
		T_pwmotor = 0;

#ifdef DEBUG_MOTOR
		Serial.println("Motor1: Stop, NO update after disconnect");
#endif
	}
	// Only act if the PWM value changed
	else
	{
		if(pwmMotor1 > 0)
		{
			digitalWrite(direction1,HIGH); 				// V3 move forward / Clockwise
			if(! IsArduinoV3) digitalWrite(direction2,LOW);
			else digitalWrite(direction2,HIGH);		// release brake on V3
			T_pwmotor = pwmMotor1;

	#ifdef DEBUG_MOTOR
			Serial.println("Motor1: Clockwise");
	#endif
		}
		else if (pwmMotor1 < 0)
		{
			digitalWrite(direction1,LOW);					// V3 move backward /AntiClockwise
			digitalWrite(direction2,HIGH);				// release brake on V3
			T_pwmotor = -pwmMotor1;								// making negative pwm value positive

	#ifdef DEBUG_MOTOR
			Serial.println("Motor1: AntiClockwise");
	#endif
		}
		else
		{
			digitalWrite(direction1,LOW);
			digitalWrite(direction2,LOW);					// V3 set brake
			T_pwmotor = 0;

	#ifdef DEBUG_MOTOR
			Serial.println("Motor1: Stop");
	#endif
		}
	}
	// only update PWM if changed
	if (T_pwmotor != PrevPwmMotor1){
		PrevPwmMotor1 = T_pwmotor;
		ledcWrite(channel_A, (T_pwmotor * 100 ) / 255);
	}

	UpdateMotors();
	return true;
}

bool motorControls::runMotor2(uint8_t pwm,uint8_t direction1,uint8_t direction2)
{
	float T_pwmotor;
	// setup only first time
	if(prevMotor2pin != pwm)
	{
		pinMode(direction1,OUTPUT);
		pinMode(direction2,OUTPUT);

		for (int i = 0 ; i < 6; i++) {
			if (pwm == valid_PWM_pins[i])
				channel_B = i;
		}
		if (channel_B == 0xff) return false;

		ledcAttachPin(pwm,channel_B);
		ledcSetup(channel_B,Motorfreq,8);

		prevMotor2pin = pwm;
	}
	
		// STOP if no update received after have been disconnected
	if (Motor_Disconnected) {
		digitalWrite(direction1,LOW);
		digitalWrite(direction2,LOW);					// V3 set brake
		T_pwmotor = 0;

#ifdef DEBUG_MOTOR
		Serial.println("Motor2: Stop, NO update after disconnect");
#endif
	}
	else 
	{
		if (pwmMotor2 > 0)
		{
			digitalWrite(direction1,HIGH); 				// V3 move forward / Clockwise
			if(! IsArduinoV3) digitalWrite(direction2,LOW);
			else digitalWrite(direction2,HIGH);		// release brake on V3
			T_pwmotor = pwmMotor2;

	#ifdef DEBUG_MOTOR
			Serial.println("Motor2: Clockwise");
	#endif
		}
		else if (pwmMotor2 < 0)
		{
			digitalWrite(direction1,LOW);					// V3 move backward /AntiClockwise
			digitalWrite(direction2,HIGH);				// release brake on V3
			T_pwmotor = -pwmMotor2;  							// making negative pwm value positive

	#ifdef DEBUG_MOTOR
			Serial.println("Motor2: AntiClockwise");
	#endif
		}
		else
		{
			digitalWrite(direction1,LOW);
			digitalWrite(direction2,LOW);					// V3 set brake
			T_pwmotor = 0;

	#ifdef DEBUG_MOTOR
			Serial.println("Motor2: Stop");
	#endif
		}
	}

		// only update PWM if changed
	if (T_pwmotor != PrevPwmMotor2){
		PrevPwmMotor2 = T_pwmotor;
		ledcWrite(channel_B, (T_pwmotor * 100) / 255);
	}

	UpdateMotors();
	return true;
}

// update the PWM pin if changed
void motorControls::UpdateMotors() {
 
	for (int i = 0 ; i < 6; i++) {
		// if updateDuty
		if (channels[i].updateDuty){

			if (!channels[i].initialized) {
				pinMode(channels[i].pin, OUTPUT);
				channels[i].pwm->begin(channels[i].freq, channels[i].duty_perc);
				channels[i].initialized = true;
			}

			channels[i].pwm->pulse_perc(channels[i].duty_perc);

			channels[i].updateDuty = false;
		}
	}
}

bool motorControls::runServo1(uint8_t pin)
{
	// init once
	if (prevServo1pin != pin)
	{
		for (int i = 0 ; i < 6; i++) {
			if (pin == valid_PWM_pins[i])
				channel_C = i;
		}
		if (channel_C == 0xff) return false;
		ledcAttachPin(pin,channel_C);
		ledcSetup(channel_C, Servofreq, 16);
		prevServo1pin = pin;
	}
	
	// only update if changed
	if (angleServo1 != PrevangleServo1){
		writeServoAngle(angleServo1,channel_C);
		PrevangleServo1 = angleServo1;
	}

	return true;
}

bool motorControls::runServo2(uint8_t pin)	 //Attach Servo2 to channel 
{
  // init once
	if (prevServo2pin != pin)
	{
		for (int i = 0 ; i < 6; i++) {
			if (pin == valid_PWM_pins[i])
				channel_D = i;
		}
		if (channel_D == 0xff) return false;

		ledcAttachPin(pin,channel_D);
		ledcSetup(channel_D, Servofreq, 16);
		prevServo1pin = pin;
	}

	// only update if changed
	if (angleServo2 != PrevangleServo2){
		writeServoAngle(angleServo2,channel_D);
		PrevangleServo2 = angleServo2;
	}
	
	return true;
}

int motorControls::angleTomicroseconds(int degree)
{
	degree = constrain(degree,minAngle,maxAngle);
	return map(degree,minAngle,maxAngle,minPulseWidth,maxPulseWidth);
}

// NOT USED from original version
int motorControls::microsecondsToDuty(int pulse)
{
	pulse=constrain(pulse,minPulseWidth,maxPulseWidth);
	return map(pulse,0,20000,0,65535);
}

void motorControls::writeServoAngle(int angle,uint8_t channel)
{
	int _pulse = angleTomicroseconds(angle);
	float _duty = _pulse / (10000 / Servofreq);
	ledcWrite(channel,_duty);

#ifdef DEBUG_MOTOR
		Serial.print(angle);
		Serial.print(" channel ");
		Serial.print(channel);
		Serial.print(" duty ");
		Serial.println(_duty);
#endif

	UpdateMotors();
}

