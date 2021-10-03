/*
  #include <ArduinoBLE.h>

  void setup() {
  Serial.begin(9600);
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  Serial.println("BLE Central scan");

  // start scanning for peripheral
  BLE.scan();

  // …


  }

  void loop() {
  while (BLE.connected()) {
    BLE.stopScan();

    BLEDevice peripheral = BLE.available();

    if (peripheral) {
      // BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
      BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

      // …

      if (switchCharacteristic.value()) {   // any value other than 0
        Serial.println("LED on");
        digitalWrite(23, HIGH);         // will turn the LED on
      } else {                              // a 0 value
        Serial.println(F("LED off"));
        digitalWrite(23, LOW);          // will turn the LED off
      }
    }
  }
  }
*/
// How to control the RGB Led and Power Led of the Nano 33 BLE boards.

#define RED 22
#define BLUE 24
#define GREEN 23
#define LED_PWR 25

int sec = 250;
void setup() {

Serial.begin(9600);
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(LED_PWR, OUTPUT);

}


void loop() {
  digitalWrite(BLUE, LOW);
  digitalWrite(RED, HIGH);
  delay(sec);
  digitalWrite(GREEN, HIGH);
  digitalWrite(RED, LOW);
  delay(sec);
  digitalWrite(BLUE, HIGH);
  digitalWrite(GREEN, LOW);
  delay(sec);

  if (sec < 1000) {
    sec++;
  } else {
    sec = 1;
  }
  Serial.println(sec);
}
