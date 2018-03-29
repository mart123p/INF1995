#include <avr/eeprom.h> 
#include "ohBoy.h"
#include "diagnostique.h"
#include "parcours.h"

int main() {
  uart::init();
  light::init();
  pwm::init();
  
  //We check for the first byte
  if(eeprom_read_byte(0) == 1){
    eeprom_write_byte(0,0);
    diagnostique();

  }else{
    eeprom_write_byte(0,1);
    parcours();
  }



  while(true);
}
