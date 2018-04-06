#ifndef PARCOURS_H
#define PARCOURS_H
#include "ohBoy.h"

class Parcours{
public:
	Parcours();
	void exec();
	void virage90_0();
	void virage90_1();
	void ajustement0();
	void ajustement1();
private:
	Sensor sensor;
	uint8_t lastValue0;
	uint8_t lastValue1;
	uint8_t currentValue0;
	uint8_t currentValue1;
};
#endif