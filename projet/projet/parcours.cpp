#include "parcours.h"
int movingAvg(int *ptrArrNumbers, int *ptrSum, int pos, int len, uint16_t nextNum);


void parcours(){
	light::init();
	light::red();
	can::can can;
	int arrNumbers[20] = {0};
	int newAvg = 0;
	int pos = 0;
	int sum = 0;

	while(true){
		newAvg = movingAvg(arrNumbers, &sum, pos, 20, can.lecture(0));
	    pos++;
	    if (pos >= 20){
	      pos = 0;
	    }
	    int distance = 0;
	    if(newAvg >= 250){
	   		distance = -23 + (11200/newAvg) + (newAvg/43); 
	    }else{
	    	distance = -23 + (11970/newAvg) + (newAvg/49); 
	    }

	    uart::print(distance);
	    uart::println();	
	}

}