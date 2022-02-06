/*
   Written: Antonio Rehwinkel [2020]
   Usage: Programm for JuFo Project
        [Movement tracker for archers]

   Used Devices:
    -Arduino Nano 33 BLE Sense

   Device Name: LSM9DS1

   Service Uuid: c54beb4a-40c7-11eb-b378-0242ac130002

   charactersitics Uuid [String]:
   accel:
   d6b78de4-40c7-11eb-b378-0242ac130002

  gyro:
  d6a507ce-5489-11ec-bf63-0242ac130002

  mag:
  11cf4eb8-86d0-11ec-a8a3-0242ac120002

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

double aXOFF = 0.00;
double aYOFF = 0.00;
double aZOFF = 0.00;
//-------------------------------------------------------------------------------------BIBLIOTHEK
//---------------------------------------------------BLE
#include <ArduinoBLE.h>
//---------------------------------------------------MPU9250
#include <Arduino_LSM9DS1.h>
//-------------------------------------------------------------------------------------BIBLIOTHEK
//-------------------------------------------------------------------------------------VARIABLEN
float ac_x, ac_y, ac_z; //Accelerometer
float gy_x, gy_y, gy_z; //Gyroscope
float ma_x, ma_y, ma_z; //Magnetometer
// volatile int tempInt = 1;  //Temperature Guy

String Level_String;  //Storage for BLE-Data
int fixed_length = 100;

long newTime = 0; //Wir sind für die Abschaffung von Atomuhren, wir sind für den Ausstieg aus der Zeit!
long oldTime = 0;
//-------------------------------------------------------------------------------------VARIABLEN
//-------------------------------------------------------------------------------------BLE_SETUP
BLEService SendingService("c54beb4a-40c7-11eb-b378-0242ac130002");
BLEStringCharacteristic accelXChar("d6b78de4-40c7-11eb-b378-0242ac130002", BLERead | BLENotify | BLEWriteWithoutResponse, fixed_length);
BLEStringCharacteristic gyroXChar("d6a507ce-5489-11ec-bf63-0242ac130002", BLERead | BLENotify | BLEWriteWithoutResponse, fixed_length);
BLEStringCharacteristic magXChar("11cf4eb8-86d0-11ec-a8a3-0242ac130002", BLERead | BLENotify | BLEWriteWithoutResponse, fixed_length);
//-------------------------------------------------------------------------------------BLE_SETUP
//-------------------------------------------------------------------------------------DATA FOR ML
const float accelerationThreshold = 26; // threshold of significant in G's || normal movement == up to 20-23 G (added up)
const int numSamples = 80;
int samplesRead = numSamples;
//-------------------------------------------------------------------------------------DATA FOR ML
//-------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------SETUP()
void setup() {
  Serial.begin(115200);
  //while (!Serial)

  //---------------------------------------------------LSM9DS1 Setup

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  //---------------------------------------------------BLE Setup

  if (!BLE.begin()) {
    Serial.println("BLE failed to Initiate");
    delay(500);
    while (1);
  }


  BLE.setLocalName("LSM9DS1");
  BLE.setAdvertisedService(SendingService);

  //BLE.setAdvertisedServiceUuid("c54beb4a-40c7-11eb-b378-0242ac130002");

  SendingService.addCharacteristic(accelXChar);
  SendingService.addCharacteristic(gyroXChar);
  SendingService.addCharacteristic(magXChar);
  // SendingService.addCharacteristic(tempXChar);

  BLE.addService(SendingService);

  BLE.advertise();

  Serial.println("Bluetooth device is now active, waiting for connections...");


  //---------------------------------------------------Intern LED Setup

  pinMode(LED_BUILTIN, OUTPUT);    //B
  pinMode(LEDR, OUTPUT);           //R
  pinMode(LEDG, OUTPUT);           //G
  //----------------------------------------------------CSV HEADER
  //Serial.println("aX,aY,aZ,gX,gY,gZ");


  //calibAccel(20);
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
        send_String_acc();
        send_String_gyro();
        send_String_mag();
        transRate();
      }

      //transRate();
      //IMU_Capture_wait(); //if active deaticate getallData() and send_xxx_xxx() Functions
      /*
        if (IMU_Capture_wait()) {
        IMU_Capture_send();
        }
      */
    }
  }
  disconnectedLight();

  Serial.print("Disconnected from central: ");
  Serial.println(central.address());

  //IMU_Capture();
}

//-------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------PROGRAMMS
//--------------------------------------------------- Send Int
/*
  void send_Int() {
  accelXChar.writeValue(accelXInt);
  }*/
//--------------------------------------------------- Send String (in ASCII)        !!!Charactersitics needs to be declared as String!!!

void send_String_acc() {
  Level_String = String(ac_x, 2) + "," + String(ac_y, 2) + "," + String(ac_z, 2)/* + "/" + String(gy_x,2) + "/" + String(gy_y,2) + "/" + String(gy_z,2) + "/" + String(ma_x,2) + "/" + String(ma_y,2) + "/" + String(ma_z,2)*/;
  accelXChar.writeValue(Level_String);
}

void send_String_gyro() {
  Level_String = /*String(ac_x, 2) + "," + String(ac_y, 2) + "," + String(ac_z, 2) + "/" + */String(gy_x, 2) + "," + String(gy_y, 2) + "," + String(gy_z, 2)/* + "/" + String(ma_x,2) + "/" + String(ma_y,2) + "/" + String(ma_z,2)*/;
  gyroXChar.writeValue(Level_String);
}

void send_String_mag() {
  Level_String = /*String(ac_x, 2) + "," + String(ac_y, 2) + "," + String(ac_z, 2) + "/" + String(gy_x, 2) + "," + String(gy_y, 2) + "," + String(gy_z, 2)/* + "/" +*/ String(ma_x, 2) + "," + String(ma_y, 2) + "," + String(ma_z, 2);
  magXChar.writeValue(Level_String);
}

void send_String_empty() {
  Level_String = "/n, /n, /n";
  gyroXChar.writeValue(Level_String);
  accelXChar.writeValue(Level_String);
  magXChar.writeValue(Level_String);
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
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ac_x, ac_y, ac_z);

    ac_x = ac_x * 9.81;
    ac_y = ac_y * 9.81;
    ac_z = ac_z * 9.81;
  }

  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope( gy_x , gy_y , gy_z );

  }

  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(ma_x, ma_y, ma_z);

  }

  return true;
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
  Serial.print(IMU.getAccelX_mss(), 6);
  Serial.print("  ");
  Serial.print("AccelY: ");
  Serial.print(IMU.getAccelY_mss(), 6);
  Serial.print("  ");
  Serial.print("AccelZ: ");
  Serial.println(IMU.getAccelZ_mss(), 6);

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
  Serial.print(IMU.getGyroX_rads(), 6);
  Serial.print("  ");
  Serial.print("GyroY: ");
  Serial.print(IMU.getGyroY_rads(), 6);
  Serial.print("  ");
  Serial.print("GyroZ: ");
  Serial.println(IMU.getGyroZ_rads(), 6);
  }
*/
//--------------------------------------------------- magnetic sensor
/*
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
/*
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
