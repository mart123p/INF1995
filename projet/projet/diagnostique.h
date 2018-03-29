#ifndef DIAGNOSTIQUE_H
#define DIAGNOSTIQUE_H
#include "ohBoy.h"
#include <avr/pgmspace.h>


const uint8_t robot[] = {'S','n','o','o','p','y'};
const uint8_t nEquipe[] PROGMEM = {'8', '2', '9', '2'};
const uint8_t nSession[] PROGMEM = {'1','8','-','1'};

uint8_t etatBoutonPoussoir();
void envoyerIdentificationRobot();
void lectureRequete();
void envoieInformation();
void diagnostique();

#endif
