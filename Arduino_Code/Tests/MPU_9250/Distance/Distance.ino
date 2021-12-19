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

unsigned long previousMillis = 0;
float hz = 0;

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

  Serial.println("Dist_A Dist_B");
}w
    
void loop() {
  float aX, aY, aZ;

  /* Check if data read */
  if (imu.Read()) {

    aX = imu.accel_x_mps2();
    aY = imu.accel_y_mps2();
    aZ = imu.accel_z_mps2();

    /*
      Serial.print(imu.new_imu_data());
      Serial.print("\t");
      Serial.print(imu.new_mag_data());
      Serial.print("\t");
      Serial.print(imu.accel_x_mps2());
      Serial.print("\t");
      Serial.print(imu.accel_y_mps2());
      Serial.print("\t");
      Serial.print(imu.accel_z_mps2());
      Serial.print("\t");
      Serial.print(imu.gyro_x_radps());
      Serial.print("\t");
      Serial.print(imu.gyro_y_radps());
      Serial.print("\t");
      Serial.print(imu.gyro_z_radps());
      Serial.print("\t");
      Serial.print(imu.mag_x_ut());
      Serial.print("\t");
      Serial.print(imu.mag_y_ut());
      Serial.print("\t");
      Serial.print(imu.mag_z_ut());
      Serial.print("\t");
      Serial.print(imu.die_temp_c());
      Serial.print("\n");
    */
    unsigned long currentMillis = millis();
    float interval = float((float(currentMillis) - float(previousMillis)) / 1000);
    previousMillis = currentMillis;

    /*
    Serial.print(hz);
    Serial.print("\t");
    Serial.println(interval);

    if(hz == interval){Serial.println("/////////////////////////////////////");}
*/

    float velocity_a = aY * hz;
    float distance_a = 0.5 * aY * (hz * hz) + velocity_a * hz + distance_a;

    float velocity_b = aY * interval;
    float distance_b = distance_b + 0.5 * velocity_b * interval;

    //Serial.println(distance_a);
    //Serial.print("\t");
    Serial.println(distance_b);

    

  }


}
/*
  int distance() {
  //Motion in one simple direction, let y=0.
  unsigned long currentMillis =  millis(); //record time of new reading
  float interval = float((float(currentMillis) - float(previousMillis)) / 1000) ; //Time of previous reading-Time of Current reading= Interval
  previousMillis = currentMillis;

    ac = bx * interval; //acceleration x time=velocity
    lv = inv;      // initial velocity=lv, final velocity=inv
    inv = inv + ac; // vf =vi+1t;
    s = s + (lv * interval) + 0.5 * ac * interval; // Distance= Previous Distance + Vit + 1/2 x a x t^2

  }
  }*/

  float measure(float acc, float hz){
    float distance, velocity, t;
    t = (hz/1000); //hz is not time but frequenzy

    velocity = acc * t; 
    distance = distance + velocity *   



  
  }
