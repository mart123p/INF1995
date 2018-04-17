#include <avr/eeprom.h> 
#include "ohBoy.h"
#include "diagnostic.h"
#include "parcours.h"

int main() {
  uart::init();
  light::init();
  pwm::init();

  if(eeprom_read_byte(0) == 1){
    eeprom_write_byte(0,0);

    //Debut du mode diagnostic
    //Comunication avec le logiciel robodiag

    Diagnostic diangnostic;
    diangnostic.exec();

  }else{
    eeprom_write_byte(0,1);

    //Debut du mode parcours
    //Robot autonome

    Parcours parcours;
    parcours.exec();
  }

  while(true);
}