#include <PluggableUSBHID.h>
#include <USBKeyboard.h>

USBKeyboard keyboard;

void printToKeyboard(String s){
  for(int i = 0; i< s.length(); i++){
    keyboard.printf("%c" ,s[i] );
    delay(10);
  }
}

/*
 void pressToKeyboard(String s){
  keyboard.key_code(s, 0x00);
 }

   //RIGHT_ARROW,      
    //LEFT_ARROW,        
   // DOWN_ARROW,         
   // UP_ARROW,           
 */
