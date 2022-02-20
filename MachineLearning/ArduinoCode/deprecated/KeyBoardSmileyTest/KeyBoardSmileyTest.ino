#include "Tastatur_lib.h"
/*
  const String gesture_up = "U023EB";
  const String gesture_down = "U023EC";
  const String gesture_left = "U023EA";
  const String gesture_right = "U023E9";
*/
const String gesture_up = "U01f601";
const String gesture_down = "U01f602";
const String gesture_left = "U01f603";
const String gesture_right = "U01f605";

#include <Arduino_APDS9960.h>

void setup() {
  //Serial.begin(9600);
  //while (!Serial); // Wait for serial monitor to open

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS9960 sensor.");
    while (true); // Stop forever
  }

  pinMode(22, OUTPUT); //R  (all LEDs are LOW-active [see line 26/27] )
  pinMode(23, OUTPUT); //G
  //pinMode(24, OUTPUT); //B
  
}

int proximity = 0;

int PWMR = 0;
int PWMG = 0;
int r, g, b;

void loop() {

  // Check if a proximity reading is available.
  if (APDS.proximityAvailable()) {
    proximity = APDS.readProximity();

    analogWrite(22,proximity);
    analogWrite(23,(255-proximity));
  }

  // check if a gesture reading is available
  if (APDS.gestureAvailable()) {
    int gesture = APDS.readGesture();
    switch (gesture) {
      case GESTURE_UP:
        printToKeyboard(gesture_up);
        break;

      case GESTURE_DOWN:
        printToKeyboard(gesture_down);
        break;

      case GESTURE_LEFT:
        printToKeyboard(gesture_left);
        break;

      case GESTURE_RIGHT:
        printToKeyboard(gesture_right);
        break;

      default:
        // ignore
        break;
    }
  }

// check if a color reading is available
  if (APDS.colorAvailable()) {
    APDS.readColor(r, g, b);
  }
  
}
