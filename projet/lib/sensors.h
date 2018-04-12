#ifndef SENSORS_H
#define SENSORS_H


#define SENSOR_SIZE 18

#include "can.h"

class Sensor{
public:
	Sensor();
	uint8_t read0();
	uint8_t read1();
	uint8_t getValSensor0();
	uint8_t getValSensor1();
	uint8_t* getOldVals0();
	uint8_t* getOldVals1();
	uint16_t getAdcSensor0();
	uint16_t getAdcSensor1();

private:
	void push(uint8_t val,uint8_t* array);
	can::can can;
	uint16_t adcSensor0;
	uint16_t adcSensor1;
	uint8_t valSensor0;
	uint8_t valSensor1;
	uint8_t oldVals0[SENSOR_SIZE];
	uint8_t oldVals1[SENSOR_SIZE];

};

#endif 
