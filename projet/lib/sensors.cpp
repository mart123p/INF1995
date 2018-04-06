#include "sensors.h"

Sensor::Sensor(){

}

uint8_t Sensor::read0(){
	int newAvg = 0;
	newAvg = can.lecture(0);
	if(newAvg == 0 )
		newAvg = 1;
	int distance = 0;
	if(newAvg >= 240){
	   	distance = -35 + (13205/newAvg) + (newAvg/25); 
	}else{
	    distance = -23 + (11970/newAvg) + (newAvg/49); 
	}
	return (uint8_t) distance;
}

uint8_t Sensor::read1(){
	int newAvg = 0;
	newAvg = can.lecture(1);
	if(newAvg == 0 )
		newAvg = 1;
	int distance = 0;
	if(newAvg >= 210){
	   	distance = -18 + (9500/newAvg) + (newAvg/51); 
	}else{
	    distance = -132 + (23350/newAvg) + (newAvg/4); 
	}
	return (uint8_t) distance;
}