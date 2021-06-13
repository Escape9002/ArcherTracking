//---------------------------------------------------MPU9250
#include "MPU9250.h"
// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire, 0x68);
int status;

float ac_x, ac_y, ac_z;

float gy_x, gy_y, gy_z;

float ma_x, ma_y, ma_z;

String Level_String = "default_Value";

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while (1) {}
  }
}
//-------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------LOOP()
void loop() {

  getallData();
 
    updategyroLevel();
  sendData();
}

void getallData() {
  IMU.readSensor();

  ac_x = IMU.getAccelX_mss();
  ac_y = IMU.getAccelY_mss();
  ac_z = IMU.getAccelZ_mss();

  gy_x = IMU.getGyroX_rads();
  gy_y = IMU.getGyroY_rads();
  gy_z = IMU.getGyroZ_rads();

  ma_x = IMU.getMagX_uT();
  ma_y = IMU.getMagY_uT();
  ma_z = IMU.getMagZ_uT();
}

void updategyroLevel() {
  //Arduino Nano 33 BLE crashs at this point
  Level_String = String(ac_x) + "/" + String(ac_y) + "/" + String(ac_z) + "/" + String(gy_x) + "/" + String(gy_y) + "/" + String(gy_z) + "/" + String(ma_x) + "/" + String(ma_y) + "/" + String(ma_z);
}

void sendData() {
  Serial.println(Level_String);
}

void getSize(){
  Serial.println(Level_String.length());
}
