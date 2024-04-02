# Dabble on UNOR4 Wifi

Based on the original ESP32 library,[https://github.com/STEMpedia/DabbleESP32], this is a version is working on an Arduino UNOR4 Wifi.
updated March / April 2024, Paulvha, Tested on UNOR4 library 1.1.0

## UNOR4 Wifi and Android Dabble app
I have used a Samsung Galaxy S21 FE 5G during the tests to the UNOR4 Wifi. The following sections are available in the app :
Phone sensors, led brightness, Terminal, gamepad, led brightness, pinstate monitor, motor control, inputs, camera, IOT (Data Logger, Notifications, Internet), Music.

The phone sensor section of the APP crashes constant when trying to enable sending some of the data. 
The notifications section in the app does not work. It disconnects when something is received.

## UNOR4 Wifi and IOS Dabble app
I have also tried an Iphone14 PRO. Only the following sections are available in the app :
Phone sensors, led brightness, Terminal, gamepad, led brightness and pinstate monitor

Th IPhone finds the MyUNOR4 device only one time under that name. In later connects it will show up as Arduino, while the advertizing name has not changed.

The IPHONE kills with the UNOR4 Wifi with WAY to much information over BLE. It becomes very slow and lacking behind. Unworkable !!.
The Phone sensor section works for Accelerometer, Gyroscope, magnetometer, soundmeter, location and Barometer, but again very slow

# Original ReadMe

## Background
Whether you’re a student, a teacher, or a hobbyist, Dabble is the perfect place for all your DIYing needs. It transforms your Smartphone into a virtual I/O device and lets you control hardware via Bluetooth, communicate with it, access sensors like accelerometer, GPS, proximity and other features of your Smartphone. It also provides you with dedicated projects compatible with Scratch and Arduino to help you learn by doing. Currently Dabble is avaiable for Android users(coming soon for iPhone users) and its is supported on Android version 5.0.0 and above. One can download the app from Google PlayStore with this link given below. [https://play.google.com/store/apps/details?id=io.dabbleapp]

The app is designed to provide data of certain features of your smartphone to evive and microcontroller boards like Arduino Uno, Nano, Mega, ESP32 and UNOR4 Wifi.It also allows you to control them by your Smartphone. The app communicates with these boards via bluetooth modules like HC-05, HC-06 and HM-10 in case of Arduino boards and with built-in bluetooth for ESP32 and UNOR4 Wifi. This repository consists of library required on your board side for communication with app using BLE on UNOR4 Wifi (based on ESP32 library). 

## Dabble app

Dabble consists of various modules that allows you to control your hardware and access various smartphone features. A brief description of each module is mentioned below:

1) Led Brightness Control: This module allows you to control digital pin of your hardware through mobile. You can make pins HIGH or LOW, can also change its PWM if PWM functionailty is supported on that pin. You can use this module to control LED brightness and carry other such activities.
2) Terminal: Send and receive text and voice commands over Bluetooth between your hardware and smartphone.
3) Gamepad: Control devices in analog (Joystick), digital, and accelerometer mode. (pvh: last one not implemented yet)
4) Pin State Monitor: Remotely monitor the live status of devices and debug them.
5) Motor Control: Control actuators such as the DC motor and servo motor.
6) Inputs: Provide analog and digital inputs via buttons, knobs, and switches.
7) Camera: Use the camera of your Smartphone for taking photos and videos and colour detection.
8) Phone Sensor: Access different sensors of your Smartphone such as the accelerometer, gyroscope, proximity sensor,magnetometer, light meter, sound meter, GPS, temperature sensor, and barometer to make projects and conduct experiments. (see Android and IOS below)
9) Oscilloscope: Visualise and analyse the input and output signals given to the device using the oscilloscope module. (?????? not found)
10) IoT: This module consist of Data Logger Module. See top of sketch
    IoT: This module consist of Notifcation Module. See top of sketch. pvh: crashes on Samsung
11) Music: Receive commands from the device and play tones, songs, or other recorded files on your Smartphone.
12) Internet : pvh: not implemented on UNOR4 Wifi.See https://forum.arduino.cc/t/rf-coexistence-ble-wifi-in-uno-r4-wifi/1223000
12) Projects: Make dedicated projects to experience different concepts of the real world first-hand.

## Dabble Library
As per the module set present in Dabble app the library is also structured in same way. This library is for UNOR4 Wifi. It consists of certain examples codes which will help you in getting started with various Dabble modules. To explore more about Dabble, goto [https://thestempedia.com/docs/dabble/getting-started-with-dabble/]



