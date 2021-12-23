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

double hz = 0;
double aXOFF = 0, aYOFF = 0 , aZOFF = 0;

double aX, aY, aZ;
double gyroX, gyroY, gyroZ;

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
  calibAccel();
  Serial.println("acc time velocity distance");
}

void loop() {


  /* Check for new data */
  if (imu.Read()) {

    aX = imu.accel_x_mps2() - aXOFF;
    aY = imu.accel_y_mps2() - aYOFF;
    aZ = imu.accel_z_mps2() - aZOFF;

    // Serial.println(constDistance_m(aY,hz));
    Serial.println(integralDistance_m(aY, hz));

  }
}

double distance = 0;
double velocity = 0;
double t = 0;

//--------------------------------------------------measures the distance via constant accleration formels,
//this is wrong fue to the nature of the acceleration. You receive different velocities within one step( as soon as the position is added)
double constDistance_cm(double acc, double freq) {

  if (acc > 0) {
    t = (freq / 1000); //hz is not time but frequenzy


    distance = (distance) + /*(velocity * t) +*/ (acc * (t * t) * 0.5);
    velocity = acc * t;

    return distance * 100;
  }
}

double constDistance_m(double acc, double freq) {
  if (acc > 0) {
    t = (freq / 1000); //hz is not time but frequenzy


    distance = (distance) /*+ (velocity * t) */ + (acc * (t * t) * 0.5);
    velocity = acc * t;

    return distance;
  }
}
/*
  String constDistance_csv() {
  //measure_cm(aY, hz);
  constDistance_m(aY, hz);

  Serial.print(t, 16);
  Serial.print("\t");
  Serial.print(aY, 16);
  Serial.print("\t");
  Serial.print(velocity, 16);
  Serial.print("\t");
  Serial.println(distance, 16);

  return String(t, 16) + "\t" + String(aY, 16) + "\t" + String(velocity, 16) + "\t" + String(distance, 16) + "\n";
  }
*/
//--------------------------------------------------------------------measure the distance via integral formels, should solve the porblem from above. New problem is the new drift of the IMU
double integralDistance_m(float acc, float freq) {
  t = (freq / 1000); // hz to time

  velocity = 0.02 * acc + velocity;

  distance = 0.02 * velocity + distance;

  return distance;
}

double integralDistance_cm(double acc, double freq) {
  t = (freq / 1000); // hz to time

  velocity = 0.02 * acc + velocity;

  distance = 0.02 * velocity + distance;

  return distance * 100;
}
/*
String integralDistance_csv(double acc, double freq) {
  integralDistance_m(aY, hz);
  /*
    Serial.print(acc, 16);
    Serial.print("\t");
    Serial.print(t, 16);
    Serial.print("\t");
    Serial.print(velocity, 16);
    Serial.print("\t");
    Serial.println(distance, 16);
  

  return String(t, 16) + "\t" + String(acc, 16) + "\t" + String(velocity, 16) + "\t" + String(distance, 16) + "\n";
}
*/
//--------------------------------------------------------------------Get the offset of the acceleration. This is supposed to calib the pure offset on the breadboard
void calibAccel() {
  Serial.println("Calib");
  int rounds = 0;
  while (rounds < 201) {
    if (imu.Read()) {
      aX = imu.accel_x_mps2();
      aY = imu.accel_y_mps2();
      aZ = imu.accel_z_mps2();

      aXOFF = (aX + aXOFF) / 2;
      aYOFF = (aY + aYOFF) / 2;
      aZOFF = (aZ + aZOFF) / 2;

      Serial.print(aXOFF, 16);
      Serial.print("\t");
      Serial.print(aYOFF, 16);
      Serial.print("\t");
      Serial.println(aZOFF, 16);
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
  Serial.println("PROCEEED");
}
