# ZHeight_Control

A precision Z-height control system using ESP8266 color detection and Arduino Uno stepper control.

## Project Structure

```
ZHeight_Control/
├── esp8266_color_sensor/           # ESP8266 with TCS34725 color sensor
│   └── esp8266_color_sensor.ino    # Detects green and sends TOUCH signal
├── arduino_uno_stepper/            # Arduino Uno with CNC Shield V3
│   └── arduino_uno_stepper.ino     # Controls Z-axis stepper motor
├── python/                         # Python coordinator script
│   └── main.py                     # Orchestrates ESP and Uno communication
├── move_Zstepper/                  # Legacy folder (old Uno code)
└── stop_motor_color/               # Legacy folder (old ESP code)
```

## How It Works

1. **Python script** (`python/main.py`) coordinates the system
2. **Arduino Uno** (`arduino_uno_stepper.ino`) controls Z-axis movement via CNC Shield
3. **ESP8266** (`esp8266_color_sensor.ino`) monitors TCS34725 color sensor for green detection
4. When green is detected, ESP sends "TOUCH" → Python stops Uno → precise halt achieved

## Hardware Setup

- **ESP8266**: Connected to TCS34725 color sensor (I2C on pins 4,5)
- **Arduino Uno**: Connected to CNC Shield V3 for stepper control
- **Communication**: Both devices connect to PC via USB serial

## Usage

1. Upload `esp8266_color_sensor.ino` to ESP8266
2. Upload `arduino_uno_stepper.ino` to Arduino Uno  
3. Update COM ports in `python/main.py`
4. Run: `python python/main.py`

## Serial Commands

**ESP8266:**
- `RST` - Reset color detection latch
- Sends `TOUCH` when green detected

**Arduino Uno:**
- `GO` - Start Z-axis movement
- `STOP` - Immediate halt
- `RST` - Reset and disable motor