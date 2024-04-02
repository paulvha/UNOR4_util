/* updated 28 Feb 2024
 * by paulvha
 * 
 * tested on UnoR4 library 1.1.0
 */
#ifndef LedControlModule_h
#define LedControlModule_h

#include "ModuleParent.h"
#include "pwm.h"

#define MAX_PWM_CHANNELS  6
#define MAX_DIGITAL_CHANNELS  8
#define MAX_TOTAL_CHANNELS  (MAX_PWM_CHANNELS + MAX_DIGITAL_CHANNELS)
#define INVALID 0xfff

// info about each channel
struct L_channel_config {
  uint8_t pin;
  uint8_t brightness;
  float   freq;
  float   duty_perc;
  bool    initialized;
  bool    updateBright;
  bool    updateFreq;
  PwmOut  *pwm;
};

class LedControlModule : public ModuleParent
{
public:

	//Constructor
	LedControlModule();
	
	public:
		// 0 = error, 1 = pwm, 2 = digital
		uint8_t ChannelType(int ch);
		uint8_t PinType(int pin);

		// 0 - 255
		uint8_t readBrightness(int ch);
		
		// true = on, false = off
		bool    getpinState(int ch);
		
		// 0 = error , else pin
		uint8_t getpinNumber(int ch);
		
		// PWM frequency
		int getFrequency(int ch);
		
		// get existing or create new channel
		// INVALID == error, else channel
		int GetChannel(int pin);
		
		// do update of channels
		void 		updatechannels();
		
		// Setup channel routine
		void ChannelSetup(uint8_t ch, int freq);
		void ChannelAttachPin(uint8_t ch, uint8_t pin);
		void ChannelWrite(uint8_t ch, uint8_t value);
	private:
		// valid PWM pins on UNOR4
		uint8_t valid_PWM_pins[6] = {D3,D5,D6,D9,D10,D11};
		
		//Reserved Variables 
		uint8_t pin;
		uint8_t brightness=0;
		uint8_t value;
		uint8_t usedDigitalChannel = 0; 
		uint8_t prevPin=0; 
		uint8_t currentChannel=0;
		L_channel_config channels[MAX_TOTAL_CHANNELS] = {0};
		
		//Process Input Data  
		void processData();
		
		// init
		void InitPWMChannel(int ch);
		void InitAllChannels(); 
};

//Extern Object
extern LedControlModule LedControl;

#endif //LedControlModule_h
