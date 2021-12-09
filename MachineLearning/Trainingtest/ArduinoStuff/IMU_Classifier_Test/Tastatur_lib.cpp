#include <PluggableUSBHID.h>
#include <USBKeyboard.h>

USBKeyboard keyboard;

void printToKeyboard(String s){
  for(int i = 0; i< s.length(); i++){
    keyboard.printf("%c" ,s[i] );
    delay(10);
  }
}


 int pressToKeyboard(int  s){ //, uint8_t  modifier
  //keyboard.key_code(s, modifier);
  keyboard..printf(s);
 }

   //RIGHT_ARROW,      
    //LEFT_ARROW,        
   // DOWN_ARROW,         
   // UP_ARROW, 


//    {0x2a, 0},          /* BS  */  /* Keyboard Delete (Backspace) */
//    {0x2b, 0},          /* TAB */  /* Keyboard Tab */
 //   {0x28, 0},          /* LF  */  /* Keyboard Return (Enter) */
 
