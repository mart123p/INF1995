#ifndef PARCOURS_H
#define PARCOURS_H
#include "ohBoy.h"
#include "poteau.h"
#include "ajustements.h"
#include "state.h"

class Parcours{
public:
	Parcours();
	void exec();
	


	bool wallScrutation();

	void changeWall();

	void virage90_0();
	void virage90_1();

private:
	Sensor sensor;
	Poteau poteau;
	Ajustement ajustement;
	uint8_t tick;
	bool canSwitchWall;
	State state;
	State lastState;
};
#endif