/*
   Written: Antonio Rehwinkel [2020]
   Usage: Programm for JuFo Project
        [Movement tracker for archers]

   Used Devices:
    -Arduino Nano 33 BLE
    -MPU9250

   Device Name: MPU9250

   Service Uuid: c54beb4a-40c7-11eb-b378-0242ac130002

   charactersitics Uuid [String]:
   d6b78de4-40c7-11eb-b378-0242ac130002

   Note:
   - no need for multiple characteristics
   - only sends Strings(decoding by Receiver needed)

  current Errors:
   none

  compatible with BLETestVol6.apk

  Datenübertragung:

    "[Wert(accelX)]|[Wert(accelY)]|[Wert(accelZ)]|
     [Wert(gyroX)]|[Wert(gyroY)]|[Wert(gyroZ)]|
     [Wert(magnetX)]|[Wert(magnetY)]|[Wert(magnetZ)]
    "

    contained in [Lvel_String]
*/


//-------------------------------------------------------------------------------------BIBLIOTHEK
//---------------------------------------------------BLE
#include <ArduinoBLE.h>
//---------------------------------------------------MPU9250
#include "mpu9250.h"
// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
bfs::Mpu9250 imu(&Wire, 0x68);
//-------------------------------------------------------------------------------------BIBLIOTHEK
//-------------------------------------------------------------------------------------VARIABLEN
//---------------------------------------------------Acclereator
/*
  volatile short accelXInt = 1;
  volatile short accelYInt = 1;
  volatile short accelZInt = 1;

  String accelXStr = " ";
  String accelYStr = " ";
  String accelZStr = " ";

  byte accelXbyte = 1;
  byte accelYbyte = 1;
  byte accelZbyte = 1;
*/
float ac_x, ac_y, ac_z;

//---------------------------------------------------Gyroscope
/*
  volatile int gyroXInt = 1;
  volatile int gyroYInt = 1;
  volatile int gyroZInt = 1;

  String gyroXStr = " ";
  String gyroYStr = " ";
  String gyroZStr = " ";
*/
//float gy_x, gy_y, gy_z;

//---------------------------------------------------magnetic sensor
/*
  volatile int magnetXInt = 1;
  volatile int magnetYInt = 1;
  volatile int magnetZInt = 1;

  String magnetXStr = " ";
  String magnetYStr = " ";
  String magnetZStr = " ";
*/
//float ma_x, ma_y, ma_z;

//---------------------------------------------------Temperatur
/*
  volatile int tempInt = 1;
  String tempStr = " ";
*/
//---------------------------------------------------Place for all data
String Level_String;
int fixed_length = 100;
//-------------------------------------------------- Timer
long newTime = 0;
long oldTime = 0;
//-------------------------------------------------------------------------------------VARIABLEN
//-------------------------------------------------------------------------------------BLE_SETUP
BLEService SendingService("c54beb4a-40c7-11eb-b378-0242ac130002");
BLEStringCharacteristic accelXChar("d6b78de4-40c7-11eb-b378-0242ac130002", BLERead | BLENotify, fixed_length);
//-------------------------------------------------------------------------------------BLE_SETUP
//-------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------SETUP()
void setup() {
  Serial.begin(115200);
  //while (!Serial)

  //---------------------------------------------------MPU9250 Setup
  /* Serial to display data */
  Serial.begin(115200);
  // while (!Serial) {}
  /* Start the I2C bus */
  Wire.begin();
  Wire.setClock(400000);
  /* Initialize and configure IMU */
  if (!imu.Begin()) {
    Serial.println("Error initializing communication with IMU");
    while (1) {}
  }
  /* Set the sample rate divider */
  /*
    if (!imu.ConfigSrd(herz(1000))) {
    Serial.println("Error configured SRD");
    while (1) {}
    }
  */
  //---------------------------------------------------BLE Setup

  if (!BLE.begin()) {
    Serial.println("BLE failed to Initiate");
    delay(500);
    while (1);
  }


  BLE.setLocalName("MPU9250");
  BLE.setAdvertisedService(SendingService);

  //BLE.setAdvertisedServiceUuid("c54beb4a-40c7-11eb-b378-0242ac130002");

  SendingService.addCharacteristic(accelXChar);
  // SendingService.addCharacteristic(accelYChar);
  // SendingService.addCharacteristic(accelZChar);

  BLE.addService(SendingService);

  BLE.advertise();

  Serial.println("Bluetooth device is now active, waiting for connections...");


  //---------------------------------------------------Intern LED Setup

  pinMode(LED_BUILTIN, OUTPUT);    //B
  pinMode(LEDR, OUTPUT);           //R
  pinMode(LEDG, OUTPUT);           //G
}
//-------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------LOOP()
void loop() {

  BLEDevice central = BLE.central();

  if (central) {
    connectedLight();
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, HIGH);

    while (central.connected()) {
      if (getallData()) {
        send_String();
        //transRate();
      }



    }
  }
  disconnectedLight();
  Serial.print("Disconnected from central: ");
  Serial.println(central.address());
}

