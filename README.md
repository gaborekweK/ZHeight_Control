# ZHeight_Control - Automated Viscometer Z-Height Calibration System

A precision Z-height control system for automated viscometer calibration, ensuring accurate spindle positioning for reliable viscosity measurements.

## Problem Statement

In automated viscometry, the **gap between the spindle tip and sample plate bottom** is critical for accurate measurements. This gap must be precisely **13 micrometers** - even tiny variations cause significant errors in viscosity readings due to the sensitive fluid dynamics involved.

Traditional manual calibration is:
- **Time-consuming** and requires operator skill
- **Inconsistent** between different operators  
- **Prone to human error** in gap estimation
- **Not suitable** for automated/unattended operation

## Solution

This system uses **LED color detection** to achieve automated, repeatable Z-height calibration:

1. **Viscometer LED indicator** turns green when optimal measurement conditions are detected
2. **ESP8266 color sensor** monitors the LED continuously during Z-axis descent  
3. **Immediate motor halt** when green is detected, locking in the precise 13μm gap
4. **Microsecond-precision timing** ensures minimal overshoot

## Project Structure

```
ZHeight_Control/
├── esp8266_color_sensor/           # ESP8266 with TCS34725 - Production system
│   └── esp8266_color_sensor.ino    # Detects green LED and sends TOUCH signal
├── color_sensor/                   # ESP8266 calibration utility  
│   └── color_sensor.ino            # ROYGBIV color calibration tool
├── arduino_uno_stepper/            # Arduino Uno with CNC Shield V3
│   └── arduino_uno_stepper.ino     # Controls Z-axis stepper motor descent
├── python/                         # Python coordinator script
│   └── main.py                     # Orchestrates ESP and Uno communication
└── debug_test.py                   # Individual device testing utility
```

## System Components

### Hardware
- **ESP8266**: Monitors TCS34725 color sensor pointed at viscometer LED
- **Arduino Uno + CNC Shield V3**: Controls Z-axis stepper motor descent
- **TCS34725 Color Sensor**: High-precision RGB detection (2.4ms integration time)
- **Stepper Motor**: Drives spindle Z-axis with microstepping precision

### Software Architecture
1. **Python Coordinator**: Manages overall calibration sequence
2. **ESP8266 Sensor**: Real-time LED color monitoring with debounced detection
3. **Arduino Controller**: Precise stepper motor control with immediate stop capability

## Calibration Workflow

1. **System Initialization**: Python coordinator establishes communication with both devices
2. **Descent Start**: Arduino begins controlled Z-axis descent toward sample
3. **LED Monitoring**: ESP8266 continuously monitors viscometer LED color  
4. **Green Detection**: When LED indicates optimal gap (green), ESP sends "TOUCH" signal
5. **Immediate Stop**: Arduino halts stepper motor with microsecond response time
6. **Position Lock**: Current Z-position becomes the calibrated reference point

## Usage

### Initial Setup
1. **Hardware**: Connect TCS34725 to ESP8266 via I2C (SDA→pin 5, SCL→pin 4)
2. **Arduino**: Upload `arduino_uno_stepper.ino` to Uno with CNC Shield
3. **ESP8266**: Upload `esp8266_color_sensor.ino` to ESP8266  
4. **Python**: Update COM ports in `python/main.py`

### Color Calibration (if needed)
1. Upload `color_sensor/color_sensor.ino` to ESP8266
2. Point sensor at viscometer LED in various states
3. Note RGBC values for green condition
4. Adjust thresholds in `esp8266_color_sensor.ino` if needed

### Production Calibration
```bash
python python/main.py
```

## Key Features

- **⚡ Fast Response**: 2.4ms color sensor integration + immediate motor stop
- **🎯 High Precision**: Maintains 13μm gap specification consistently  
- **🔄 Repeatable**: Eliminates human variability in calibration
- **📊 Automated**: Suitable for unattended viscometer operation
- **🛡️ Robust**: Debounced detection prevents false triggers from LED flicker

## Timing Performance

- **Color Detection**: ~3ms per sample with 3/5 confirmation
- **Motor Response**: Immediate halt via enable pin control
- **Total System Latency**: <10ms from green detection to complete stop
- **Positioning Accuracy**: Sub-micrometer precision with microstepping

## Applications

This system is designed specifically for automated viscometer calibration but the precision Z-height control approach could be adapted for:
- Probe positioning in analytical instruments
- Surface contact detection in metrology  
- Automated sample preparation systems
- Any application requiring precise distance/gap control