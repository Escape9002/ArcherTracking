/*
  imu Capture

  This example uses the on-board imu to start reading acceleration and gyroscope
  data from on-board imu and prints it to the Serial Monitor for one second
  when the significant motion is detected.

  You can also use the Serial Plotter to graph the data.

  The circuit:
  - Arduino Nano 33 BLE or Arduino Nano 33 BLE Sense board.

  Created by Don Coleman, Sandeep Mistry
  Modified by Dominic Pajak, Sandeep Mistry

  This example code is in the public domain.
*/

//input your wished freq and receive it in a value usable by imu.sample_rate_divider(int i);

/*
 * Tastaturausgabe mögl. tippen aber langsam + leerstellen werden mit "ß" aufgefüllt. alles =/ 
 * 
 * Tastaur immer auf false!
 */

#include "Tastatur_lib.h"
bool keyb = false; // disbale! [see line 19]

#include "mpu9250.h"
// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
bfs::Mpu9250 imu(&Wire, 0x68);

const float accelerationThreshold = 25; // threshold of significant in G's
const int numSamples = 119;

int samplesRead = numSamples;

int herz(int i) {
  i = (1000 - i) / i;
  return i;
}

int HzToSrd(int i) {
  i = 1000 / (i + 1);
  return i;
}

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

void setup() {
  /* Serial to display data */
  Serial.begin(115200);
  while (!Serial) {}
  /* Start the I2C bus */
  Wire.begin();
  Wire.setClock(400000);
  /* Initialize and configure IMU */
  if (!imu.Begin()) {
    Serial.println("Error initializing communication with IMU");
    while (1) {}
  }
  /* Set the sample rate divider */
  if (!imu.ConfigSrd(herz(50))) {
    Serial.println("Error configured SRD");
    while (1) {}
  }

  //checkIMU();
  //delay(5000);

  // print the header
  if(keyb){
    printToKeyboard("aX,aY,aZ,gX,gY,gZ");
  }
  Serial.println("aX,aY,aZ,gX,gY,gZ");
}


void loop() {
  float aX, aY, aZ, gX, gY, gZ;
  // imu.readSensor();

  // wait for significant motion
  while (samplesRead == numSamples) {

    if (imu.Read()) {

      // read the acceleration data
      aX = imu.gyro_x_radps() ;
      aY = imu.gyro_y_radps() ;
      aZ = imu.gyro_z_radps() ;

      // sum up the absolutes
      float aSum = fabs(aX) + fabs(aY) + fabs(aZ);


      // check if it's above the threshold
      if (aSum >= accelerationThreshold) {
        // reset the sample read count
        samplesRead = 0;
        break;
      }

    }
  }

  // check if the all the required samples have been read since
  // the last time the significant motion was detected
  while (samplesRead < numSamples) {
    if (imu.Read()) {

      // read the acceleration and gyroscope data
      aX = imu.accel_x_mps2() ;
      aY = imu.accel_y_mps2();
      aZ = imu.accel_z_mps2();
      gX = imu.gyro_x_radps();
      gY = imu.gyro_y_radps();
      gZ = imu.gyro_z_radps();

      samplesRead++;

      if (keyb) {
        String params = String(aX,3) + "," + String(aY,3) + "," + String(aZ,3) + "," + String(gX,3) + "," + String(gY,3) + "," + String(gZ,3) + "\n";
        printToKeyboard(params);
        
      } else {
        Serial.print(aX, 3);
        Serial.print(',');
        Serial.print(aY, 3);
        Serial.print(',');
        Serial.print(aZ, 3);
        Serial.print(',');
        Serial.print(gX, 3);
        Serial.print(',');
        Serial.print(gY, 3);
        Serial.print(',');
        Serial.print(gZ, 3);
        Serial.println();
      }
      // print the data in CSV format




      if (samplesRead == numSamples) {
        // add an empty line if it's the last sample
        if(keyb){
          printToKeyboard("\n");
        }else{
          Serial.println();
        }
        
        
      }

    }
  }
}
