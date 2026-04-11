#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>               // I2C communication
#include <LiquidCrystal.h>      // LCD display
#include <SD.h>                 // SD card logging
#include <SPI.h>                // SPI communication (SD card)

// MPU6050 I2C address
const int MPU_addr = 0x68;

// variables
int16_t accX, accY, accZ;
int16_t gyroX, gyroY, gyroZ;

int stepCount = 0;              // step counter
float previousAccZ = 0;         // Z acceleration
bool stepDetected = false;      // bool to detect steps

// pin assignments
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// SD card CS pin
const int chipSelect = 10;

File dataFile;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Initializing...");

  // initialize MPU6050
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  byte mpuStatus = Wire.endTransmission(true);

  if (mpuStatus != 0) {
    lcd.setCursor(0, 1);
    lcd.print("MPU FAIL: ");
    lcd.print(mpuStatus);
    Serial.print("MPU6050 not responding. Error code: ");
    Serial.println(mpuStatus);
    while (1); // Halt if MPU fails
  }

  // initialize SD card
  if (!SD.begin(chipSelect)) {
    lcd.setCursor(0, 1);
    lcd.print("SD Init Fail");
    Serial.println("SD card initialization failed!");
    while (1); // Halt if SD fails
  }

  dataFile = SD.open("data.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println("accX,accY,accZ,gyroX,gyroY,gyroZ");
    dataFile.close();
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Steps: ");
  lcd.setCursor(0, 1);
  lcd.print(stepCount);
  Serial.println("MPU and SD ready");
}

void loop() {
  // data from MPU6050
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);

  int bytesReceived = Wire.requestFrom(MPU_addr, 14, true);
  if (bytesReceived < 14) {
    lcd.setCursor(0, 1);
    lcd.print("MPU Read Err  ");
    Serial.print("Error: received ");
    Serial.print(bytesReceived);
    Serial.println(" bytes");
    delay(500);
    return;
  }

  // sensor data
  accX = Wire.read() << 8 | Wire.read();
  accY = Wire.read() << 8 | Wire.read();
  accZ = Wire.read() << 8 | Wire.read();
  gyroX = Wire.read() << 8 | Wire.read();
  gyroY = Wire.read() << 8 | Wire.read();
  gyroZ = Wire.read() << 8 | Wire.read();

  // accZ to g-units
  float accZg = accZ / 16384.0;
  float threshold = 0.10;        // Raised threshold
  float deadzone = 0.05;         // Increased deadzone

  // output to Serial
  Serial.print("accZg: ");
  Serial.print(accZg, 4);
  Serial.print(" | Steps: ");
  Serial.println(stepCount);


  if (!stepDetected && (accZg - previousAccZ > threshold) && abs(accZg - previousAccZ) > deadzone) {
    stepCount++;
    stepDetected = true;
  }

  // =
  if (stepDetected && (accZg < previousAccZ)) {
    stepDetected = false;
  }

  previousAccZ = accZg;

  // update LCD
  lcd.setCursor(0, 0);
  lcd.print("Steps: ");
  lcd.setCursor(0, 1);
  lcd.print("          ");  // Clear line
  lcd.setCursor(0, 1);
  lcd.print(stepCount);

  // write to SD card 
  dataFile = SD.open("data.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.print(accX); dataFile.print(",");
    dataFile.print(accY); dataFile.print(",");
    dataFile.print(accZ); dataFile.print(",");
    dataFile.print(gyroX); dataFile.print(",");
    dataFile.print(gyroY); dataFile.print(",");
    dataFile.println(gyroZ);
    dataFile.close();
  }

  delay(100);  
}