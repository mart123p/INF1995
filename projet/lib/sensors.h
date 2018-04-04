#ifndef SENSORS_H
#define SENSORS_H

#define SAMPLE_SIZE 20

#include "can.h"

class Sensor{
public:
	Sensor();
	uint8_t read0();
	uint8_t read1();

private:
	can::can can;
};

#endif 
