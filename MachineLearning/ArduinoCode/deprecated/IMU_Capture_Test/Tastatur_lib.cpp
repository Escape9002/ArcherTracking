#include <PluggableUSBHID.h>
#include <USBKeyboard.h>

USBKeyboard keyboard;

void printToKeyboard(String s){
  for(int i = 0; i< s.length(); i++){
    keyboard.printf("%c" ,s[i] );
    delay(10);
  }
}
