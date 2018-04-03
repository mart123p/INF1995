#include "sensors.h"

Sensor::Sensor(){
	pos0 = 0;
	pos1 = 0;
	sum0 = 0;
	sum1 = 0;
}

int Sensor::movingAvg(int *ptrArrNumbers, int *ptrSum, uint8_t pos, uint8_t len, uint16_t nextNum)
{
	  //Subtract the oldest number from the prev sum, add the new number
	  *ptrSum = *ptrSum - ptrArrNumbers[pos] + (int) nextNum;
	  //Assign the nextNum to the position in the array
	  ptrArrNumbers[pos] = nextNum;
	  //return the average
	  return *ptrSum / len;
}

uint8_t Sensor::read0(){
	int newAvg = 0;
	newAvg = movingAvg(data0, &sum0, pos0, SAMPLE_SIZE, can.lecture(0));
	pos0++;
	if (pos0 >= SAMPLE_SIZE){
	    pos0 = 0;
	}
	int distance = 0;
	if(newAvg >= 250){
	   	distance = -23 + (11200/newAvg) + (newAvg/43); 
	}else{
	    distance = -23 + (11970/newAvg) + (newAvg/49); 
	}
	return (uint8_t) distance;
}

uint8_t Sensor::read1(){
	int newAvg = 0;
	newAvg = movingAvg(data1, &sum1, pos1, SAMPLE_SIZE, can.lecture(1));
	pos1++;
	if (pos1 >= SAMPLE_SIZE){
	    pos1 = 0;
	}
	int distance = 0;
	if(newAvg >= 210){
	   	distance = -18 + (9500/newAvg) + (newAvg/51); 
	}else{
	    distance = -132 + (23370/newAvg) + (newAvg/4); 
	}
	return (uint8_t) distance;
}


void Sensor::tick(){
	movingAvg(data0, &sum0, pos0, SAMPLE_SIZE, can.lecture(0));
	pos0++;
	if (pos0 >= SAMPLE_SIZE){
	    pos0 = 0;
	}

	movingAvg(data1, &sum1, pos1, SAMPLE_SIZE, can.lecture(1));
	pos1++;
	if (pos1 >= SAMPLE_SIZE){
	    pos1 = 0;
	}

}