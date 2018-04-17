#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H
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


class Diagnostic{
	public:
	   /**
	    * Permet d'envoyer les informations du robot au logiciel Robodiag
	    */
		void exec();
	private:	
		uint8_t etatBoutonPoussoir();
		void envoieInformation();
		Sensor sensor;
};

   /**
    * Permet d'envoyer les informations d'identification du robot au 
    * logiciel Robodiag, lorsque demandé.
    */
	void envoyerIdentificationRobot();

	 /**
    * Permet de lire ce qui est envoyé par le logiciel et d'effectuer
    * des actions en conséquence. 
    */
	void lectureRequete();

#endif
