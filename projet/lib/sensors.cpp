#include "sensors.h"

Sensor::Sensor(){
	adcSensor0 = 0;
	adcSensor1 = 0;
	valSensor0 = 0;
	valSensor1 = 0;
}

void Sensor::push(uint8_t val, uint8_t* array){
    for(uint8_t i = SENSOR_SIZE; i > 0; i--){
        array[i] = array[i-1];
    }
    array[0] = val;
}

uint8_t Sensor::read0(){
	int newAvg = 0;
	newAvg = can.lecture(0);
	adcSensor0 = (uint16_t) newAvg;
	if(newAvg == 0 )
		newAvg = 1;
	int distance = 0;
	if(newAvg >= 240){
	   	distance = -35 + (13205/newAvg) + (newAvg/25); 
	}else if(newAvg <= 110){
		distance = 80;
	}else{
	    distance = -23 + (11970/newAvg) + (newAvg/49); 
	}
	valSensor0 = (uint8_t) distance;
	
	push(valSensor0,oldVals0);
	
	return valSensor0;
}

uint8_t Sensor::read1(){
	int newAvg = 0;
	newAvg = can.lecture(1);
	adcSensor1 = (uint16_t) newAvg;
	if(newAvg == 0 )
		newAvg = 1;
	int distance = 0;
	if(newAvg >= 210){
	   	distance = -18 + (9500/newAvg) + (newAvg/51); 
	}else if(newAvg <= 110){
		distance = 80;
	}
	else{
	    distance = -132 + (23350/newAvg) + (newAvg/4); 
	}
	valSensor1 = (uint8_t) distance;

	push(valSensor1,oldVals1);
	
	return valSensor1;
}
uint8_t* Sensor::getOldVals0(){
	return oldVals0;
}
uint8_t* Sensor::getOldVals1(){
	return oldVals1;
}
uint8_t Sensor::getValSensor0(){
	return valSensor0;
}
uint8_t Sensor::getValSensor1(){
	return valSensor1;
}
uint16_t Sensor::getAdcSensor0(){
	return adcSensor0;
}
uint16_t Sensor::getAdcSensor1(){
	return adcSensor1;
}
