/* updated 28 Feb 2024
 * by paulvha
 * 
 * tested on UnoR4 library 1.1.0
 */

#include "UNOR4BLEUtilities.h"

bool BLE_status=0;

// remove comment to allow debug info in file
//#define DEBUG_UTIL

#define RXBUFFER_SIZE 240

bool isDeviceConnected=false;
bool prevDeviceConnected = false;
bool isSubscribed = false;
uint8_t tx_Value = 0;
uint8_t rxdatalength=0;
uint8_t WritePointer=0;
uint8_t ReadPointer=0;
uint8_t* rxdataBuffer=NULL;

BLEService DabbleService(UUID_Service);
BLECharacteristic DabbleRxCharacteristic(UUID_Transmit, BLEWrite, 20);
BLECharacteristic DabbleTxCharacteristic(UUID_Receive, BLENotify, 20);

/*----------------------------------------------*/
void onConnect(BLEDevice central) {
/*----------------------------------------------*/
  Serial.println("connected");
  isDeviceConnected = true;
  prevDeviceConnected = isDeviceConnected;
}

/*----------------------------------------------*/
void onDisConnect(BLEDevice central) {
/*----------------------------------------------*/
  Serial.println("Disconnected");
  isDeviceConnected = false;
  isDabbleConnected =false; // extern
  isSubscribed = false;
  prevDeviceConnected = isDeviceConnected;
}

// When subscribed to Notify
/*----------------------------------------------*/
void onSubscribe(BLECharacteristic c) {
/*----------------------------------------------*/
  isSubscribed = true;
}

// send something more to the remote
/*----------------------------------------------*/
void ReadCallBack(BLEDevice *central, BLECharacteristic c) {
/*----------------------------------------------*/
  Serial.println("ReadCallBack");
  // TODO
}

// receive from remote (smartphone or ...)
/*----------------------------------------------*/
void WriteCallBack(BLEDevice *central, BLECharacteristic c) {
/*----------------------------------------------*/
  //Serial.println("WriteCallBack");
  int lenc = c.valueLength();
  uint8_t *valuec = (uint8_t*) c.value();

  if (lenc > 0) {
    
    // drop if does not fit in buffer
    if (WritePointer + lenc > rxdatalength) return;
#ifdef DEBUG_UTIL
    Serial.print("WriteCallBack:");
#endif
    for (int i = 0; i < lenc; i++)
    {
      rxdataBuffer[WritePointer + i] = (uint8_t) valuec[i];
#ifdef DEBUG_UTIL
      Serial.print(" 0x");
      Serial.print(valuec[i], HEX);
#endif
    }

#ifdef DEBUG_UTIL
    Serial.println();
#endif

    WritePointer += lenc;
  }
}

/*----------------------------------------------*/
void UnoR4ble::begin(std::string a) {
/*----------------------------------------------*/
  Serial.println("Starting BLE");

  while(!BLE.begin())
  {
    Serial.println("Failed Starting BLE, retrying");
    delay(30);
  }

  // allocate a buffer
  if (rxdataBuffer == NULL ) {
    rxdataBuffer = new uint8_t[RXBUFFER_SIZE];
    
    if (rxdataBuffer) {
      rxdatalength = RXBUFFER_SIZE;
      WritePointer = ReadPointer = 0;
    }
    else {
      Serial.println("Can not obtain buffer"); 
      return;
    }
  }
  
  Serial.print("Setting device name as: "); Serial.println(a.c_str());
  BLE.setLocalName(a.c_str());

  // Create the BLE Server
  BLE.setAdvertisedService(DabbleService);
  DabbleService.addCharacteristic(DabbleTxCharacteristic);
  DabbleService.addCharacteristic(DabbleRxCharacteristic);
  BLE.addService(DabbleService);
  
  // set connection handlers
  BLE.setEventHandler(BLEConnected, onConnect);
  BLE.setEventHandler(BLEDisconnected, onDisConnect);

  Serial.println("Begin advertising.");
  
  // start advertising
  BLE.advertise();

  // called when read is performed by remote (and thus we write)
  DabbleTxCharacteristic.setEventHandler(BLERead, (BLECharacteristicEventHandler) ReadCallBack);
  
  // called when remote subcribed to notify
  DabbleTxCharacteristic.setEventHandler(BLESubscribed, (BLECharacteristicEventHandler) onSubscribe);

  // called when something written by remote (and thus we read)
  DabbleRxCharacteristic.setEventHandler(BLEWritten, (BLECharacteristicEventHandler) WriteCallBack);

  WaitOnNotify();
}

