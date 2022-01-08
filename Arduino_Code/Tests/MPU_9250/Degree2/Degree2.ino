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
double gXOFF = 0, gYOFF = 0 , gZOFF = 0;

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
  calibGyrAccl(200);

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

    gX = imu.gyro_x_radps() - gXOFF;
    gY = imu.gyro_y_radps() - gYOFF;
    gZ = imu.gyro_z_radps() - gZOFF;

    aX = imu.accel_x_mps2() - aXOFF;
    aY = imu.accel_y_mps2() - aYOFF;
    aZ = imu.accel_z_mps2() - aZOFF;

    //---------------------------------------------------which formula to use
    // function takes the radiants per second and true Hz to work.
    //addOrt(gX, gY, gZ, hz);
    //eulerOrt(aX, aY, aZ, hz);
    eulerOrt(aX, aY, aZ, gX, gY, gZ, hz);

  }
}

double t = 0;
double ortX = 0;
double ortY = 0;
double ortZ = 0;

//------------------------------------------------------------
double eulerOrt(double X_out, double Y_out, double Z_out, double freq) {

  t = (freq / 1000); //hz is not time but frequenzy

  X_out = X_out / 9.81;
  Y_out = Y_out / 9.81;
  Z_out = Z_out / 9.81;

  float  roll = atan(Y_out / sqrt(pow(X_out, 2) + pow(Z_out, 2))) * 180 / PI;
  float pitch = atan(-1 * X_out / sqrt(pow(Y_out, 2) + pow(Z_out, 2))) * 180 / PI;


#ifdef CSVDEGREE
  Serial.print(roll);
  Serial.print("\t");
  Serial.print(pitch);
  Serial.print("\n");
#endif
}

double acRoll, acPitch, gyRoll, gyPitch, gyYaw, trueRoll, truePitch, trueYaw;
#define RAD_TO_DEG 57.295779513082320876798154814105
double eulerOrt(double acX, double acY, double acZ, double gyX, double gyY, double gyZ,  double freq) {

  t = (freq / 1000); //hz is not time but frequenzy

  acX = acX / 9.81;
  acY = acY / 9.81;
  acZ = acZ / 9.81;

  acRoll = atan(acY / sqrt(pow(acX, 2) + pow(acZ, 2))) * 180 / PI;
  acPitch = atan(-1 * acX / sqrt(pow(acY, 2) + pow(acZ, 2))) * 180 / PI;

  gyRoll = gyRoll + gyX *0.5 *  t; // deg/s * t = deg
  gyPitch = gyPitch + gyY * 0.5 * t;
  gyYaw = gyYaw + gyZ * 0.5 *   t;

  // Complementary filter - combine acceleromter and gyro angle values
  trueRoll = 0.5 * gyRoll + 0.5 * acRoll;
  truePitch = 0.5 * gyPitch + 0.5 * acPitch;
  trueYaw = gyYaw;

#ifdef CSVDEGREE
  Serial.print(t);
  Serial.print("\t");
  Serial.print(trueRoll);
  Serial.print("\t");
  Serial.print(truePitch);
  Serial.print("\t");
  Serial.print(trueYaw);
  Serial.print("\n");
#endif
}
//--------------------------------------------------------------------Get the offset of the acceleration. This is supposed to calib the pure offset on the breadboard
void calibGyro(int coun) {
  int rounds = 0;
  while (rounds < coun) {
    if (imu.Read()) {
      gX = imu.gyro_x_radps();
      gY = imu.gyro_y_radps();
      gZ = imu.gyro_z_radps();

      gXOFF = gXOFF + gX;
      gYOFF = gYOFF + gY;
      gZOFF = gZOFF + gZ;

      rounds++;
    }
  }

  gXOFF = gXOFF / coun;
  gYOFF = gYOFF / coun;
  gZOFF = gZOFF / coun;

}

void calibAccel(int coun) {
  //Serial.println("Calib");
  int rounds = 0;
  while (rounds < coun) {
    if (imu.Read()) {
      aX = imu.accel_x_mps2();
      aY = imu.accel_y_mps2();

      aXOFF = (aX + aXOFF) ;
      aYOFF = (aY + aYOFF) ;

      rounds ++;
    }
  }
  aXOFF = aXOFF / coun;
  aYOFF = aYOFF / coun;
}

void calibGyrAccl(int coun) {
  int rounds = 0;
  while (rounds < coun) {
    if (imu.Read()) {
      gX = imu.gyro_x_radps();
      gY = imu.gyro_y_radps();
      gZ = imu.gyro_z_radps();

      gXOFF = gXOFF + gX;
      gYOFF = gYOFF + gY;
      gZOFF = gZOFF + gZ;

      aX = imu.accel_x_mps2();
      aY = imu.accel_y_mps2();

      aXOFF = (aX + aXOFF);
      aYOFF = (aY + aYOFF);

      rounds++;
    }
  }
  aXOFF = aXOFF / coun;
  aYOFF = aYOFF / coun;

  gXOFF = gXOFF / coun;
  gYOFF = gYOFF / coun;
  gZOFF = gZOFF / coun;
}
