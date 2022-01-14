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

   Note:
   - no need for multiple characteristics
   - only sends Strings(decoding by Receiver needed)

  current Errors:
   none

  compatible with BLETestVol6.apk

  Datenübertragung:

    "[Wert(accelX)]|[Wert(accelY)]|[Wert(accelZ)]|
     [Wert(gyroX)]|[Wert(gyroY)]|[Wert(gyroZ)]|
     [Wert(magnetX)]|[Wert(magnetY)]|[Wert(magnetZ)]
    "

    contained in [Lvel_String]
*/

int counts = 0;
//-------------------------------------------------------------------------------------BIBLIOTHEK
//---------------------------------------------------BLE
#include <ArduinoBLE.h>
//-------------------------------------------------------------------------------------BIBLIOTHEK
//-------------------------------------------------------------------------------------VARIABLEN

String Level_String;  //Storage for BLE-Data
int fixed_length = 100;

long newTime = 0; //Wir sind für die Abschaffung von Atomuhren, wir sind für den Ausstieg aus der Zeit!
long oldTime = 0;
//-------------------------------------------------------------------------------------VARIABLEN
//-------------------------------------------------------------------------------------BLE_SETUP
BLEService SendingService("c54beb4a-40c7-11eb-b378-0242ac130002");
BLEStringCharacteristic accelXChar("d6b78de4-40c7-11eb-b378-0242ac130002", BLERead | BLENotify | BLEWriteWithoutResponse, fixed_length);
BLEStringCharacteristic gyroXChar("d6a507ce-5489-11ec-bf63-0242ac130002", BLERead | BLENotify | BLEWriteWithoutResponse, fixed_length);
//-------------------------------------------------------------------------------------BLE_SETUP
//-------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------SETUP()
void setup() {
  Serial.begin(115200);
  //while (!Serial)

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
  // SendingService.addCharacteristic(accelYChar);
  // SendingService.addCharacteristic(accelZChar);

  BLE.addService(SendingService);

  BLE.advertise();

  Serial.println("Bluetooth device is now active, waiting for connections...");


  //---------------------------------------------------Intern LED Setup

  pinMode(LED_BUILTIN, OUTPUT);    //B
  pinMode(LEDR, OUTPUT);           //R
  pinMode(LEDG, OUTPUT);           //G
  //----------------------------------------------------CSV HEADER
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
       send_String_val();
       send_String_valZwei();
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
bool getallData(){
 counts = counts +0;
 return true;
}
//-------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------PROGRAMMS
//--------------------------------------------------- Send Int
/*
  void send_Int() {
  accelXChar.writeValue(accelXInt);
  }*/
//--------------------------------------------------- Send String (in ASCII)        !!!Charactersitics needs to be declared as String!!!

void send_String_val() {
  Level_String = String(counts, 2) + "," + String(counts, 2) + "," + String(counts, 2)/* + "/" + String(gy_x,2) + "/" + String(gy_y,2) + "/" + String(gy_z,2) + "/" + String(ma_x,2) + "/" + String(ma_y,2) + "/" + String(ma_z,2)*/;
  accelXChar.writeValue(Level_String);
}

void send_String_valZwei() {
  Level_String = String(counts, 2) + "," + String(counts, 2) + "," + String(counts, 2)/* + "/" + String(gy_x,2) + "/" + String(gy_y,2) + "/" + String(gy_z,2) + "/" + String(ma_x,2) + "/" + String(ma_y,2) + "/" + String(ma_z,2)*/;
  gyroXChar.writeValue(Level_String);
}

//--------------------------------------------------- get all data except temperatur
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
