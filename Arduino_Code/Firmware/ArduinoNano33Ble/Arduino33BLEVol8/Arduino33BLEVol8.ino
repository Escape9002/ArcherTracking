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
   accel:
   d6b78de4-40c7-11eb-b378-0242ac130002

  gyro:
  d6a507ce-5489-11ec-bf63-0242ac130002

  mag:
  11cf4eb8-86d0-11ec-a8a3-0242ac120002

   Note:
   - only sends Strings(decoding by Receiver needed)

  current Errors:
   none

  compatible with BLETestVol11.apk and above

  Datenübertragung:

  acclXChar = "[Wert(accelX)],[Wert(accelY)],[Wert(accelZ)]"
  gyroXChar = "[Wert(gyroX)],[Wert(gyroY)],[Wert(gyroZ)]"
  magXChar =  "[Wert(magnetX)],[Wert(magnetY)],[Wert(magnetZ)]"
    

    contained in [Level_String]

    ! Last compatible MPU9250 library is 5.3.1!
*/

double aXOFF = 0.00;
double aYOFF = 0.00;
double aZOFF = 0.00;
//-------------------------------------------------------------------------------------BIBLIOTHEK
//---------------------------------------------------BLE
#include <ArduinoBLE.h>
//---------------------------------------------------MPU9250
#include "mpu9250.h"
// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
bfs::Mpu9250 imu(&Wire, 0x68);
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
BLEStringCharacteristic magXChar("11cf4eb8-86d0-11ec-a8a3-0242ac130002", BLERead | BLENotify |BLEWriteWithoutResponse, fixed_length);
//-------------------------------------------------------------------------------------BLE_SETUP
//-------------------------------------------------------------------------------------DATA FOR ML
const float accelerationThreshold = 26; // threshold of significant in G's || normal movement == up to 20-23 G (added up)
const int numSamples = 80;
int samplesRead = numSamples;
//-------------------------------------------------------------------------------------DATA FOR ML
//-------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------SETUP()
void setup() {
  //Serial.begin(115200);
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
  
    if (!imu.ConfigSrd(9)) { //19
    Serial.println("Error configured SRD");
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
  Level_String = /*String(ac_x, 2) + "," + String(ac_y, 2) + "," + String(ac_z, 2) + "/" + String(gy_x, 2) + "," + String(gy_y, 2) + "," + String(gy_z, 2)/* + "/" +*/ String(ma_x,2) + "," + String(ma_y,2) + "," + String(ma_z,2);
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
  if (imu.Read()) {
    ac_x = (imu.accel_x_mps2() - aXOFF);
    ac_y = (imu.accel_y_mps2() - aYOFF);
    ac_z = (imu.accel_z_mps2() - aZOFF)* (-1);

    gy_x = imu.gyro_x_radps();
    gy_y = imu.gyro_y_radps();
    gy_z = imu.gyro_z_radps();

    
    ma_x = imu.mag_x_ut();  
    ma_y = imu.mag_y_ut();
    ma_z = imu.mag_z_ut();
    
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

//-------------------------------------------------- IMU_Capture
boolean IMU_Capture_wait() {
  // wait for significant motion
  if (samplesRead == numSamples) {
    // if (IMU.accelerationAvailable()) {
    // read the acceleration data

    if (!getallData()) {
      return false;
    }

    // sum up the absolutes
    float aSum = fabs(ac_x) + fabs(ac_y) + fabs(ac_z);
    //Serial.println(aSum);

    // check if it's above the threshold
    if (aSum >= accelerationThreshold) {
      // reset the sample read count
      samplesRead = 0;
      return true;
    }
    return false;
  }
  return false;
}
void IMU_Capture_send() {
  // check if the all the required samples have been read since
  // the last time the significant motion was detected
  while (samplesRead < numSamples) {
    // read the acceleration and gyroscope data
    if (getallData()) {
      samplesRead++;

      send_String_acc();
      send_String_gyro();
    }



    if (samplesRead == numSamples) {
      // add an empty line if it's the last sample
      send_String_empty();
    }
  }
}

void calibAccel(int coun) {
  //Serial.println("Calib");
  int rounds = 0;
  while (rounds < coun) {
    if (imu.Read()) {
      ac_x = imu.accel_x_mps2();
      ac_y = imu.accel_y_mps2();
      ac_z = imu.accel_z_mps2();

      aXOFF = (ac_x + aXOFF) / 2;
      aYOFF = (ac_y + aYOFF) / 2;
      aZOFF = (ac_z + aZOFF) / 2;

      //  Serial.print(aXOFF, 16);
      // Serial.print("\t");
      //Serial.print(aYOFF, 16);
      //Serial.print("\t");
      //Serial.println(aZOFF, 16);
      /*
              Serial.print(aX);
          Serial.print("\t");
          Serial.print(aY);
          Serial.print("\t");
          Serial.println(aZ);
      */
      rounds ++;
    } else {

    }

  }
  //  Serial.println("PROCEEED");
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
