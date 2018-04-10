#include "poteau.h"
#define SENSOR_SIZE 10
#define DISTANCE 10
#define vide 130

Poteau::Poteau(){
	soundpwm::init();
	detected = false;
}

bool Poteau::getDetected(){
	return detected;
}

void Poteau::reset(){
	detected = false;
}


void Poteau::wasDetected(){
	detected = true;
	for(uint8_t i = 0; i < 3;i++){
		soundpwm::beep(81);
		_delay_ms(200);
		soundpwm::off();
		if(i < 2)
			_delay_ms(100);
	}
}

void Poteau::scrutation(Sensor& sensor,uint8_t state){
	//On peut detecter le poteau a 60 on la avant derniere valeur pour regarder le poteau.
	//La valeur actuelle peut etre dans le vide.
	
	//On fait le code 2 fois, une fois pour chaque cote des capteurs.
	if(state != 5){
		for(uint8_t i = 0; i < 2; i++){
			uint8_t* oldVals;
			
			//Choisis si on est sur le cote 0 ou 1 du robot.
			if(i == 0){
				oldVals = sensor.getOldVals0();
			}else{
				oldVals = sensor.getOldVals1();
			}



			if (oldVals[1] <= vide){
				//On peut aller jusqu'a 2 donnee en arriere pour savoir si je la difference de -10 existe		
				bool poteauMaybe = false;
				uint8_t pos = 1;
				for(pos = 1; pos < 2; pos++){
					if(((int8_t)oldVals[0] - (int8_t)oldVals[pos]) > DISTANCE){
						poteauMaybe = true;
						break;
					}
				}
				uint8_t posMin = pos;
				pos++;

				if(poteauMaybe){
				uart::parcoursDebug(sensor,state,"Poteaux maybe");
				uart::parcoursDebug(sensor,state,oldVals[posMin]);
				uart::print("{");
				for(uint8_t i = 0; i <SENSOR_SIZE; i++){
					uart::print(oldVals[i]);
					uart::print(" ");
				}
				uart::print("}");
				uart::println();

					for(;pos < SENSOR_SIZE;pos++){
						if((int8_t)oldVals[pos] - (int8_t)oldVals[posMin] > DISTANCE){
							
							//POTEAUX!!!!
				
							uart::parcoursDebug(sensor,state,"Poteaux!!");
							wasDetected();
							break;
						}
					}
				}
			}
		}
	}
}