#include "parcours.h"
#define defaultSpeed 50

enum State {WALL_0, WALL_1, NONE, READY};


void Parcours::exec(){
	
	uart::print("Début parcours\n");
	State state = NONE;
	bool isRunning = true;
	uint8_t cntReady = 0;
	uint8_t adjustementCnt = 0;
	bool inAdjustement = true;
	light::green();
	while(isRunning){
		switch(state){
			case NONE:
				cntReady++;
				if(cntReady >= 60)
					state = READY;
			case READY:
				if(sensor.read0() > sensor.read1()){
					state = WALL_1;
				}else{
					state = WALL_0;
				}
				pwm::setA(defaultSpeed);
				pwm::setB(defaultSpeed);
			break;

			case WALL_0:
				//Adjustement for the wall

				if(sensor.read0() > 20){
					light::red();

					inAdjustement = true;
					if(adjustementCnt < 50){
						pwm::setA(75);
					}else{
						pwm::setA(defaultSpeed);
						pwm::setB(defaultSpeed);
					}

				}
				else if(sensor.read0() < 15){
					light::red();
					pwm::setB(75);
					inAdjustement = true;
				}
				else{
					inAdjustement = false;
					light::green();
					pwm::setA(defaultSpeed);
					pwm::setB(defaultSpeed);
				}
				if(inAdjustement){
					if(adjustementCnt < 50)
					adjustementCnt++;
				}else{
					adjustementCnt = 0;
				}

			break;
			case WALL_1:
			break;
		}
		_delay_ms(10);
	}
}