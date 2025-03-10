#include <Wire.h>
#include <MPU6050.h>
#define OUTPUT_READABLE_YAWPITCHROLL

// Define the analog pins for the FSRs
const int fsrPin1 = A0;
const int fsrPin2 = A1;
const int fsrPin3 = A2;
const int fsrPin4 = A3;

// Define the PWM pins for the LEDs
const int rightGreenLED = 3;
const int leftGreenLED = 5;
const int leftRedLED = 6;
const int rightRedLED = 9;

const int buzzer = 10;

// Initialize the MPU6050 object
MPU6050 mpu;

const int calibration = 0;

void setup() {
  // Start serial communication at 115200 baud rate
  Serial.begin(115200);
  Serial.flush();

  // Initialize I2C communication for MPU6050
  Wire.begin();
  mpu.initialize();

  // Set the offsets obtained from the calibration process
  mpu.setXAccelOffset(0);
  mpu.setYAccelOffset(0);
  mpu.setZAccelOffset(0);
  mpu.setXGyroOffset(0);
  mpu.setYGyroOffset(0);
  mpu.setZGyroOffset(0);

  if (calibration == 1) {
    mpu.CalibrateAccel(6);  // Calibration Time: generate offsets and calibrate our MPU6050
    mpu.CalibrateGyro(6);
    Serial.println("These are the Active offsets: ");
    mpu.PrintActiveOffsets();
  }

  // Check if MPU6050 is connected
  if (!mpu.testConnection()) {
    while (1) {
      Serial.println("ERROR");
    }
  }

  pinMode(fsrPin1, INPUT);
  pinMode(fsrPin2, INPUT);
  pinMode(fsrPin3, INPUT);
  pinMode(fsrPin4, INPUT);

  pinMode(rightGreenLED, OUTPUT);
  pinMode(leftGreenLED, OUTPUT);
  pinMode(leftRedLED, OUTPUT);
  pinMode(rightRedLED, OUTPUT);
  
  pinMode(buzzer, OUTPUT);
}

int leftRedValue = 255;
int leftGreenValue = 50;
int rightRedValue = 255;
int rightGreenValue = 50;

void parseInput(String input) {
  int commaIndex = 0;
  leftRedValue = input.substring(0, input.indexOf(',')).toInt();
  commaIndex = input.indexOf(',') + 1;
  leftGreenValue = input.substring(commaIndex, input.indexOf(',', commaIndex)).toInt();
  commaIndex = input.indexOf(',', commaIndex) + 1;
  rightRedValue = input.substring(commaIndex, input.indexOf(',', commaIndex)).toInt();
  commaIndex = input.indexOf(',', commaIndex) + 1;
  rightGreenValue = input.substring(commaIndex).toInt();
}

String input = "";

void loop() {
  while (Serial.available() > 0) {
    char val = (char) Serial.read();
    if (val != "\n") {
      input += val;
    } else {
      parseInput(input);
      input = "";
    }
  }

  // Read analog values from each FSR pin
  int fsrValue1 = analogRead(fsrPin1);
  int fsrValue2 = analogRead(fsrPin2);
  int fsrValue3 = analogRead(fsrPin3);
  int fsrValue4 = analogRead(fsrPin4);

  // Set LED brightness based on mapped FSR values
  analogWrite(leftRedLED, 255 - leftRedValue);
  analogWrite(leftGreenLED, 255 - leftGreenValue);
  analogWrite(rightRedLED, 255 - rightRedValue);
  analogWrite(rightGreenLED, 255 - rightGreenValue);

  // Get gyroscope and accelerometer data from MPU6050
  int16_t ax_raw, ay_raw, az_raw, gx_raw, gy_raw, gz_raw;
  mpu.getMotion6(&ax_raw, &ay_raw, &az_raw, &gx_raw, &gy_raw, &gz_raw);

  // Convert accelerometer raw data to m/s² (assuming a ±2g range with 16384 LSB/g)
  float ax = ax_raw / 16384.0 * 9.8;
  float ay = ay_raw / 16384.0 * 9.8;
  float az = az_raw / 16384.0 * 9.8;

  // Convert gyroscope raw data to degrees per second (assuming ±250°/s range)
  float gx = gx_raw / 131.0;
  float gy = gy_raw / 131.0;
  float gz = gz_raw / 131.0;

  // Send data to Processing as a comma-separated string
  Serial.print(String(fsrValue1) + ',' + String(fsrValue2) + ',' + String(fsrValue3) + ',' + String(fsrValue4) + ',' + String(gx) + ',' + String(gy) + ',' + String(gz) + '\n');
  
  analogWrite(leftRedLED, 255 - 255);
  analogWrite(leftGreenLED, 255 - 0);
  analogWrite(rightRedLED, 255 - 255);
  analogWrite(rightGreenLED, 255 - 0);

  delay(5000);

  for (int i = 0; i < 10; ++i) {
    analogWrite(leftRedLED, 255 - leftRedValue);
    analogWrite(leftGreenLED, 255 - leftGreenValue);
    analogWrite(rightRedLED, 255 - rightRedValue);
    analogWrite(rightGreenLED, 255 - rightGreenValue);
    delay(500);
    analogWrite(leftRedLED, 255);
    analogWrite(leftGreenLED, 255);
    analogWrite(rightRedLED, 255);
    analogWrite(rightGreenLED, 255);
    delay(500);
  }

  delay(2500);
  
  analogWrite(leftRedLED, 255 - 10);
  analogWrite(leftGreenLED, 255);
  analogWrite(rightRedLED, 255 - 10);
  analogWrite(rightGreenLED, 255);

  delay(2500);
  

  for (int i = 0; i < 10; ++i) {
    analogWrite(leftRedLED, 255 - leftRedValue);
    analogWrite(leftGreenLED, 255 - leftGreenValue);
    analogWrite(rightRedLED, 255 - rightRedValue);
    analogWrite(rightGreenLED, 255 - rightGreenValue);
    delay(500);
    analogWrite(leftRedLED, 255 - 10);
    analogWrite(leftGreenLED, 255);
    analogWrite(rightRedLED, 255 - 10);
    analogWrite(rightGreenLED, 255);
    delay(500);
  }

  delay(2500);

  analogWrite(leftRedLED, 255);
  analogWrite(leftGreenLED, 255);
  analogWrite(rightRedLED, 255);
  analogWrite(rightGreenLED, 255);

  delay(2500);

  for (int i = 0; i < 10; ++i) {
    analogWrite(leftRedLED, 255 - leftRedValue);
    analogWrite(leftGreenLED, 255 - leftGreenValue);
    delay(500);
    analogWrite(leftRedLED, 255);
    analogWrite(leftGreenLED, 255);
    delay(500);
  }

  // Add a short delay between readings
  delay(15);
}
