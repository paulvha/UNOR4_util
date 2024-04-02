/* updated 28 Feb 2024
 * by paulvha
 * 
 * tested on UnoR4 library 1.1.0
 */

#ifndef BLE_UTILITIES_H
#define BLE_UTILITIES_H
#include <ArduinoBLE.h>

#define UUID_Service  "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define UUID_Transmit "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"		// to controller
#define UUID_Receive  "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"		// from controller

class UnoR4ble
{
	public:
		void begin(std::string a);
		void write(uint8_t a);
		void write(std::string x);
		void write(int a) {write( (uint8_t) a);}
		void write(float a);
		uint8_t available();
		uint8_t read();
		void stop();
	private:
		void WaitOnNotify();
};

extern bool BLE_status;        //extern BLE_status
extern bool isDabbleConnected;
extern UnoR4ble unoR4ble;
#endif  // BLE_UTILITIES_H
