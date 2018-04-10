#ifndef PARCOURS_H
#define PARCOURS_H
#include "ohBoy.h"
#include "poteau.h"

enum State {WALL_0,BIG_TURN_0, SWITCH_WALL, WALL_1, BIG_TURN_1, READY}; //Rajouter des etats a la fin

class Parcours{
public:
	Parcours();
	void exec();
	


	bool wallScrutation();

	void changeWall();

	void virage90_0();
	void virage90_1();

	void ajustement0();
	void ajustement1();

	void grosAjustement0();
	void grosAjustement1();
private:
	Sensor sensor;
	Poteau poteau;
	uint8_t lastValue0;
	uint8_t lastValue1;
	uint8_t tick;
	bool canSwitchWall;
	State state;
	State lastState;
};
#endif