/*----------------------------------------------*/
void UnoR4ble::WaitOnNotify() {
/*----------------------------------------------*/
#ifdef DEBUG_UTIL
    Serial.println("Waiting a client connection to notify...");
#endif
  while(!isSubscribed) {
    BLE.poll();
  }
}

/*----------------------------------------------*/
void UnoR4ble::write(uint8_t a) {
/*----------------------------------------------*/

  if (isDeviceConnected) {
    DabbleTxCharacteristic.writeValue(a);
    delay(10); // bluetooth stack will go into congestion, if too many packets are sent
  }

  // disconnected
  if (!isDeviceConnected && prevDeviceConnected) {
    delay(500);       // give the bluetooth stack the chance to get things ready
    BLE.advertise();  // restart advertising
#ifdef DEBUG_UTIL
    Serial.println("re-start advertising");
#endif
    prevDeviceConnected = isDeviceConnected;
    WaitOnNotify();
  }
}

/*----------------------------------------------*/
void UnoR4ble::write(std::string x) {
/*----------------------------------------------*/
  if (isDeviceConnected) {

    if (x.length() > 0) {

      for (int i = 0; i < x.length(); i++)
      {
        DabbleTxCharacteristic.writeValue( (uint8_t) x[i]);
        delay(10);    // bluetooth stack will go into congestion, if too many packets are sent
      }
    }
  }
  
  // disconnected and previous connected
  if (!isDeviceConnected && prevDeviceConnected) {
    delay(500);       // give the bluetooth stack the chance to get things ready
    BLE.advertise();  // restart advertising
#ifdef DEBUG_UTIL
    Serial.println("start advertising");
#endif
    prevDeviceConnected = isDeviceConnected;
    WaitOnNotify();
  }
}

/*----------------------------------------------*/
void UnoR4ble::write(float a) {
/*----------------------------------------------*/
 Serial.println("write float");
 // todo
 
 // translate a float to uint8_t 4-size buffer

 // send the buffer
/*
  if (isDeviceConnected) {
    bleDabbleTxCharacteristic->setValue(a);
    delay(10); // bluetooth stack will go into congestion, if too many packets are sent
  }

    // disconnecting
  if (!isDeviceConnected && prevDeviceConnected) {
    delay(500); // give the bluetooth stack the chance to get things ready
    BLE.advertise(); // restart advertising
#ifdef DEBUG_UTIL
    Serial.println("start advertising");
#endif
    prevDeviceConnected = isDeviceConnected;
    WaitOnNotify();
  }
*/
}

/*----------------------------------------------*/
uint8_t UnoR4ble::available() {
/*----------------------------------------------*/

  BLE.poll();           // regular poll ArduinoBLE

  if (WritePointer == ReadPointer) {
    WritePointer = ReadPointer = 0;
  }
  
  return (WritePointer - ReadPointer);
}

/*----------------------------------------------*/
uint8_t UnoR4ble::read() {
/*----------------------------------------------*/
  uint8_t a = rxdataBuffer[ReadPointer];
  ReadPointer++;
  return a;
}

/*----------------------------------------------*/
void UnoR4ble::stop() {
/*----------------------------------------------*/
  BLE.stopAdvertise();
}

UnoR4ble unoR4ble;
