# TRW_AnalogTouch

A simple touch detection script for Arduino-compatible boards using their analog pins.

Tested on these boards:
 - XIAO ESP32-C3
 - SparkFun ESP32-Thing
 - SparkFun Pro Micro
 - Digispark Attiny85

There are two .ino script files for Arduino IDE to serve as an idea for your projects.

With touch detection you avoid the use of push buttons and save a lot of physical space in your project.

Original idea came from here:
https://www.esp32.com/viewtopic.php?t=23066

## Process description
1. Read an array of ADC values and calculate the mean value.
2. If the last value is 200mV lower than the average, touch is recognised.

# ESP32C3 example

This example only prints touch detection on serial monitor.
It can be used in any compatible board with analog pins.

## Usage
1. Connect a jumper wire to any ADC pin on your board.
2. Define ADC pin as **TOUCHPIN** in the script '#define's.
3. Load script to your board using Arduino IDE.
4. Calibrate following guidelines for fine performance.

# Pro Micro Blink example

This example prints touch detection on serial monitor and lights a LED connected to any output pin.

## Usage
1. Connect a led to any output PIN, then ground it via a resistor.
2. Connect a jumper wire to any ADC pin on your board.
3. Define ADC pin as **TOUCHPIN^ and led pin as **LEDPIN** in the script '#define's.
4. Load script to your board using Arduino IDE.
5. Calibrate following guidelines for fine performance.


# Calibration variables

VALUES: number of values used in array to calculate ADC mean value.
  --> Higher value, more efficiency. Default is 20.

REFV: voltage drop (in mV) that triggers touch detection.
  --> Default is 750. Lower values increase sensitivity but it can trigger itself without touch. Up to 200 is advisable with good efficiency. 

VALINTERVAL: comparison delay time in ms.
  --> Lower value increases CPU load but reduces efficiency. Default is 100.

## Calibration guide
1. Set a high REFV, for example 1000
2. Run the extended script with OUT 1
3. Open Serial Monitor and have a look at the average values for a while (e.g.: 230)
4. Now touch the jumper wire and see what the values are now (e.g.: 170)
5. Make the difference between the average at 3. and the average at 4 (230-170=60).
6. Set your REFV to this value (60) and play around it.
- For more sensitivity, set it a bit lower (50)
- If there is detection with no touch, increase it (80)
- Because of CPU load, REFV value will vary if OUT 1 or OUT 0
- Touch could trigger itself for a few seconds during start, as analog readings are higher. Set a WARMUP waiting time for readings to start.

# Debugging

Monitor output can be reduced or extended setting the OUT variable to 0 or 1

1: extended --> Prints ADC array, mean value, touch count and touch message with touch count

0: minimal  --> Prints touch message with touch count
