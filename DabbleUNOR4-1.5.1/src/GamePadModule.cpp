/* updated 28 Feb 2024
 * by paulvha
 * 
 * tested on UnoR4 library 1.1.0
 */
 
#define FROM_DABBLE_LIBRARY
#include "DabbleUNOR4.h"
#include "GamePadModule.h"
#define PI 3.14159

// uncomment to enable debug messages
//#define DEBUG_GAME

#ifndef DEBUG_GAME
	#ifdef DEBUG_ALL
		#define DEBUG_GAME
	#endif
#endif

//Class Constructor
GamePadModule::GamePadModule() : ModuleParent(GAMEPAD_ID)
{
	value =0;
	value0=0;
}

//Up ArrowChecker
bool GamePadModule::isUpPressed()
{
	if(mode == 0)	return (value & (1<<UP_BIT));
  else	return 0;
}

//Down Arrow Checker 
bool GamePadModule::isDownPressed()
{
	if(mode == 0)	return (value & (1<<DOWN_BIT));
	else	return 0;
}

//Left Arrow Checker
bool GamePadModule::isLeftPressed()
{   
	if(mode == 0)	return (value & (1<<LEFT_BIT));
  else	return 0;
}

//Right Arrow Checker
bool GamePadModule::isRightPressed()
{
	if(mode == 0)	return (value & (1<<RIGHT_BIT));
  else	return 0;
}

//Orange Button Checker
bool GamePadModule::isStartPressed()
{
	return (value0 & (1<<START_BIT));
}

//Red Button Checker 
bool GamePadModule::isSelectPressed()
{
	return (value0 & (1<<SELECT_BIT));
}

//Green Button Checker 
bool GamePadModule::isTrianglePressed()
{
	return (value0 & (1<<TRIANGLE_BIT));
}

//Blue Button Checker
bool GamePadModule::isCirclePressed()
{
	return (value0 & (1<<CIRCLE_BIT));
}

bool GamePadModule::isCrossPressed()
{
	return (value0 & (1<<CROSS_BIT));
}

bool GamePadModule::isSquarePressed()
{
	return (value0 & (1<<SQUARE_BIT));
}

// get keystatus
bool GamePadModule::isPressed(uint8_t a)   
{
	switch(a){
		case 0:
			return(isUpPressed());
			break;
		case 1:
			return(isDownPressed());
			break;
		case 2:
			return(isLeftPressed());
			break;
		case 3:
			return(isRightPressed());
			break;
		case 4:
			return(isStartPressed());
			break;
		case 5:
			return(isSelectPressed());
			break;
		case 6:
			return(isTrianglePressed());
			break;
		case 7:
			return(isCirclePressed());
			break;
		case 8:
			return(isCrossPressed());
			break;
		case 9:
			return(isSquarePressed());
			break;
		default:
			return(false);
			break;
	}
}

//GamePad Input Data Processing  
void GamePadModule::processData()
{
	//Checking Function-ID
	byte functionId =getDabbleInstance().getFunctionId();
	
	if(functionId==GAMEPAD_DIGITAL) mode=0;
	else if(functionId == GAMEPAD_ANALOG) mode=1;
	else if(functionId == GAMEPAD_ACCL)	mode=1;

	value0=getDabbleInstance().getArgumentData(0)[0]; 
	value=getDabbleInstance().getArgumentData(0)[1];
	
#ifdef DEBUG_GAME
	Serial.print("GamePad:processData, mode: ");
	Serial.print(mode);
	Serial.print("value0: 0b");
	Serial.print(value0,BIN);
	Serial.print("value: 0b");
	Serial.println(value,BIN);
#endif
}

float GamePadModule::getJoystickData(uint8_t b)
{
	switch(b){
		case 0:
			return(getAngle());
			break;
		case 1:
			return(getRadius());
			break;
		case 2:
			return(getXaxisData());
			break;
		case 3:
			return(getYaxisData());
			break;
		default:
			return 0;
			break;
	}
}

uint16_t GamePadModule::getAngle()
{
	if(mode == 1)	return ((value >> 3)*15);
	else	return 0;
}

uint8_t GamePadModule::getRadius()
{
	if(mode == 1)	return (value&0x07);
	else	return 0;
}

float GamePadModule::getXaxisData()
{
	if(mode != 1) return 0;
	uint16_t angle=((value >> 3)*15);
	uint8_t radius=value&0x07;
	float x_value= float(radius*(float(cos(float(angle*PI/180)))));
	return x_value;
}

float GamePadModule::getYaxisData()
{
	if(mode != 1) return 0;
	uint16_t angle=((value >> 3)*15);
	uint8_t radius=value&0x07;
	float y_value= float(radius*(float(sin(float(angle*PI/180)))));
	return y_value;
}

float GamePadModule::getx_axis()
{
	return(getXaxisData());
}

float GamePadModule::gety_axis()
{
	return(getYaxisData());
}
