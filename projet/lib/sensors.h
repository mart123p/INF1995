#ifndef SENSORS_H
#define SENSORS_H

#define SAMPLE_SIZE 20

#include "can.h"

class Sensor{
public:
	Sensor();
	uint8_t read0();
	uint8_t read1();
	uint8_t getValSensor0();
	uint8_t getValSensor1();
	uint16_t getAdcSensor0();
	uint16_t getAdcSensor1();

private:
	can::can can;
	uint16_t adcSensor0;
	uint16_t adcSensor1;
	uint8_t valSensor0;
	uint8_t valSensor1;
};

#endif 
