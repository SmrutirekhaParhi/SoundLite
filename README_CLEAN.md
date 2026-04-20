# SOUNDLITE- Sound Activated Lighting System
A sound-activated RGB lighting system using Arduino Nano. Detects ambient sounds (claps, music beats) and triggers color-changing LED lights in real-time.
## Overview
This project demonstrates a simple microcontroller application that uses sound detection to control lighting. The system cycles through red, green, and blue LED colors with each detected sound using external interrupts and software debouncing.
## Hardware Requirements
- Arduino Nano (ATmega328P)
- Sound Sensor Module (digital output)
- RGB LED (common cathode, 5mm)
- 3x 220 Ohm resistors
- Jumper wires
- Mini breadboard
- USB cable (Micro USB)
- 5V power supply (optional)
## Pin Configurations
Arduino Nano Pinout:
Pin 3  -> Sound Sensor (Digital Output)
Pin 7  -> Red LED (via 220 ohm resistor)
Pin 8  -> Green LED (via 220 ohm resistor)
Pin 9  -> Blue LED (via 220 ohm resistor)
5V     -> Power
GND    -> Ground
## How it works
1. Sound sensor detects audio and outputs a digital signal
2. Arduino Pin 3 (external interrupt) detects rising edge
3. Interrupt service routine (color_swap) is triggered
4. Software debounce prevents false triggers (300ms delay)
5. Next LED color is activated in sequence: Red -> Green -> Blue -> Red
## Installation
1. Install Arduino IDE from arduino.cc
2. Open soundlite.ino in Arduino IDE
3. Select Tools -> Board -> Arduino Nano
4. Select Tools -> Processor -> ATmega328P
5. Select your COM port
6. Click Upload
## Usage
1. Power on the system
2. Red LED will light up
3. Clap or make a loud sound near the sensor
4. LED will change to green
5. Repeat to cycle through colors
## Code Files
- soundlite.ino - Basic version with sound detection and LED cycling
- soundlite_advanced.ino - Advanced version with serial console control
