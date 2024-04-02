/*
 * This example will allow setting each of the 6 PWM pins for an UNOR4 independently.
 * 
 * Each pin is assigned to a channel. In each channel the PWM information for the pin is stored.
 * In this example an update of the PWM information can be performed from the keyboard. In other 
 * situations this could (also) be done from remote (Bluetooth or WiFi).
 * 
 * All channels are then checked and pin is updated as necessary.
 * 
 * updated 28 Feb 2024
 * by paulvha
 * 
 * tested on UnoR4 library 1.1.0
 */
#define FROM_DABBLE_LIBRARY
#include "DabbleUNOR4.h"
#include "LedControlModule.h"

// uncomment to enable debug messages
//#define DEBUG_LED

#ifndef DEBUG_LED
	#ifdef DEBUG_ALL
		#define DEBUG_LED
	#endif
#endif

// default constructors.. only pin will be assigned
PwmOut L_pwm3(3);
PwmOut L_pwm5(5);
PwmOut L_pwm6(6);
PwmOut L_pwm9(9);
PwmOut L_pwm10(10);
PwmOut L_pwm11(11);

LedControlModule::LedControlModule(): ModuleParent(LEDCONTROL_ID)
{
	InitAllChannels();
}

// update PWM frequency
void LedControlModule::ChannelSetup(uint8_t ch, int freq) {
	channels[ch].freq = freq;
	channels[ch].updateFreq = true;
}

// assign a pin to a channel
void LedControlModule::ChannelAttachPin(uint8_t ch, uint8_t pin) {
	channels[ch].pin = pin;
}

// update brightness /duty percentage
void LedControlModule::ChannelWrite(uint8_t ch, uint8_t value) {
	channels[ch].brightness = value;
	channels[ch].duty_perc = (float) (value*100) / 255;
	channels[ch].updateBright = true;
}

// init all the pins
void LedControlModule::InitAllChannels() {
	
	for (int i = 0 ; i < MAX_TOTAL_CHANNELS ; i++)
	{
    if (i < MAX_PWM_CHANNELS) InitPWMChannel(i);
    else {
			channels[i].pin = 0x0;						// init digital channel
			channels[i].initialized = false;
			channels[i].brightness = 0;
			channels[i].freq = 0;
		}
	}
}

// init a PWM channel
void LedControlModule::InitPWMChannel(int ch) {

	if (ch > MAX_PWM_CHANNELS) return;
  
  channels[ch].pin = 0x0;
  channels[ch].initialized = false;
  channels[ch].brightness = 0;
  channels[ch].duty_perc = 0;
  channels[ch].freq = 490;
  channels[ch].updateBright = false;
  channels[ch].updateFreq = false;
  
  switch (ch) {
    case 0: channels[ch].pwm = &L_pwm3;
      break;
    case 1: channels[ch].pwm = &L_pwm5;
      break;
    case 2: channels[ch].pwm = &L_pwm6;
      break;
    case 3: channels[ch].pwm = &L_pwm9;
      break;
    case 4: channels[ch].pwm = &L_pwm10;
      break;
    case 5: channels[ch].pwm = &L_pwm11;
      break;
  }
}

// process incoming frame
void LedControlModule::processData()
{
	byte functionId = getDabbleInstance().getFunctionId();
	  
	if (functionId == 0x01)		// set pin
	{
		pin = getDabbleInstance().getArgumentData(0)[0]; 	

		if(prevPin != pin)		// if not same as last
		{
			currentChannel = GetChannel(pin);

			if (currentChannel == INVALID) {

#ifdef DEBUG_LED
				if (pin > PINS_COUNT){
					Serial.print("Invalid: to large pin number: ");
					Serial.print(pin);
					Serial.println(". Dropping request");
				}
				else {
					Serial.println("Channel table is full. Dropping request");
				}
#endif
				pin = INVALID;		// not assigned
				return;
			}

#ifdef DEBUG_LED
			if (currentChannel < MAX_PWM_CHANNELS) {
				Serial.print("\nTreat as PWM pin: ");
			}
			else		// digital pin handling
			{
				Serial.print("\nTreat as digital pin: ");
			}
			Serial.println(pin);
#endif
			prevPin = pin;
		}
	}
	else if (functionId == 0x03)    //ON as per Brightness value
	{
		if (pin == INVALID) return;
		brightness = getDabbleInstance().getArgumentData(0)[0]; 	 
		value = map(brightness,0,100,0,255);
		ChannelWrite(currentChannel, value);
		
#ifdef DEBUG_LED
		Serial.print("Bright: ");
		Serial.print(brightness);
		Serial.print(" val: ");
		Serial.println(value);
#endif

	}
	else if(functionId == 0x02)    // OFF
	{
		if (pin == INVALID) return;
#ifdef DEBUG_LED
		Serial.println("OFF");
#endif
		ChannelWrite(currentChannel, 0);
	}

	updatechannels();
}

