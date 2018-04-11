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
	void onTimer2();
	void wasDetected();
	void reset();
	bool detected;
	uint8_t soundCount;
};

#endif