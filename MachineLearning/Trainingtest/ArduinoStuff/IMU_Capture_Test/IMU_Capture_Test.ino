/*
  IMU Capture

  This example uses the on-board IMU to start reading acceleration and gyroscope
  data from on-board IMU and prints it to the Serial Monitor for one second
  when the significant motion is detected.

  You can also use the Serial Plotter to graph the data.

  The circuit:
  - Arduino Nano 33 BLE or Arduino Nano 33 BLE Sense board.

  Created by Don Coleman, Sandeep Mistry
  Modified by Dominic Pajak, Sandeep Mistry

  This example code is in the public domain.
*/

#include "MPU9250.h"
// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire, 0x68);
int status;

const float accelerationThreshold = 25; // threshold of significant in G's
const int numSamples = 119;

int samplesRead = numSamples;

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

  // print the header
  Serial.println("aX,aY,aZ,gX,gY,gZ");
}

void loop() {
  float aX, aY, aZ, gX, gY, gZ;
 // IMU.readSensor();

  // wait for significant motion
  while (samplesRead == numSamples) {
    IMU.readSensor();
    // read the acceleration data
    aX = IMU.getAccelX_mss() ;
    aY = IMU.getAccelY_mss() ;
    aZ = IMU.getAccelZ_mss() ;
    // sum up the absolutes
    float aSum = fabs(aX) + fabs(aY) + fabs(aZ);
/*
    Serial.print(aX);
    Serial.print('\t');
    Serial.print(aY);
    Serial.print('\t');
    Serial.print(aZ);
    Serial.print('\t');
    Serial.println(aSum);
*/

    // check if it's above the threshold
    if (aSum >= accelerationThreshold) {
      // reset the sample read count
      samplesRead = 0;
      break;
    }
    //}
  }

  // check if the all the required samples have been read since
  // the last time the significant motion was detected
  while (samplesRead < numSamples) {
    IMU.readSensor();
    // read the acceleration and gyroscope data
    aX = IMU.getAccelX_mss() ;
    aY = IMU.getAccelY_mss();
    aZ = IMU.getAccelZ_mss();
    gX = IMU.getGyroX_rads();
    gY = IMU.getGyroY_rads();
    gZ = IMU.getGyroZ_rads();

    samplesRead++;

    // print the data in CSV format
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

    if (samplesRead == numSamples) {
      // add an empty line if it's the last sample
      Serial.println();
    }
  }
  //}
}
