/*
   Measures the distance via constance acceleration formulas or integral formulas.
   If wanted one can output the Data as CSV in the Serial Monitor.

   Conifureable:
   CSV-Output
   HZ of the Sensor
   used formula

   BolderFlight Library used
*/
//---------------------------------------------------toggle CSVDISTANCE mode
#define CSVDEGREE 1

#define DISABLE_MPU9250_FIFO
#include "mpu9250.h"

/* Mpu9250 object, I2C bus,  0x68 address */
bfs::Mpu9250 imu(&Wire, 0x68);

double hz = 0;
double aXOFF = 0, aYOFF = 0 , aZOFF = 0;

double gX, gY, gZ;
double aX, aY, aZ;
double gyroX, gyroY, gyroZ;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
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
  // -------------------------------------------------------------HZ of the sensor
  //(9 works ok, 0 zero is to high (rauschen), 19 is too low(ungenügende Daten))
  /* Set the sample rate divider */
  if (!imu.ConfigSrd(9)) {
    Serial.println("Error configured SRD");
    while (1) {}
  }

  hz = 1000 / (imu.srd() + 1);

  //Serial.println("start");
  //calibGyro(200);
  calibAccel(200);

#ifdef CSVDISTANCE
  Serial.print("time");
  Serial.print("\t");
  Serial.print("gyro");
  Serial.print("\t");
  Serial.print("degree");
  Serial.print("\t");
  Serial.println("orientation");
#endif

  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {


  /* Check for new data */
  if (imu.Read()) {

    gX = imu.gyro_x_radps();
    gY = imu.gyro_y_radps();
    gZ = imu.gyro_z_radps();

    aX = imu.accel_x_mps2() - aXOFF;
    aY = imu.accel_y_mps2() - aYOFF;
    aZ = imu.accel_z_mps2() - aZOFF;

    //---------------------------------------------------which formula to use
    // function takes the radiants per second and true Hz to work.
    //addOrt(gX, gY, gZ, hz);
    eulerOrt(aX, aY, aZ, hz);

  }
}

double t = 0;
double ortX = 0;
double ortY = 0;
double ortZ = 0;
//--------------------------------------------------measures the distance via constant accleration formels,
//this is wrong fue to the nature of the acceleration. You receive different velocities within one step( as soon as the position is added)
// correct Formula (distance) + /*(velocity * t) +*/ (acc * (t * t) * 0.5);
void addOrt(double gyX, double gyY, double gyZ, double freq) {
  t = (freq / 1000); //hz is not time but frequenzy

  double degreeX = gyX * (180 * 3.14159);
  double degreeY = gyY * (180 * 3.14159);
  double degreeZ = gyZ * (180 * 3.14159);

  ortX = ortX + degreeX;
  ortY = ortY + degreeY;
  ortZ = ortZ + degreeZ;

#ifdef CSVDEGREE
  Serial.print(t);
  Serial.print("\t");
  Serial.print(gyX);
  Serial.print("\t");
  Serial.print(gyY);
  Serial.print("\t");
  Serial.print(gyZ);

  Serial.print("\t");
  Serial.print(degreeX);
  Serial.print("\t");
  Serial.print(degreeY);
  Serial.print("\t");
  Serial.print(degreeZ);
  /*
    Serial.print("\t");
    Serial.print(ortX);
    Serial.print("\t");
    Serial.print(ortY);
    Serial.print("\t");
    Serial.print(ortZ);
  */
  Serial.print("\n");

#endif

}
//------------------------------------------------------------
double eulerOrt(double X_out, double Y_out, double Z_out, double freq) {

  t = (freq / 1000); //hz is not time but frequenzy

  X_out = X_out / 9, 81;
  Y_out = Y_out / 9, 81;
  Z_out = Z_out / 9, 81;

  float  roll = atan(Y_out / sqrt(pow(X_out, 2) + pow(Z_out, 2))) * 180 / PI;
  float pitch = atan(-1 * X_out / sqrt(pow(Y_out, 2) + pow(Z_out, 2))) * 180 / PI;
  // Low-pass filter
  //rollF = 0.94 * rollF + 0.06 * roll;
  //pitchF = 0.94 * pitchF + 0.06 * pitch;

#ifdef CSVDEGREE
  /*
    Serial.print(X_out);
    Serial.print("\t");
    Serial.print(Y_out);
    Serial.print("\t");
    Serial.print(Z_out);
    Serial.print("\t");
  */
  Serial.print(roll);
  Serial.print("\t");
  Serial.print(pitch);
  Serial.print("\n");
#endif
}
//--------------------------------------------------------------------Get the offset of the acceleration. This is supposed to calib the pure offset on the breadboard
void calibGyro(int coun) {
  Serial.print("Not working");
}

void calibAccel(int coun) {
  //Serial.println("Calib");
  int rounds = 0;
  while (rounds < coun) {
    if (imu.Read()) {
      aX = imu.accel_x_mps2();
      aY = imu.accel_y_mps2();
      aZ = imu.accel_z_mps2();

      aXOFF = (aX + aXOFF) / 2;
      aYOFF = (aY + aYOFF) / 2;
      aZOFF = (aZ + aZOFF) / 2;

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
