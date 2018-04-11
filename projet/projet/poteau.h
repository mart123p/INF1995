#ifndef POTEAU_H
#define POTEAU_H
#include <avr/io.h> 
#include <avr/interrupt.h>
#include "ohBoy.h"
#include "tache.h"

class Poteau : Tache {
public:
	
	/**
	 * Constructeur par défaut de poteau
	 */
	Poteau();
	
	/**
	 * Fait la scrutation en tout temps pour trouver des poteaux
	 * @param sensor    Les sensors utilisés
	 * @param state     Le state qui doit être afficher
	 * @param lastState Dernier sate du robot
	 */
	void scrutation(Sensor& sensor,State& state,State& lastState);
	
	/**
	 * Permet de savoir si le robot à détecté un poteau
	 * @return bool poteau détecté
	 */
	bool getDetected();

private:
	
	/**
	 * Donne les indications nécessaires une fois un poteau détecté
	 */
	void wasDetected();
	
	/**
	 * Remet les états à leur état de base pour pouvoir détecter un autre poteau
	 */
	void reset();

	uint8_t tick; //Utilise pour limiter le nombre de detection evite
				  //les falses positives. Ou les boucles sans fin de 
				  //dectection de poteau.
	bool detected;
};

#endif