#ifndef POTEAU_H
#define POTEAU_H
#include <avr/io.h> 
#include <avr/interrupt.h>
#include "ohBoy.h"
#include "tache.h"

class Poteau : Tache {
public:
	
	Poteau();
	void scrutation(Sensor& sensor,State& state,State& lastState);
	bool getDetected();

private:
	void wasDetected();
	void reset();

	uint8_t tick; //Utilise pour limiter le nombre de detection evite
				  //les falses positives. Ou les boucles sans fin de 
				  //dectection de poteau.
	bool detected;
};

#endif