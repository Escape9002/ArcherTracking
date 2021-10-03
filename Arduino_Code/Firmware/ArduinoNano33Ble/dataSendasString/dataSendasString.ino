
#include <Arduino_LSM9DS1.h>
#include <ArduinoBLE.h>

BLEService gyroService("0000480f-0000-1000-8000-00805f9b34fb");
BLEStringCharacteristic LevelChar("00002a11-0000-1000-8000-00805f9b34fb", BLERead | BLENotify, 40);

void setup() {
  Serial.begin(9600);
  if (!IMU.begin()) { //LSM9DS1 begin
    Serial.println("LSM9DS1 error!");
    while (1);
  }
  if (!BLE.begin()) {
    Serial.println("BLE error!");
    while (1);
  }
  // set advertised local name and service UUID:
  BLE.setAdvertisedService(gyroService); // add the service UUID
  gyroService.addCharacteristic(LevelChar);
  BLE.addService(gyroService);
  // start advertising
  BLE.setLocalName("gyro");
  // start advertising
  BLE.advertise();
}

float ac_x, ac_y, ac_z;
float gy_x, gy_y, gy_z;
float ma_x, ma_y, ma_z;
String Level_String;

void updategyroLevel() {
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ac_x, ac_y, ac_z);
    Level_String="ST, "+String(ac_x,3)+", "+String(ac_y,3)+", "+String(ac_z,3);
  }
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gy_x, gy_y, gy_z);
    Level_String+=", "+String(gy_x,3)+", "+String(gy_y,3)+", "+String(gy_z,3);
  }
  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(ma_x, ma_y, ma_z);
    Level_String+=", "+String(ma_x,3)+", "+String(ma_y,3)+", "+String(ma_z,3)+", EN";
  }
  LevelChar.writeValue(Level_String);
//  Serial.println(Level_String);
}

void loop() {
  // listen for BLE peripherals to connect:
