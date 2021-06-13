/*
   Written: Antonio Rehwinkel [2020]
   Usage: Programm for JuFo Project
        [Movement tracker for archers]

   Used Devices:
    -Arduino Nano 33 BLE
    -MPU9250

   Device Name: MPU9250

   Service Uuid: c54beb4a-40c7-11eb-b378-0242ac130002

   charactersitics Uuid [Int]:
            accelX : d6b78de4-40c7-11eb-b378-0242ac130002
            accelY : 383e9b64-42c8-11eb-b378-0242ac130002
            accelZ : 47f065f6-42c8-11eb-b378-0242ac130002

   charactersitics Uuid [String]:
            accelX : -
            accelY : -
            accelZ : -
   Note:
   no characteristics for:
   -gyroscope
   -magnetic sensor
   -temperatur

   only sends byte(positive values)

  current Errors:
   none

  compatible with BLETestVol5.apk
  Datenübertragung:
  int = Auf den Arduino Due- und SAMD-basierten Boards (wie MKR1000 und Zero) speichert ein int einen 32-Bit-Wert (4 Byte).
  BLE: A characteristic value can be up to 512 bytes long.
*/


//-------------------------------------------------------------------------------------BIBLIOTHEK {
//---------------------------------------------------BLE
#include <ArduinoBLE.h>
//---------------------------------------------------MPU9250
#include "MPU9250.h"
// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire, 0x68);
int status;
//-------------------------------------------------------------------------------------BIBLIOTHEK }
//-------------------------------------------------------------------------------------VARIABLEN {
//---------------------------------------------------Acclereator
/*
  volatile int accelXInt = 1;
  volatile int accelYInt = 1;
  volatile int accelZInt = 1;
*/
volatile short accelXshort = 1;
volatile short accelYshort = 1;
volatile short accelZshort = 1;
/*
  String accelXStr = " ";
  String accelYStr = " ";
  String accelZStr = " ";

  volatile byte accelXbyte = 1;
  volatile byte accelYbyte = 1;
  volatile byte accelZbyte = 1;
*/
  //---------------------------------------------------Gyroscope
  /*
  volatile int gyroXInt = 1;
  volatile int gyroYInt = 1;
  volatile int gyroZInt = 1;

  volatile short gyroXshort = 1;
  volatile short gyroYshort = 1;
  volatile short gyroZshort = 1;

  String gyroXStr = " ";
  String gyroYStr = " ";
  String gyroZStr = " ";

  volatile byte gyroXbyte = 1;
  volatile byte gyroYbyte = 1;
  volatile byte gyroZbyte = 1;
  */
  //---------------------------------------------------magnetic sensor
  /*
  volatile int magnetXInt = 1;
  volatile int magnetYInt = 1;
  volatile int magnetZInt = 1;

  volatile short magnetXshort = 1;
  volatile short magnetYshort = 1;
  volatile short magnetZshort = 1;

  String magnetXStr = " ";
  String magnetYStr = " ";
  String magnetZStr = " ";

  volatile byte magnetXbyte = 1;
  volatile byte magnetYbyte = 1;
  volatile byte magnetZbyte = 1;
  */
  //---------------------------------------------------Temperatur
  /*
  volatile int tempInt = 1;

  volatile short tempshort = 1;

  String tempStr = " ";

  volatile byte tempbyte = 1;
*/
//--------------------------------------------------- TImer
long newTime = 1;
long oldTime = 1;
//-------------------------------------------------------------------------------------VARIABLEN }
//-------------------------------------------------------------------------------------BLE_SETUP {
BLEService SendingService("c54beb4a-40c7-11eb-b378-0242ac130002");
BLEShortCharacteristic accelXChar("d6b78de4-40c7-11eb-b378-0242ac130002", BLERead | BLENotify);
//BLEShortCharacteristic accelYChar("383e9b64-42c8-11eb-b378-0242ac130002", BLERead | BLENotify);
//BLEShortCharacteristic accelZChar("47f065f6-42c8-11eb-b378-0242ac130002", BLERead | BLENotify);
//-------------------------------------------------------------------------------------BLE_SETUP }
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

  SendingService.addCharacteristic(accelXChar);
  //SendingService.addCharacteristic(accelYChar);
  //SendingService.addCharacteristic(accelZChar);

  BLE.addService(SendingService);

  BLE.advertise();

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
      //debug_accel();
      send_short();
      transRate();
    }
  }
  disconnectedLight();
  Serial.print("Disconnected from central: ");
  Serial.println(central.address());
}

