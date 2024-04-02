/* 
 * In case the BLE disconnects the returned values will be zero.
 * 
 * updated 6 Mar 2024
 * by paulvha
 * 
 * tested on UnoR4 library 1.1.0
 */

#define FROM_DABBLE_LIBRARY
#include "DabbleUNOR4.h"
#include "DabbleInputs.h"

bool Input_Disconnected = true;

// called when BLE disconnects after have been connected
void OnInputDisconnect() {
	Input_Disconnected = true;
}

DabbleInputs::DabbleInputs() : ModuleParent(EVIVEINTERFACES_ID)
{
	// set callback when disconnected
	getDabbleInstance().SetDisconnectCallBack(OnInputDisconnect);
}

uint16_t DabbleInputs::getvalue_Pot1()
{
	if (Input_Disconnected) return 0;
	return uint16_t(data_1 << 8) + uint16_t (data_2);
}

uint16_t DabbleInputs::getvalue_Pot2()
{
	if (Input_Disconnected) return 0;
	return uint16_t(data_3 << 8) + uint16_t (data_4);
}

uint8_t DabbleInputs::getStatus_SlideSwitch1()
{
	if (Input_Disconnected) return 0;
	
	if((data_5 & 0x04) == 0x04)
	{
		state_ss1 = 2;
	}
	else if((data_5 & 0x02) == 0x02)
	{
		state_ss1 = 0;
	}
	else if((data_5 & 0x01) == 0x01)
	{
		state_ss1 =1;
	}
	return state_ss1;
}

uint8_t DabbleInputs::getStatus_SlideSwitch2()
{
	if (Input_Disconnected) return 0;
	
	if((data_5 & 0x20) == 0x20)
	{
		state_ss2 = 2;
	}
	else if((data_5 & 0x10) == 0x10)
	{
		state_ss2 = 0;
	}
	else if((data_5 & 0x08) == 0x08)
	{
		state_ss2 = 1;
	}
	return state_ss2;
}

bool DabbleInputs::getStatus_TactileSwitch1()
{
	if (Input_Disconnected) return 0;
	
	return ((data_5 & 0x40) == 0x40);
}

bool DabbleInputs::getStatus_TactileSwitch2()
{
	if (Input_Disconnected) return 0;
	return ((data_5 & 0x80) == 0x80);
}

uint16_t DabbleInputs::getPot1Value()
{
	if (Input_Disconnected) return 0;
	return uint16_t(data_1 << 8) + uint16_t (data_2);
}

uint16_t DabbleInputs::getPot2Value()
{
	if (Input_Disconnected) return 0;
	return uint16_t(data_3 << 8) + uint16_t (data_4);
}

uint8_t DabbleInputs::getSlideSwitch1Value()
{
	if (Input_Disconnected) return 0;
	
	if((data_5 & 0x04) == 0x04)
	{
		state_ss1 = 2;
	}
	else if((data_5 & 0x02) == 0x02)
	{
		state_ss1 =0;
	}
	else if((data_5 & 0x01) == 0x01)
	{
		state_ss1 =1;
	}
	return state_ss1;
}

uint8_t DabbleInputs::getSlideSwitch2Value()
{
	if (Input_Disconnected) return 0;
	
	if((data_5 & 0x20) == 0x20)
	{
		state_ss2 = 2;
	}
	else if((data_5 & 0x10) == 0x10)
	{
		state_ss2 = 0;
	}
	else if((data_5 & 0x08) == 0x08)
	{
		state_ss2=1;
	}
	return state_ss2;
}

bool DabbleInputs::getTactileSwitch1Value()
{
	if (Input_Disconnected) return 0;
	return ((data_5 & 0x40) == 0x40);
}

bool DabbleInputs::getTactileSwitch2Value()
{
	if (Input_Disconnected) return 0;
	return ((data_5 & 0x80) == 0x80);
}

bool DabbleInputs::getSlideSwitchStatus(uint8_t SS,uint8_t dir)
{	
	if (Input_Disconnected) return 0;
	if (SS == 1 && dir == 2)   //SS1 left
	{
		if((data_5 & 0x01) == 0x01)
			return 1;
		else
			return 0;
	}

	else if(SS == 1 && dir == 3)   //SS1 right
	{
		if((data_5 & 0x04) == 0x04)
			return 1;
		else
			return 0;
	}
	
	else if(SS == 1 && dir == 1)   //SS1 Off
	{
		if((data_5 & 0x02) == 0x02)
			return 1;
		else
			return 0;
	}
	
	else if(SS == 2 && dir == 2)  //SS2 left
	{
		if((data_5 & 0x08) == 0x08)
			return 1;
		else
			return 0;
	}
	
	else if(SS == 2 && dir == 3)  //SS2 right
	{
		if((data_5 & 0x20) == 0x20)
			return 1;
		else
			return 0;
	}
	
	else if(SS == 2 && dir == 1)  //SS2 Off
	{
		if((data_5 & 0x10) == 0x10)
			return 1;
		else
			return 0;
	}
}

bool DabbleInputs::getTactileSwitchStatus(uint8_t TS)
{
	if (Input_Disconnected) return 0;
	if(TS == 1)
	{
		if((data_5 & 0x40) == 0x40)
			return 1;
		else 
			return 0;
	}
	else if(TS == 2)
	{
		if((data_5 & 0x80) == 0x80)
			return 1;
		else 
			return 0;
	}
}

int DabbleInputs::getPotValue(uint8_t Pot)
{
	if (Input_Disconnected) return 0;
	if(Pot == 1)
	{
		return uint16_t(data_1 << 8) + uint16_t (data_2);
	}
	else if(Pot == 2)
	{
		return uint16_t(data_3 << 8) + uint16_t (data_4);
	}
}

void DabbleInputs::processData()
{
	/*#ifdef DEBUG
	Serial.println("DabbleInputs:processData");
	#endif*/
	
	Input_Disconnected= false;
	
	//Checking Function-ID
  byte functionId =getDabbleInstance().getFunctionId();
	if(functionId == Potentiometer_1)
	{
		data_1=getDabbleInstance().getArgumentData(0)[0]; 
		data_2=getDabbleInstance().getArgumentData(0)[1];
	}
	else if(functionId == Potentiometer_2)
	{
		data_3=getDabbleInstance().getArgumentData(0)[0]; 
		data_4=getDabbleInstance().getArgumentData(0)[1];
	}
	else if(functionId == Switches) 
	{
		data_5=getDabbleInstance().getArgumentData(0)[0]; 
	}
}
