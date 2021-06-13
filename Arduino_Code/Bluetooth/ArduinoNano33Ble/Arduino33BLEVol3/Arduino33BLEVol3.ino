/*
   Written: Antonio Rehwinkel [2020]
   Usage: Programm for JuFo Project
        [Movement tracker for archers]

   Device Name: MPU9250

   Service Uuid: c54beb4a-40c7-11eb-b378-0242ac130002

   charactersitics Uuid [Int]:
            accelX : d6b78de4-40c7-11eb-b378-0242ac130002
            accelY : 2102
            accelZ : 2103

   charactersitics Uuid [String]:
            accelX : -
            accelY : -
            accelZ : -
   Note:
   no characteristics for:
   -gyroscope
   -magnetic sensor
   -temperatur

current Errors:
   -isAdvertising() returns 0 --> no data captured by app
      "nRF Connect" shows Service and characteristic. Also data is captured.
      Own app doesn't show any Data + says that it's not advertising.

sending works with bytes
*/


//-------------------------------------------------------------------------------------BIBLIOTHEK
//---------------------------------------------------BLE
#include <ArduinoBLE.h>
//---------------------------------------------------MPU9250
#include "MPU9250.h"
// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire, 0x68);
int status;
//-------------------------------------------------------------------------------------BIBLIOTHEK
//-------------------------------------------------------------------------------------VARIABLEN
//---------------------------------------------------Acclereator
volatile int accelXInt = 1;
volatile int accelYInt = 1;
volatile int accelZInt = 1;

String accelXStr = " ";
String accelYStr = " ";
String accelZStr = " ";

byte accelXbyte = 1;
byte accelYbyte = 1;
byte accelZbyte = 1;
/*
  //---------------------------------------------------Gyroscope
  volatile int gyroXInt = 1;
  volatile int gyroYInt = 1;
  volatile int gyroZInt = 1;

  String gyroXStr = " ";
  String gyroYStr = " ";
  String gyroZStr = " ";
  //---------------------------------------------------magnetic sensor
  volatile int magnetXInt = 1;
  volatile int magnetYInt = 1;
  volatile int magnetZInt = 1;

  String magnetXStr = " ";
  String magnetYStr = " ";
  String magnetZStr = " ";
  //---------------------------------------------------Temperatur
  volatile int tempInt = 1;
  String tempStr = " ";
*/
//-------------------------------------------------------------------------------------VARIABLEN
//-------------------------------------------------------------------------------------BLE_SETUP
BLEService SendingService("c54beb4a-40c7-11eb-b378-0242ac130002");
BLEByteCharacteristic accelXChar("d6b78de4-40c7-11eb-b378-0242ac130002", BLERead | BLENotify);
//BLEUnsignedIntCharacteristic accelYChar("2102", BLERead | BLENotify);
//BLEUnsignedIntCharacteristic accelZChar("2103", BLERead | BLENotify);
//-------------------------------------------------------------------------------------BLE_SETUP
//-------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------SETUP()
void setup() {
  Serial.begin(115200);
  while (!Serial)
//---------------------------------------------------MPU9250 Setup

    status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while (1) {}
  }

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

  accelXChar.writeValue(accelXInt);
  // accelYChar.writeValue(accelYInt);
  // accelZChar.writeValue(accelZInt);

  
  String address = BLE.address();
  Serial.print("Local address is: ");
  Serial.println(address);
  delay(1000);

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
      read_accel();
      debug_accel();
      send_String();

      isAdvertising();


    }
  }
  disconnectedLight();
  Serial.print("Disconnected from central: ");
  Serial.println(central.address());
}

//-------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------PROGRAMMS
//--------------------------------------------------- Send Integers
/*
void send_int() {

  accelXChar.writeValue(accelXInt);
  // accelYChar.writeValue(accelYInt);
  // accelZChar.writeValue(accelZInt);

}
*/
//--------------------------------------------------- Send String (in ASCII)        !!!Charactersitics needs to be declared as String!!!

  void send_String() {
  accelXStr = String(accelXInt);
  // accelYStr = String(accelYInt);
  //accelZStr = String(accelZInt);

  accelXChar.writeValue(accelXbyte);
  // accelYChar.writeValue(accelYStr);
  // accelZChar.writeValue(accelZStr);

  }

void send_byte(){
  accelXChar.writeValue(accelXbyte);
  // accelYChar.writeValue(accelYInt);
  // accelZChar.writeValue(accelZInt);
}
//--------------------------------------------------- Accelerator READ
void read_accel() {

  IMU.readSensor();
/*
  accelXInt = IMU.getAccelX_mss();
  accelYInt = IMU.getAccelY_mss();
  accelZInt = IMU.getAccelZ_mss();
*/
  accelXbyte = IMU.getAccelX_mss();
 // accelYInt = IMU.getAccelY_mss();
  //accelZInt = IMU.getAccelZ_mss();
}
/*
  //--------------------------------------------------- Gyroscope READ
  void read_gyro() {

  gyroXInt = IMU.getGyroX_rads();
  gyroYInt = IMU.getGyroY_rads();
  gyroZInt = IMU.getGyroZ_rads();

  }

  //--------------------------------------------------- Magentic READ
  void read_magnetic() {

  magnetXInt = IMU.getMagX_uT();
  magnetYInt = IMU.getMagY_uT();
  magnetZInt = IMU.getMagZ_uT();

  }
  //--------------------------------------------------- Temperatur READ
  void read_tempInt() {

  tempInt = IMU.getTemperature_C();

  }
*/
//--------------------------------------------------- LED ( Connection status)
void connectedLight() {
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, HIGH);
}
void disconnectedLight() {
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, LOW);
}
//------------------------------------------------------------------------------------------------------ Debugging
//--------------------------------------------------- accelerator
void debug_accel() {
  
    Serial.print("AccelX: ");
    Serial.print(IMU.getAccelX_mss(), 6);
    Serial.print("  ");
    Serial.print("AccelY: ");
    Serial.print(IMU.getAccelY_mss(), 6);
    Serial.print("  ");
    Serial.print("AccelZ: ");
    Serial.println(IMU.getAccelZ_mss(), 6);
  /*
  Serial.print("AccelX: ");
  Serial.print(accelXInt);
  Serial.print("  ");
  Serial.print("AccelY: ");
  Serial.print(accelYInt);
  Serial.print("  ");
  Serial.print("AccelZ: ");
  Serial.println(accelZInt);
  */
}
/*
  //--------------------------------------------------- gyroscope
  void debug_gyro() {

  Serial.print("GyroX: ");
  Serial.print(IMU.getGyroX_rads(), 6);
  Serial.print("  ");
  Serial.print("GyroY: ");
  Serial.print(IMU.getGyroY_rads(), 6);
  Serial.print("  ");
  Serial.print("GyroZ: ");
  Serial.println(IMU.getGyroZ_rads(), 6);
  }
  //--------------------------------------------------- magnetic sensor
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
  void debug_temp(){
  Serial.print("Temperature in C: ");
  Serial.println(IMU.getTemperature_C(), 6);
  Serial.println();
  }
*/
//--------------------------------------------------- is Arduino avertising() ?
void isAdvertising() {
  BLE.advertise();
  Serial.print("Advertising: ");
  Serial.println(BLE.advertise());
}
