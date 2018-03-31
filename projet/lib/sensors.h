#ifndef SENSORS_H
#define SENSORS_H

#define SAMPLE_SIZE 20

#include "can.h"

class Sensor{
public:
	Sensor();
	uint8_t read0();
	uint8_t read1();
	void tick();

private:
	int movingAvg(int *ptrArrNumbers, int *ptrSum, uint8_t pos, uint8_t len, uint16_t nextNum);
	can::can can;
	int* data0;
	int* data1;
	uint8_t pos0;
	uint8_t pos1;
	int sum0;
	int sum1;
};

#endif 
