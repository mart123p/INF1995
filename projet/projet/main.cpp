#include <avr/eeprom.h> 
#include "ohBoy.h"
#include "diagnostique.h"
#include "parcours.h"

void (Parcours::*ptr180)() const = NULL;

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
    ptr180 = &Parcours::interrupt180;
    parcours.exec();
  }



  while(true);
}

ISR(INT0_vect)
{
  _delay_ms(5);
  if ( PIND & 0x04 ){
    ptr180();
  }
}