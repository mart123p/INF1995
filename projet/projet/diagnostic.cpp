#include "diagnostic.h"
#include "ohBoy.h"

volatile bool ready = false;

void Diagnostic::exec(){
	DDRD &= ~(1 << DDD2);     //Mettre le bouton en entrée 
	
	UCSR0B |= (1 << RXCIE0);  //Activer le récepteur d'interruption

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
	// Retourne 0 si le bouton est enfoncé et 1 sinon.
	return (!(PIND & 0x04));   
} 


void envoyerIdentificationRobot() {
	uint8_t robot[6] = {'S','n','o','o','p','y'};
	uint8_t nEquipe[4] = {'8', '2', '9', '2'};
	uint8_t nSession[4] = {'1','8','-','1'};

	// Envoie du nom du robot
	uart::sendData(nom);
	for (int i = 0; i < 6; i++) 
		uart::sendData(robot[i]);

	// Envoie du numéro d'équipe
	uart::sendData(equipe);
	for (int i = 0; i < 4; i++)
		uart::sendData(nEquipe[i]);

	// Envoie de la couleur du robot
	uart::sendData(couleur);
	uart::sendData(vert);	
	
	// Envoie du numéro de section
	uart::sendData(section);
	uart::sendData(nSection);	
	
	// Envoie du numéro de session
	uart::sendData(session);
	for (int i = 0; i < 4; i++)
		uart::sendData(nSession[i]);
	
}


void lectureRequete() {
	
	// Tableau qui stocke les données envoyées par le logiciel RoboDiag
	uint8_t tableauDonnees[2];	
	bool aLuIntruction = false;

	while(!aLuIntruction) {
		// On met l'instruction à la position 0 du tableau
		tableauDonnees[0] = uart::readData(); 		
		cli();
		if(tableauDonnees[0] != 0){			  		// S'il y a une instruction
 			tableauDonnees[1] = uart::readData();   // valide, on lit la donnée
			aLuIntruction = true;
		}
	}

	switch (tableauDonnees[0]){

		case 0xF8:
			// Poser la vitesse de la roue gauche à une certaine donnée
			pwm::set0(tableauDonnees[1]);
			break;

		case 0xF9:
			// Poser la vitesse de la roue droite à une certaine donnée				
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
