#ifndef PARCOURS_H
#define PARCOURS_H
#include "ohBoy.h"

enum State {WALL_0,BIG_TURN_0, SWITCH_WALL, WALL_1, BIG_TURN_1, READY};

class Parcours{
public:
	Parcours();
	void exec();
	void virage90_0();
	void virage90_1();
	void ajustement0();
	void ajustement1();
	void wallScrutation();
	void grosAjustement0();
	void grosAjustement1();
	void changeWall();
private:
	Sensor sensor;
	uint8_t lastValue0;
	uint8_t lastValue1;
	uint8_t currentValue0;
	uint8_t currentValue1;
	bool canSwitchWall;
	State state;
	State lastState;
};
#endif