#include "poteau.h"
#define SENSOR_SIZE 5
#define DISTANCE 10
#define vide 60
Poteau::Poteau(){
	detected = false;
}

bool Poteau::getDetect(){
	return detected;
}

void reset(){
	detected = false;
}

void Poteau::scrutation(uint8_t* oldVals, uint8_t currentValue){
	if (currentValue < 60){
		
	}







	for (uint8_t i = 0; i < SENSOR_SIZE; i++){
		if (currentValue - oldVals[i] < DISTANCE && oldVals[SENSOR_SIZE] - oldVals[i] < DISTANCE){
			detected = false;
			i = SENSOR_SIZE;
		}
		else{
			detected = true;
		}
	}
}