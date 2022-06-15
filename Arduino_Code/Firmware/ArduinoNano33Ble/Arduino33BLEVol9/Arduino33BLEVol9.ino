/************************************************************
  MPU9250_DMP_Quaternion
  Quaternion example for MPU-9250 DMP Arduino Library
  Jim Lindblom @ SparkFun Electronics
  original creation date: November 23, 2016
  https://github.com/sparkfun/SparkFun_MPU9250_DMP_Arduino_Library

  The MPU-9250's digital motion processor (DMP) can calculate
  four unit quaternions, which can be used to represent the
  rotation of an object.

  This exmaple demonstrates how to configure the DMP to
  calculate quaternions, and prints them out to the serial
  monitor. It also calculates pitch, roll, and yaw from those
  values.

  Development environment specifics:
  Arduino IDE 1.6.12
  SparkFun 9DoF Razor IMU M0

  Supported Platforms:
  - ATSAMD21 (Arduino Zero, SparkFun SAMD21 Breakouts)
*************************************************************/

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

  compatible with BLETestVol10.apk

  Datenübertragung:

  QuatOne = "[xxx,xxx,xxx]" for x 0-200. by (xxx-100) / 100 you get the quaternions back
  QuatTwo = "[]"
  magXChar =  "[]"


    contained in [Level_String]

    Delivered Values in Quaternions!
*/

#include <SparkFunMPU9250-DMP.h>

#define SerialPort SerialUSB

MPU9250_DMP imu;

//-------------------------------------------------------------------------------------VARIABLEN
//float ac_x, ac_y, ac_z; //Accelerometer
//float gy_x, gy_y, gy_z; //Gyroscope
//float ma_x, ma_y, ma_z; //Magnetometer
// volatile int tempInt = 1;  //Temperature Guy

String Level_String;  //Storage for BLE-Data
int fixed_length = 100;

//-------------------------------------------------------------------------------------VARIABLEN
//-------------------------------------------------------------------------------------BLE_SETUP
#include <ArduinoBLE.h>

BLEService SendingService("c54beb4a-40c7-11eb-b378-0242ac130002");
BLEStringCharacteristic QuatOne("d6b78de4-40c7-11eb-b378-0242ac130002", BLERead | BLENotify | BLEWriteWithoutResponse, fixed_length);
//BLEStringCharacteristic QuatTwo("d6a507ce-5489-11ec-bf63-0242ac130002", BLERead | BLENotify | BLEWriteWithoutResponse, fixed_length);
//BLEStringCharacteristic magXChar("11cf4eb8-86d0-11ec-a8a3-0242ac130002", BLERead | BLENotify | BLEWriteWithoutResponse, fixed_length);
//-------------------------------------------------------------------------------------BLE_SETUP

void setup() {
  SerialPort.begin(115200);

  // Call imu.begin() to verify communication and initialize
  if (imu.begin() != INV_SUCCESS)
  {
    while (1)
    {
      SerialPort.println("Unable to communicate with MPU-9250");
      SerialPort.println("Check connections, and try again.");
      SerialPort.println();
      delay(5000);
    }
  }

  imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT | // Enable 6-axis quat
               DMP_FEATURE_GYRO_CAL, // Use gyro calibration
               100); // Set DMP FIFO rate to 100 Hz (10 Hz min to 200Hz max)
  // DMP_FEATURE_LP_QUAT can also be used. It uses the
  // accelerometer in low-power mode to estimate quat's.
  // DMP_FEATURE_LP_QUAT and 6X_LP_QUAT are mutually exclusive
  //-------------------------------------------------------------------------------
  if (!BLE.begin()) {
    Serial.println("BLE failed to Initiate");
    delay(500);
    while (1);
  }


  BLE.setLocalName("MPU9250");
  BLE.setAdvertisedService(SendingService);

  //BLE.setAdvertisedServiceUuid("c54beb4a-40c7-11eb-b378-0242ac130002");

  SendingService.addCharacteristic(QuatOne);
  //SendingService.addCharacteristic(QuatTwo);
  //SendingService.addCharacteristic(magXChar);
  // SendingService.addCharacteristic(tempXChar);

  BLE.addService(SendingService);

  BLE.advertise();

  Serial.println("Bluetooth device is now active, waiting for connections...");


  //---------------------------------------------------Intern LED Setup

  pinMode(LED_BUILTIN, OUTPUT);    //B
  pinMode(LEDR, OUTPUT);           //R
  pinMode(LEDG, OUTPUT);           //G
}

void loop() {

  BLEDevice central = BLE.central();

  if (central) {
    connectedLight();

    Serial.print("Connected to central: ");
    Serial.println(central.address());

    digitalWrite(LED_BUILTIN, HIGH);

    while (central.connected()) {

      // Check for new data in the FIFO
      if ( imu.fifoAvailable() ) {
        // Use dmpUpdateFifo to update the ax, gx, mx, etc. values
        if ( imu.dmpUpdateFifo() == INV_SUCCESS) {
          sendIMUData();

        }
      }
    }
  }
  disconnectedLight();

  Serial.print("Disconnected from central: ");
  Serial.println(central.address());

}

void sendIMUData(void) {
  // After calling dmpUpdateFifo() the ax, gx, mx, etc. values
  // are all updated.
  // Quaternion values are, by default, stored in Q30 long
  // format. calcQuat turns them into a float between -1 and 1
  float q0 = imu.calcQuat(imu.qw);
  float q1 = imu.calcQuat(imu.qx);
  float q2 = imu.calcQuat(imu.qy);
  float q3 = imu.calcQuat(imu.qz);

  /*
    String Level_String = String(q0, 2) + ", " +
                          String(q1, 2) + ", " +
                          String(q2, 2) + ", " +
                          String(q3, 2);
  */
  
  //QuatOne.writeValue(String(q0, 2) + "," + String(q1, 2) + "," + String(q2, 2));
  //QuatTwo.writeValue(String(q3, 2) + ",0.00, 0.00");
  QuatOne.writeValue(String(q0 * 100 + 100, 0) + "," + String(q1 * 100 + 100, 0) + "," + String(q2 * 100 + 100, 0) + "," + String(q3 * 100 + 100, 0));

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
