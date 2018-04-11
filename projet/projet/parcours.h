#ifndef PARCOURS_H
#define PARCOURS_H
#include "ohBoy.h"
#include "poteau.h"
#include "ajustements.h"
#include "state.h"
#include "tache.h"
#include "mur.h"

class Parcours{
public:
	Parcours();
	void exec();
	
	void changeWall();

	void virage90_0();
	void virage90_1();
	
	static void interrupt180();

private:
	static Parcours* pThis;
	
	Sensor sensor;
	Poteau poteau;
	Mur mur;
	Ajustement ajustement;
	
	
	State state;
	State lastState;

	uint8_t compteurBigTurn;
	bool effectueVirage90;
};
#endif