# SOUNDLITE - Setup Guide

## Hardware Assembly

### Step 1: Prepare Components

You will need:
- Arduino Nano
- Sound Sensor Module
- RGB LED (Common Cathode)
- 3 x 220 Ohm resistors
- Jumper wires
- Breadboard

### Step 2: Connect Power and Ground

1. Connect Arduino 5V to breadboard positive rail
2. Connect Arduino GND to breadboard negative rail
3. Connect Sound Sensor VCC to positive rail
4. Connect Sound Sensor GND to negative rail

### Step 3: Connect Sound Sensor

1. Connect Sound Sensor DO (Digital Output) to Arduino Pin 3
2. Verify connections are secure

### Step 4: Connect RGB LED

For each LED color:
1. Insert 220 Ohm resistor into breadboard
2. Connect resistor to Arduino pin

Red LED:
- Resistor -> Pin 7
- LED anode -> Resistor output

Green LED:
- Resistor -> Pin 8
- LED anode -> Resistor output

Blue LED:
- Resistor -> Pin 9
- LED anode -> Resistor output

LED Cathode (longest pin):
- Connect all cathodes together
- Connect to breadboard negative rail (GND)

### Step 5: Verify All Connections

Check the following:
- 5V power connected to power rail
- GND connected to ground rail
- Sound sensor connected to Pin 3
- Red LED resistor to Pin 7
- Green LED resistor to Pin 8
- Blue LED resistor to Pin 9
- All LED cathodes to ground

## Software Installation

### Install Arduino IDE

1. Download from arduino.cc
2. Install following the installer instructions
3. Launch Arduino IDE

### Install USB Drivers (if needed)

Windows: Download CH340 driver from manufacturer website
Mac/Linux: Usually works without additional driver

### Board Configuration

In Arduino IDE:
1. Tools -> Board -> Arduino AVR Boards -> Arduino Nano
2. Tools -> Processor -> ATmega328P
3. Tools -> Port -> Select your COM port

### Upload Code

1. Open soundlite.ino
2. Click Verify (check mark icon) to compile
3. Click Upload (arrow icon) to upload to Arduino
4. Wait for "Done uploading" message

## Testing

1. Power on the system
2. Red LED should light up
3. Clap loudly near the sound sensor
4. LED should change to green
5. Clap again, LED changes to blue
6. Clap again, LED changes back to red

If this works, your system is functioning correctly.

## Troubleshooting

LED not lighting:
- Check all wire connections
- Verify resistor values with multimeter
- Check LED polarity (cathode is longest pin)
- Test with a simple blink sketch

Sound not detected:
- Check sensor is connected to Pin 3
- Verify sensor has 5V power
- Try clapping louder near sensor
- Check for loose connections

Arduino not found:
- Check USB cable
- Try different USB port
- Reinstall USB driver
- Check Device Manager for unknown devices

## Optional: Serial Monitor

To view debug output:
1. Connect Arduino to computer via USB
2. Open Serial Monitor (Ctrl+Shift+M)
3. Set baud rate to 9600
4. You should see "SOUNDLITE Initialized"
5. Clap and you should see "Sound Detected! LED: [color]"