//-------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------PROGRAMMS
//--------------------------------------------------- Input Herz for the sample Rate to work with direct input
int herz(int i) {
  i = (1000 - i) / i;
  return i;
}
//--------------------------------------------------- reverse herz --> input sample divider, get herz
int HzToSrd(int i) {
  i = 1000 / (i + 1);
  return i;
}
//--------------------------------------------------- check all imu settings via Serial Monitor
void checkIMU() {

  unsigned long int acl = imu.accel_range();
  unsigned long int gyr = imu.gyro_range();
  unsigned long int fil = imu.dlpf_bandwidth();

  String valAcl = "";
  String valGyr = "";
  String valFil = "";

  switch (acl) {
    case 0x00:
      valAcl = "2G";
      break;

    case 0x08:
      valAcl = "4G";
      break;

    case 0x10:
      valAcl = "8G";
      break;

    case 0x18:
      valAcl = "16G";
      break;
  }

  switch (gyr) {
    case 0x00:
      valGyr = "250DPS";
      break;

    case 0x08:
      valGyr = "500DPS";
      break;

    case 0x10:
      valGyr = "1000DPS";
      break;

    case 0x18:
      valGyr = "2000DPS";
      break;
  }

  switch (fil) {
    case 0x01:
      valFil = "184 Hz";
      break;

    case 0x02:
      valFil = "92 Hz";
      break;

    case 0x03:
      valFil = "41 Hz";
      break;

    case 0x04:
      valFil = "20 Hz";
      break;

    case 0x05:
      valFil = "10 Hz";
      break;

    case 0x06:
      valFil = "5 Hz";
      break;
  }
  Serial.println("//////////////Data of MPU9250/////////////////");
  Serial.print("Accel Range: ");
  Serial.println(valAcl);
  Serial.print("Gyro Range: ");
  Serial.println(valGyr);
  Serial.print("Herz Running: ");
  Serial.println(HzToSrd(imu.srd()));
  Serial.print("Low Pass Filter: ");
  Serial.println(valFil);
  Serial.println("/////////////Data of MPU9250//////////////");
}
//--------------------------------------------------- Send Int
/*
  void send_Int() {
  accelXChar.writeValue(accelXInt);
  }*/
//--------------------------------------------------- Send String (in ASCII)        !!!Charactersitics needs to be declared as String!!!

void send_String() {
  Level_String = String(ac_x, 2) + "," + String(ac_y, 2) + "," + String(ac_z, 2)/* + "/" + String(gy_x,2) + "/" + String(gy_y,2) + "/" + String(gy_z,2) + "/" + String(ma_x,2) + "/" + String(ma_y,2) + "/" + String(ma_z,2)*/;
  accelXChar.writeValue(Level_String);
}

//--------------------------------------------------- Send Bytes
/*
  void send_byte() {
  accelXChar.writeValue(accelXbyte);
  //accelYChar.writeValue(accelYbyte);
  //accelZChar.writeValue(accelZbyte);
  }
*/

//--------------------------------------------------- Temperatur READ
/*
  void read_tempInt() {
  IMU.readSensor();

  tempInt = IMU.getTemperature_C();

  }
*/
//--------------------------------------------------- get all data except temperatur
bool getallData() {
  if (imu.Read()) {
    ac_x = imu.accel_x_mps2();
    ac_y = imu.accel_y_mps2();
    ac_z = imu.accel_z_mps2();
    /*
      gy_x = imu.gyro_x_radps();
      gy_y = imu.gyro_y_radps();
      gy_z = imu.gyro_z_radps();
    */
    /*
      ma_x = IMU.getMagX_uT();  //look these guys up, this is not the correct way
      ma_y = IMU.getMagY_uT();
      ma_z = IMU.getMagZ_uT();
    */
    return true;
  }
  else {
    return false;
  }


}
//--------------------------------------------------- LED ( Connection status)
void connectedLight() {
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, HIGH);
}
void disconnectedLight() {
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, LOW);
}
//--------------------------------------------------- Timer for transmitting Rate
void transRate() {
  newTime = millis();
  Serial.println(newTime - oldTime);
  oldTime = newTime;
}
//------------------------------------------------------------------------------------------------------ Debugging
//--------------------------------------------------- accelerator
/*
  void debug_accel() {

  Serial.print("AccelX: ");
  Serial.print(imu.accel_x_mps2(), 6);
  Serial.print("  ");
  Serial.print("AccelY: ");
  Serial.print(imu.accel_x_mps2(), 6);
  Serial.print("  ");
  Serial.print("AccelZ: ");
  Serial.println(imu.accel_x_mps2(), 6);

    Serial.print("AccelX: ");
    Serial.print(accelXInt);
    Serial.print("  ");
    Serial.print("AccelY: ");
    Serial.print(accelYInt);
    Serial.print("  ");
    Serial.print("AccelZ: ");
    Serial.println(accelZInt);

  }
*/
//--------------------------------------------------- gyroscope
/*
  void debug_gyro() {

  Serial.print("GyroX: ");
  Serial.print(imu.gyro_x_radps(), 6);
  Serial.print("  ");
  Serial.print("GyroY: ");
  Serial.print(imu.gyro_x_radps(), 6);
  Serial.print("  ");
  Serial.print("GyroZ: ");
  Serial.println(imu.gyro_x_radps(), 6);
  }
*/
//--------------------------------------------------- magnetic sensor
/* || look it up again, these functions aint right
  void debug_magnet() {
  Serial.print("MagX: ");
  Serial.print(IMU.getMagX_uT(), 6);
  Serial.print("  ");
  Serial.print("MagY: ");
  Serial.print(IMU.getMagY_uT(), 6);
  Serial.print("  ");
  Serial.print("MagZ: ");
  Serial.println(IMU.getMagZ_uT(), 6);
  }
*/
//--------------------------------------------------- temperatur
/* || look it up again, these functions aint right
  void debug_temp(){
  Serial.print("Temperature in C: ");
  Serial.println(IMU.getTemperature_C(), 6);
  Serial.println();
  }
*/
//-------------------------------------------------- Level_String
/*
  void debug_Level_String() {
  Serial.println(Level_String);

  Serial.print("sizeof: ");
  Serial.println(sizeof(Level_String));
  Serial.print("VALUELENGTH: ");
  Serial.println(accelXChar.valueLength());

  }
*/
