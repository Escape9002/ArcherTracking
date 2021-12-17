/*
  Brian R Taylor
  brian.taylor@bolderflight.com

  Copyright (c) 2021 Bolder Flight Systems Inc

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the “Software”), to
  deal in the Software without restriction, including without limitation the
  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
  sell copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.
*/
#define DISABLE_MPU9250_FIFO
#include "mpu9250.h"

/* Mpu9250 object, I2C bus,  0x68 address */
bfs::Mpu9250 imu(&Wire, 0x68);

float hz = 0;
float aXOFF = 0, aYOFF = 0 , aZOFF = 0;

float aX, aY, aZ;
float gyroX, gyroY, gyroZ;

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
  if (!imu.ConfigSrd(19)) {
    Serial.println("Error configured SRD");
    while (1) {}
  }

  hz = 1000 / (imu.srd() + 1);

  Serial.println("start");
 // calibAccel();
}

void loop() {


  /* Check if data read */
  if (imu.Read()) {

    aX = imu.accel_x_mps2() - aXOFF;
    aY = imu.accel_y_mps2() - aYOFF;
    aZ = imu.accel_z_mps2() - aZOFF;
    /*
      Serial.print(imu.gyro_x_radps());
      Serial.print("\t");
      Serial.print(imu.gyro_y_radps());
      Serial.print("\t");
      Serial.println(imu.gyro_z_radps());
    
/*
    Serial.print(aX);
    Serial.print("\t");
    Serial.print(aY);
    Serial.print("\t");
    Serial.println(aZ);
      */
      if (aY > 0) {

      float dist = measure_raw(aY, hz);
      Serial.println(dist);
      //Serial.print("\t");
      //Serial.println(30 / dist);
      }
 
  }


}

float distance = 0;
float velocity = 0;
float t = 0;

float measure_cm(float acc, float freq) {

  t = (freq / 1000); //hz is not time but frequenzy


  distance = (distance) + /*(velocity * t) +*/ (acc * (t * t) * 0.5);
  velocity = acc * t;

  return distance * 100;


}

float measure_raw(float acc, float freq) {

  t = (freq / 1000); //hz is not time but frequenzy


  distance = (distance) /*+ (velocity * t) */+ (acc * (t * t) * 0.5);
  velocity = acc * t;

  return distance;


}

void calibAccel() {
  Serial.println("Calib");
  int rounds = 0;
  while (rounds < 201) {
    if(imu.Read()){
          aX = imu.accel_x_mps2();
    aY = imu.accel_y_mps2();
    aZ = imu.accel_z_mps2();

    aXOFF = (aX + aXOFF) / 2;
    aYOFF = (aY + aYOFF) / 2;
    aZOFF = (aZ + aZOFF) / 2;

    Serial.print(aXOFF);
    Serial.print("\t");
    Serial.print(aYOFF);
    Serial.print("\t");
    Serial.println(aZOFF);
/*
        Serial.print(aX);
    Serial.print("\t");
    Serial.print(aY);
    Serial.print("\t");
    Serial.println(aZ);
*/
    rounds ++;
    }else{
      
    }

  }

 
  delay(5000); 
  Serial.println("PROCEEDING");
}
