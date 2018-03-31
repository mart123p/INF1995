#include "diagnostique.h"

#include "ohBoy.h"

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

uint8_t etatBoutonPoussoir(){
	_delay_ms(5);
	if(PIND & 0x04) {
		return 0x00; 		// 0x00 pour enfonce
	}
	return 0x01;			// 0x01 pour relache
} 



void envoyerIdentificationRobot() {
	// nom
	uart::sendData(nom);
	for (int i = 0; i < 6; i++) {
		uart::sendData(robot[i]);
	}
	// equipe
	uart::sendData(equipe);
	for (int i = 0; i < 4; i++)
		uart::sendData(nEquipe[i]);
	
	// section
	uart::sendData(section);
	uart::sendData(nSection);	
	
	// session
	uart::sendData(session);
	for (int i = 0; i < 4; i++)
		uart::sendData(nSession[i]);
	
	// couleur
	uart::sendData(couleur);
	uart::sendData(vert);	
}


void lectureRequete() {
	uint8_t tableauDonnees[2];	
	bool aLuIntruction = false;
	while(!aLuIntruction) {
		tableauDonnees[0] = uart::readData(); 		// On met instruction dans tableau
		if(tableauDonnees[0] != 0){			  		// S'il y a une instruction
 			tableauDonnees[1] = uart::readData();   // valide, on lit la donnee
			aLuIntruction = true;
		}
	}

	switch (tableauDonnees[0]){

		case 0xF8:
			pwm::setA(tableauDonnees[1]);
			break;

		case 0xF9:
			pwm::setB(tableauDonnees[1]);
			break;

		case 0xFA:
			light::controleDeLaDel(tableauDonnees[1]); 
			break;

		case 0xFB:
			envoyerIdentificationRobot();
			break;
	}
}
		

void envoieInformation() {
	// Envoie etat du BP
	uart::sendData(etatBP);
	uart::sendData(etatBoutonPoussoir());
	// Envoie etat des capteurs
	uart::sendData(capteurD);
	uart::sendData(60);
	uart::sendData(capteurG);
	uart::sendData(60);
}


void diagnostique(){
	light::init();
	while(1) {
	   lectureRequete();
	   //envoieInformation();
  }
}