//-------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------PROGRAMMS
//----------------------------------------------------------------------------------------- SENDING
//--------------------------------------------------- Send short
void send_short() {
  //-----------------------------------accel
  accelXChar.writeValue(accelXshort);
  //accelYChar.writeValue(accelYshort);
  //accelZChar.writeValue(accelZshort);
  //-----------------------------------gyro

  //-----------------------------------magnet

  //-----------------------------------temp

}

  //--------------------------------------------------- Send Bytes
  /*
   void send_byte() {
  //------------------------------------accel
  accelXChar.writeValue(accelXbyte);
  accelYChar.writeValue(accelYbyte);
  accelZChar.writeValue(accelZbyte);

  //-----------------------------------gyro

  //-----------------------------------magnet

  //-----------------------------------temp
  }
*/
  //--------------------------------------------------- Send String (in ASCII)        !!!Charactersitics needs to be declared as String!!!
  /*
  void send_String() {
  //---------------------------- convert to String
  accelXStr = String(accelXInt);
  accelYStr = String(accelYInt);
  accelZStr = String(accelZInt);

  //-----------------------------------accel
  accelXChar.writeValue(accelXStr);
  accelYChar.writeValue(accelYStr);
  accelZChar.writeValue(accelZStr);

  //-----------------------------------gyro

  //-----------------------------------magnet

  //-----------------------------------temp

  }
*/
//----------------------------------------------------------------------------------------- READING
//--------------------------------------------------- Accelerator READ
void read_accel() {
  IMU.readSensor();
  /*
    //--------------------------------byte
    accelXbyte = IMU.getAccelX_mss();
    accelYbyte = IMU.getAccelY_mss();
    accelZbyte = IMU.getAccelZ_mss();
  */
  /*
    //--------------------------------String (is converted in send_string() )
    accelXInt = IMU.getAccelX_mss();
    accelYInt = IMU.getAccelY_mss();
    accelZInt = IMU.getAccelZ_mss();
  */
  //--------------------------------short
  accelXshort = IMU.getAccelX_mss();
  accelYshort = IMU.getAccelY_mss();
  accelZshort = IMU.getAccelZ_mss();

}

//--------------------------------------------------- Gyroscope READ
/*
  void read_gyro() {
  IMU.readSensor();

  //--------------------------------byte
  gyroXbyte = IMU.getGyroX_rads();
  gyroYbyte = IMU.getGyroY_rads();
  gyroZbyte = IMU.getGyroZ_rads();


  //--------------------------------String (is converted in send_string() )
  gyroXInt = IMU.getGyroX_rads();
  gyroYInt = IMU.getGyroY_rads();
  gyroZInt = IMU.getGyroZ_rads();

  //--------------------------------short
  gyroXshort = IMU.getGyroX_rads();
  gyroYshort = IMU.getGyroY_rads();
  gyroZshort = IMU.getGyroZ_rads();

  }
*/
//--------------------------------------------------- Magentic READ
/*
  void read_magnetic() {
  IMU.readSensor();

  //--------------------------------byte
  magnetXbyte = IMU.getMagX_uT();
  magnetYbyte = IMU.getMagY_uT();
  magnetZbyte = IMU.getMagZ_uT();


  //--------------------------------String (is converted in send_string() )
  magnetXInt = IMU.getMagX_uT();
  magnetYInt = IMU.getMagY_uT();
  magnetZInt = IMU.getMagZ_uT();

  //--------------------------------short
  magnetXshort = IMU.getMagX_uT();
  magnetYshort = IMU.getMagY_uT();
  magnetZshort = IMU.getMagZ_uT();

  }
*/
//--------------------------------------------------- Temperatur READ
/*
  void read_tempInt() {
  IMU.readSensor();

  //--------------------------------byte
  tempbyte = IMU.getTemperature_C();
  //--------------------------------String (is converted in send_string() )
  tempInt = IMU.getTemperature_C();
  //--------------------------------short
  tempshort = IMU.getTemperature_C();

  }
*/
//----------------------------------------------------------------------------------------- LED-CONNECTION-INDICATOR
//--------------------------------------------------- LED ( Connection status)
void connectedLight() {
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, HIGH);
}
void disconnectedLight() {
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, LOW);
}
//--------------------------------------------------- Transmitting Rate
void transRate(){
  newTime = millis();
  Serial.println(newTime - oldTime);
  oldTime = newTime;
}
//----------------------------------------------------------------------------------------- DEBUGGING
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
  //--------------------------------------------------- temperatur sensor
  void debug_temp() {
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
