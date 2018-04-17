#include "diagnostique.h"
#include "ohBoy.h"

volatile bool ready = false;

void Diagnostic::exec(){
	DDRD &= ~(1 << DDD2);     // Mettre le bouton en entrée 
	
	UCSR0B |= (1 << RXCIE0); // Activate receive interrupt

	sei();
	while(1) {
		if(ready){
	   		envoieInformation();
	   		_delay_ms(50);
	   	}
  }
}

ISR(USART0_RX_vect) {
	ready = true;
	lectureRequete();
}

uint8_t Diagnostic::etatBoutonPoussoir(){
	if(PIND & 0x04) {
		return 0x00; 		// 0x00 pour enfonce
	}
	return 0x01;			// 0x01 pour relache
} 


void envoyerIdentificationRobot() {
	uint8_t robot[6] = {'S','n','o','o','p','y'};
	uint8_t nEquipe[4] = {'8', '2', '9', '2'};
	uint8_t nSession[4] = {'1','8','-','1'};
	// nom
	uart::sendData(nom);
	for (int i = 0; i < 6; i++) {
		uart::sendData(robot[i]);
	}
	// equipe
	uart::sendData(equipe);
	for (int i = 0; i < 4; i++)
		uart::sendData(nEquipe[i]);
	// couleur
	uart::sendData(couleur);
	uart::sendData(vert);	
	
	// section
	uart::sendData(section);
	uart::sendData(nSection);	
	
	// session
	uart::sendData(session);
	for (int i = 0; i < 4; i++)
		uart::sendData(nSession[i]);
	
}


void lectureRequete() {
	uint8_t tableauDonnees[2];	
	bool aLuIntruction = false;
	while(!aLuIntruction) {
		tableauDonnees[0] = uart::readData(); 		// On met instruction dans tableau
		cli();
		if(tableauDonnees[0] != 0){			  		// S'il y a une instruction
 			tableauDonnees[1] = uart::readData();   // valide, on lit la donnee
			aLuIntruction = true;
		}
	}

	switch (tableauDonnees[0]){

		case 0xF8:
			pwm::set0(tableauDonnees[1]);
			break;

		case 0xF9:					
			pwm::set1(tableauDonnees[1]);
			break;

		case 0xFA:
			light::controleDeLaDel(tableauDonnees[1]); 
			break;

		case 0xFB:
			envoyerIdentificationRobot();
			ready = true;
			break;
	}
	sei();


}
		

void Diagnostic::envoieInformation() {
	// Envoie etat du BP
	uart::sendData(etatBP);
	uart::sendData(etatBoutonPoussoir());
	// Envoie etat des capteurs
	uart::sendData(capteurD);
	uart::sendData(sensor.read0());
	uart::sendData(capteurG);
	uart::sendData(sensor.read1());
}
