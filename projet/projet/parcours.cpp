#include "parcours.h"

enum State {WALL_0, WALL_1, NONE};
void parcours(){
	State state = NONE;
	Sensor sensor;
	bool isRunning = true;
	light::green();
	while(isRunning){
		switch(state){
			case NONE:
				if(sensor.read0() > sensor.read1()){
					state = WALL_1;
				}else{
					state = WALL_0;
				}
				pwm::setA(75);
				pwm::setB(75);
			break;

			case WALL_0:
				uart::print("WALL_0");
			break;
			case WALL_1:
				uart::print("WALL_1");
			break;
		}
		sensor.tick();
	}
}