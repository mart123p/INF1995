#include "diagnostique.h"

#include "OhBoy.h"

void lectureRequete() {
	uint8_t tableauDonnees[2];	
	bool aLuIntruction = false;
	while(!aLuIntruction) {
		tableauDonnees[0] = uart::readData();
		if(tableauDonnees[0] != 0){
			tableauDonnees[1] = uart::readData();
			aLuIntruction =true;
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
			light::
			break;

		case 0xFB:
			
			break;
	}
}
		

void envoieInformation();


void diagnostique(){
	light::init();
	light::green();
}