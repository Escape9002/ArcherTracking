/* HC05 - Bluetooth AT-Command mode modified on 10 Feb 2019 by Saeed Hosseini https://electropeak.com/learn/ */

#include <SoftwareSerial.h>
SoftwareSerial MyBlue(2, 3); // RX | TX
int flag = 0;
int langPause = 25;
int kurzPause = 100;
void setup() {
  Serial.begin(9600);
  MyBlue.begin(9600);
  for (int i = 3; i < 14; i++) {
    pinMode(i, OUTPUT);
  }
  Serial.println("Ready to connect\nDefualt password is 1234 or 000");
}
void loop() {
  if (MyBlue.available())
    flag = MyBlue.read();

  if (flag == 49) {
    Serial.println("LED On");

      for (int i = 3; i < 14; i ++) {
        digitalWrite(i, HIGH);
        delay(langPause);
        digitalWrite(i, LOW);
        delay(langPause);
      }
      for (int y = 13; y > 3; y = y - 1) {
        digitalWrite(y, HIGH);
        delay(langPause);
        digitalWrite(y, LOW);
        delay(langPause);
      }
    
  }
  if (flag == 48) {
    Serial.println("LED Off");
      for (int c = 4; c < 14; c = c + 2) {
        digitalWrite(c, HIGH);
        delay( kurzPause);
        digitalWrite(c, LOW);
        delay(kurzPause);

        if (c != 4) {
          c = c - 1;
        }

        digitalWrite(c, HIGH);
        delay( kurzPause);
        digitalWrite(c, LOW);
        delay(kurzPause);
    }
  }
}
/* 
HC05 - Bluetooth AT-Command mode 
modified on 10 Feb 2019 
by Saeed Hosseini 
https://electropeak.com/learn/guides
 
#include "SoftwareSerial.h"
SoftwareSerial MyBlue(2, 3); // RX | TX 
void setup() 
{ 
 Serial.begin(9600); 
 MyBlue.begin(38400);  //Baud Rate for AT-command Mode.  
 Serial.println("***AT commands mode***"); 
} 
void loop() 
{ 
 //from bluetooth to Terminal. 
 if (MyBlue.available()) 
   Serial.write(MyBlue.read()); 
 //from termial to bluetooth 
 if (Serial.available()) 
   MyBlue.write(Serial.read());
}
*/
