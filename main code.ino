#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <U8g2lib.h>
#include <BleKeyboard.h>

// --- Hardware Setup ---
Adafruit_MPU6050 mpu;
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);
BleKeyboard bleKeyboard("Gesture-Glove");

// --- Calibration Ranges (Adjust these based on your sensor!) ---
float xMin = -3.0, xMax = 3.0;
float yMin = -6.0, yMax = 6.0;
float zMin = 7.0, zMax = 11.0;

// --- Key States ---
bool wPressed = false;
bool sPressed = false;
bool aPressed = false;
bool dPressed = false;
bool shiftPressed = false;

void releaseAll() {
  if (wPressed) { bleKeyboard.release('w'); wPressed = false; }
  if (sPressed) { bleKeyboard.release('s'); sPressed = false; }
  if (aPressed) { bleKeyboard.release('a'); aPressed = false; }
  if (dPressed) { bleKeyboard.release('d'); dPressed = false; }
  if (shiftPressed) { bleKeyboard.release(KEY_LEFT_SHIFT); shiftPressed = false; }
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  u8g2.begin();

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("MPU6050 not found!");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);

  bleKeyboard.begin();
  Serial.println("Gesture Glove Initialized");
  delay(1000);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float x = a.acceleration.x;
  float y = a.acceleration.y;
  float z = a.acceleration.z;
  String movement = "None";

  if (bleKeyboard.isConnected()) {
    releaseAll();  // Reset all keys every frame

    // --- Gesture Detection ---
    // Right movement (tilt right)
    if (x < xMin + 1) {
      bleKeyboard.press('d');
      dPressed = true;
      movement = "Right";
    } 
    // Left movement (tilt left)
    else if (x > xMax - 1) {
      bleKeyboard.press('a');
      aPressed = true;
      movement = "Left";
    } 
    // Forward movement (tilt forward)
    else if (y < yMin + 2) {
      bleKeyboard.press('w');
      wPressed = true;
      movement = "Forward";
    } 
    // Backward movement (tilt back)
    else if (y > yMax - 2) {
      bleKeyboard.press('s');
      sPressed = true;
      movement = "Back";
    }

    // --- Run Mode (press Shift when tilted far back) ---
    if (y > yMax) {
      bleKeyboard.press(KEY_LEFT_SHIFT);
      shiftPressed = true;
      movement = "Run";
    }
  } else {
    movement = "BLE N/C";  // BLE Not Connected
  }

  // --- Display Data on OLED ---
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tr);

  u8g2.setCursor(0, 10);
  u8g2.print("X: "); u8g2.print(x, 2);
  
  u8g2.setCursor(0, 20);
  u8g2.print("Y: "); u8g2.print(y, 2);
  
  u8g2.setCursor(0, 30);
  u8g2.print("Z: "); u8g2.print(z, 2);

  u8g2.setCursor(0, 50);
  u8g2.print("Move: "); u8g2.print(movement);

  u8g2.sendBuffer();

  delay(100); // Control sensitivity (lower = more responsive)