//check and apply any change that has been made. 
void LedControlModule::updatechannels() {
  
  for (int i = 0 ; i < MAX_TOTAL_CHANNELS ; i++) {

    // if frequency has been changed (valid for PWM only)
    if (channels[i].updateFreq) {
      channels[i].initialized = false;        // force re-init
      channels[i].updateFreq = false;
    }

		// if not initialized , but there is pin AND brightness is larger than zero
    if (! channels[i].initialized && channels[i].pin != 0 && channels[i].brightness > 0) {
      pinMode(channels[i].pin, OUTPUT);
      
      if (i < MAX_PWM_CHANNELS) {
				channels[i].pwm->begin(channels[i].freq, channels[i].duty_perc);
				channels[i].updateBright = false;
      }
      channels[i].initialized = true;
    }

    // if only brightness / duty cycle has been changed
    if (channels[i].updateBright && channels[i].initialized) {
			
			if (i < MAX_PWM_CHANNELS) {
				channels[i].pwm->pulse_perc(channels[i].duty_perc);
      }
      else {
				if(channels[i].brightness > 250)
				{
					digitalWrite(channels[i].pin,HIGH);
				}
				else
				{
					digitalWrite(channels[i].pin,LOW);
				}
			}
			channels[i].updateBright = false;
		}
  }
}

// what kind of pin is allocated to the channel
// 1  = PWM, 2 = digital, 0 = error
uint8_t LedControlModule::ChannelType(int ch) {
	if (ch > MAX_TOTAL_CHANNELS) return 0;
	
	if (ch < MAX_PWM_CHANNELS)	return 1;
	else return 2;
}

// what kind of pin
// 1  = PWM, 2 = digital, 0 = error
uint8_t LedControlModule::PinType(int pin) {
	if (pin > PINS_COUNT) return 0;
	
	for (int i = 0; i < MAX_PWM_CHANNELS ; i++) {
		// check for valid PWM Pin
		if (valid_PWM_pins[i] == pin) return 1;
	}

	return 2;
}

// get current brightness on channel
uint8_t LedControlModule::readBrightness(int ch)
{
	if (ch > MAX_TOTAL_CHANNELS) return 0;
	return channels[ch].brightness;
}

// get current status from pin on channel
// true = on, false = off
bool LedControlModule::getpinState(int ch)
{
	if (ch > MAX_TOTAL_CHANNELS) return 0;
	
	if (channels[ch].brightness > 250)
		return true; 
	else
		return false;
}

// get pin number on channel
uint8_t LedControlModule::getpinNumber(int ch)
{
	if (ch > MAX_TOTAL_CHANNELS) return 0;
	return channels[ch].pin;
}

// get frequency on channel
int LedControlModule::getFrequency(int ch)
{
	if (ch > MAX_TOTAL_CHANNELS) return 0;
	return channels[ch].freq;
}

// get channel, current or new
// INVALID is error en channel
int LedControlModule::GetChannel(int pin)
{
	int ch = INVALID;
	
	// is pin a PWM ?
  for (int i = 0; i < MAX_PWM_CHANNELS ; i++) {
    if (valid_PWM_pins[i] == pin) {
			ChannelAttachPin(i,pin);
			return i;
		}
	}
	
	// pin is digital, is a channel already set ?
	for (int i = MAX_PWM_CHANNELS; i < (MAX_PWM_CHANNELS + usedDigitalChannel); i++) {
		// check whether digital Pin was used before
		if (channels[i].pin == pin)  return i;
	}
		// no channel set for digital pin, try to allocate channel
	if (usedDigitalChannel < MAX_DIGITAL_CHANNELS) {
		ch = MAX_PWM_CHANNELS + usedDigitalChannel;
		ChannelAttachPin(ch,pin);
		usedDigitalChannel++;
	}

	return(ch);
}
