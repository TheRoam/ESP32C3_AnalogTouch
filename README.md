# TRW_AnalogTouch

A simple touch detection script for Arduino-compatible boards using their analog pins.

Tested on these boards:
 - XIAO ESP32-C3
 - SparkFun ESP32-Thing
 - SparkFun Pro Micro
 - Digispark Attiny85

This is an .ino script file for Arduino IDE to serve as an idea for your projects.

With touch detection you avoid the use of push buttons and save a lot of physical space in your project.

Original idea came from here:
https://www.esp32.com/viewtopic.php?t=23066

## Process description
1. Read an array of ADC values and calculate the mean value.
2. If the last value is 200mV lower than the average, touch is recognised.

## Usage
1. Connect a jumper wire to any ADC pin on your board.
2. Define ADC pin as TP in the script.
3. Load script to your board using Arduino IDE.

The example uses pin D1 (GPIO3) of a XIAO ESP32-C3

https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/#pinout-diagram

## Calibration variables

VALUES: number of values used in array to calculate ADC mean value.
  --> Higher value, more efficiency. Default is 20.

REFV: voltage drop (in mV) that triggers touch detection.
  --> Default is 750. Lower values increase sensitivity but it can trigger itself without touch. Up to 200 is advisable with good efficiency. 

VALINTERVAL: comparison delay time in ms.
  --> Lower value increases CPU load but reduces efficiency. Default is 100.
  
## Debugging

Monitor output can be switched to two values in "out" variable.

1: extended --> Prints ADC array, mean value, touch count and touch message with touch count

0: minimal  --> Prints touch message with touch count
