#include <avr/eeprom.h> 
#include "ohBoy.h"
#include "diagnostique.h"
#include "parcours.h"
void delay(uint16_t time){
  for(uint16_t i =0; i < time; i++){
    _delay_ms(1);
  }
}

int main() {
  uart::init();
  light::init();
  pwm::init();

  //We check for the first byte
  if(eeprom_read_byte(0) == 1){
    eeprom_write_byte(0,0);
    Diagnostique diangnostique;
    diangnostique.exec();

  }else{
    eeprom_write_byte(0,1);
    Parcours parcours;
    parcours.exec();
  }



  while(true);
}
