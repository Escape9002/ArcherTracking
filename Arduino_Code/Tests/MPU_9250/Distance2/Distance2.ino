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
#define CSVDISTANCE 1

#define DISABLE_MPU9250_FIFO
#include "mpu9250.h"

/* Mpu9250 object, I2C bus,  0x68 address */
bfs::Mpu9250 imu(&Wire, 0x68);

double hz = 0;
double aXOFF = 0, aYOFF = 0 , aZOFF = 0;

double aX, aY, aZ;
double gyroX, gyroY, gyroZ;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  /* Serial to display data */
  Serial.begin(115200);
  //while (!Serial) {}
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
  if (!imu.ConfigSrd(19)) {
    Serial.println("Error configured SRD");
    while (1) {}
  }

  hz = 1000 / (imu.srd() + 1);

  //Serial.println("start");
  calibAccel(200);

#ifdef CSVDISTANCE
  Serial.print("time");
  Serial.print("\t");
  Serial.print("acc");
  Serial.print("\t");
  Serial.print("velocity");
  Serial.print("\t");
  Serial.println("distance");
#endif

  digitalWrite(LED_BUILTIN, LOW);
}

double aYold = 0.00;
void loop() {


  /* Check for new data */
  if (imu.Read()) {

    //aX = imu.accel_x_mps2() - aXOFF;
    //aY = (aYold *0.5) + (0.5 *(imu.accel_y_mps2() - aYOFF));
    aY = imu.accel_y_mps2() - aYOFF ;
    //Serial.println(aY);
    //aYold = aY;
    //aZ = imu.accel_z_mps2() - aZOFF;

    //---------------------------------------------------which formula to use
    // function takes the acceleration and true Hz to work.
    //constDistance_m(aY, hz);
    //constDistance_cm(aY, hz);

    integralDistance_m(aY, hz);
    //integralDistance_m_second(aY, hz);


  }
}

double distance = 0;
double velocity = 0;
double t = 0;

//--------------------------------------------------measures the distance via constant accleration formels,
//this is wrong fue to the nature of the acceleration. You receive different velocities within one step( as soon as the position is added)
// correct Formula (distance) + /*(velocity * t) +*/ (acc * (t * t) * 0.5);
double constDistance_cm(double acc, double freq) {

  if (acc > 0) {
    t = (freq / 1000); //hz is not time but frequenzy



    distance = (distance) + /*(velocity * t) +*/ (acc * (t * t) * 0.5);
    velocity = acc * t;

#ifdef CSVDISTANCE
    Serial.print(t);
    Serial.print("\t");
    Serial.print(acc);
    Serial.print("\t");
    Serial.print(velocity);
    Serial.print("\t");
    Serial.println(distance);
#endif

    return distance * 100;
  }
}

//int debounce = 0;

double constDistance_m(double acc, double freq) {
  if (acc > 0) {
    t = (freq / 1000); //hz is not time but frequenzy

    distance = (distance) /*+ (velocity * t) */ + (acc * (t * t) * 0.5);
    velocity = acc * t;

#ifdef CSVDISTANCE
    Serial.print(t);
    Serial.print("\t");
    Serial.print(acc);
    Serial.print("\t");
    Serial.print(velocity);
    Serial.print("\t");
    Serial.println(distance);
#endif

    return distance;
  /*  debounce = 0;
  } else {
    if (debounce > 20 && !(String(Serial.read()).equals("\n"))) {
      Serial.print("\n");
      debounce = 0;
    } else {
      debounce++;
    }*/
  }
}
//--------------------------------------------------------------------measure the distance via integral formels, should solve the porblem from above. New problem is the new drift of the IMU
double accOld = 0;
double velocityOld = 0;

double integralDistance_m(float acc, float freq) {
  t = (freq / 1000); // hz to time

  velocity = t * ((acc*0.5 + accOld*0.5) ) + velocity;
  accOld = acc;
  distance = t * ((velocity*0.5 + velocityOld*0.5) ) + distance;
  velocityOld = velocity;

#ifdef CSVDISTANCE
  Serial.print(t);
  Serial.print("\t");
  Serial.print(acc);
  Serial.print("\t");
  Serial.print(velocity);
  Serial.print("\t");
  Serial.println(distance);
#endif

  return distance;
}


double integralDistance_m_second(float acc, float freq) {
  t = (freq / 1000); // hz to time
  //t = 1 / freq;

  velocity = acc * t;

  distance = (acc * (t * t) ) / 2 + velocity * t + distance;

#ifdef CSVDISTANCE
  Serial.print(t);
  Serial.print("\t");
  Serial.print(acc);
  Serial.print("\t");
  Serial.print(velocity);
  Serial.print("\t");
  Serial.println(distance);
#endif

  return distance;
}

double integralDistance_cm(double acc, double freq) {
  t = (freq / 1000); // hz to time

  velocity = t * ((acc + accOld) / 2) + velocity;
  accOld = acc;
  distance = t * ((velocity + velocityOld) / 2) + distance;
  velocityOld = velocity;


#ifdef CSVDISTANCE
  Serial.print(t);
  Serial.print("\t");
  Serial.print(acc);
  Serial.print("\t");
  Serial.print(velocity);
  Serial.print("\t");
  Serial.println(distance);
#endif
  return distance * 100;
}
//--------------------------------------------------------------------Get the offset of the acceleration. This is supposed to calib the pure offset on the breadboard
void calibAccel(int coun) {
  //Serial.println("Calib");
  int rounds = 0;
  while (rounds <= coun) {
    if (imu.Read()) {
      aX = imu.accel_x_mps2();
      aY = imu.accel_y_mps2();
      aZ = imu.accel_z_mps2();

      aXOFF = (aX + aXOFF) ;
      aYOFF = (aY + aYOFF) ;
      aZOFF = (aZ + aZOFF) ;

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
  aXOFF = aXOFF / coun;
  aYOFF = aYOFF / coun;
  aZOFF = aZOFF / coun;
  //  Serial.println("PROCEEED");
}
