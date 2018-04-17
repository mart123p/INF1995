/******************************************************************************
 * Mode diagnostic et parcours
 *
 * Créé par 
 * Cédric Tessier (1907831)
 * Martin Pouliot (1900045)
 * Pascal-Alexandre Morel (1907829)
 * Sean Costello (1902126)
 *
 * 15 avril 2018
 *
 * Description du programme:
 * Le programme permet d'alterner entre le mode parcours et diagnostic. Dans 
 * le premier mode, le robot doit se promener et completer un parcours selon
 * des exigences précises. Dans le deuxième mode, le robot envoie et reçoit
 * des données par l'intermédiaire du logiciel RoboDiag.
 *
 *****************************************************************************/

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