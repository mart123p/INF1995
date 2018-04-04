#ifndef DIAGNOSTIQUE_H
#define DIAGNOSTIQUE_H
#include "ohBoy.h"
#include <avr/pgmspace.h>


// Instructions d'envoie d'identification du robot
#define nom 0xf0
#define equipe 0xf1
#define section 0xf2
#define session 0xf3
#define couleur 0xf4
#define etatBP 0xf5
#define capteurD 0xf6
#define capteurG 0xf7

// Donnees pour identification du robot
#define nSection 0x04
#define vert 0x04


class Diagnostique{
	public:
	void exec();
private:
	
	uint8_t etatBoutonPoussoir();
	void envoieInformation();
	Sensor sensor;
};

void envoyerIdentificationRobot();
void lectureRequete();

#endif
