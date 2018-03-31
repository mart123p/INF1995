#include "diagnostique.h"
int movingAvg(int *ptrArrNumbers, int *ptrSum, int pos, int len, uint16_t nextNum)
{
	  //Subtract the oldest number from the prev sum, add the new number
	  *ptrSum = *ptrSum - ptrArrNumbers[pos] + (int) nextNum;
	  //Assign the nextNum to the position in the array
	  ptrArrNumbers[pos] = nextNum;
	  //return the average
	  return *ptrSum / len;
}

void diagnostique(){
	light::init();
	light::green();
	can::can can;
	int arrNumbers[20] = {0};
	int newAvg = 0;
	int pos = 0;
	int sum = 0;

	while(true){
		newAvg = movingAvg(arrNumbers, &sum, pos, 20, can.lecture(1));
	    pos++;
	    if (pos >= 20){
	      pos = 0;
	    }
	    int distance = 0;
	    if(newAvg >= 210){
	   		distance = -18 + (9500/newAvg) + (newAvg/51); 
	    }else{
	    	distance = -132 + (23370/newAvg) + (newAvg/4); 
	    }

	    uart::print(distance);
	    uart::println();	
	}


	/*
	uart::print("\nSensor 1 [");
	uart::print(can.lecture(1));
	*/

}