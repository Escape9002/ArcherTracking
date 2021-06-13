/*
 * !! 
 * Arduino 33 BLE kennt den Map befehl nicht
 * beim ausführen == crash
 * solve= 2* rewset hinterinander und formel verwenden
 * !!
 * 
 */


int PotiValue = 0;


void setup() {

  pinMode(D2, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(9600);
}

void loop() {

  PotiValue = analogRead(A0);
  analogWrite(2, PotiValue);
  Serial.println(PotiValue);
  delay(50);
}
