#ifndef POTEAU_H
#define POTEAU_H
#include <avr/io.h> 
#include <avr/interrupt.h>
#include "ohBoy.h"

extern "C" void TIMER2_COMPA_vect(void) __attribute__((signal)); 


class Poteau{
	friend void ::TIMER2_COMPA_vect(void); 
public:
	
	Poteau();
	bool getDetected();
	void scrutation(Sensor& sensor,uint8_t state);


private:
	void onTimer2();
	void wasDetected();
	void reset();
	bool detected;
	uint8_t soundCount;
};

#endif