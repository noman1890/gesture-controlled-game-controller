# gesture-controlled-game-controller
A gesture-controlled game controller using ESP32, MPU6050, and OLED display to play games with hand movements.

COMPONENTS
ESP32 Development Board
- MPU6050 (Accelerometer + Gyroscope)
- OLED Display 0.96"
- Jumper Wires
- breadboard(coustom if possible)

  CONNECTION
  MPU6050                          ESP32
  vcc                              3.3v
  gnd                              gnd
  sda                              21
  scl                              22
  AD0                             no connection
  INT                             no connection

  OLED                           ESP32
  vcc                              3.3v
  gnd                              gnd
  sda                              21
  scl                              22

  Both MPU6050 and OLED share the I2C bus — that’s fine. ESP32 I2C allows multiple devices.

  LIBRARY  USED
  Adafruit_MPU6050.h- https://github.com/adafruit/Adafruit_MPU6050
  Adafruit_Sensor.h- https://github.com/adafruit/Adafruit_Sensor
  U8g2lib.h - https://github.com/olikraus/U8g2_Arduino
  BleKeyboard.h - https://github.com/T-vK/ESP32-BLE-Keyboard

  Installation Instructions:**  
1. Open Arduino IDE → Sketch → Include Library → Manage Libraries  
2. Search for each library and install  
3. For GitHub libraries, download ZIP → Sketch → Include Library → Add .ZIP Library  


  Both MPU6050 and OLED require 3.3V, not 5V. ESP32 can supply 3.3V from its onboard pin
  Make sure there is a common gnd
  code is in the maincode.io
  Working / How It Works

1. MPU6050 detects **hand movements** (X, Y, Z acceleration).  
2. ESP32 processes the sensor data and decides which keys to press:  
   - Tilt forward → `W`  
   - Tilt backward → `S`  
   - Tilt left → `A`  
   - Tilt right → `D`  
   - Tilt far back → `Shift` (Run mode)  
3. OLED displays **real-time X, Y, Z values** and the detected movement.  
4. BLE Keyboard sends keystrokes wirelessly to a PC/game.

Challenges I Faced

OLED not turning on initially → check wiring, power, and initialization
Hand shakes can reset or affect the timing of the data
Calibration took a lot of time to get accurate gesture detection
False movements may occur occasionally → this is normal due to sensor noise
Some OLED displays may require a different library.
If your display does not work with U8g2lib, try using Adafruit_SSD1306 together with Adafruit_GFX libraries instead.
Always check your OLED model and the library examples to confirm proper wiring and initialization.




  This project is licensed under the **MIT License** - see the LICENSE file for details
  
  
