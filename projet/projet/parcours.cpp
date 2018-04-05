#include "parcours.h"
#define defaultSpeed 40

enum State {WALL_0, WALL_1, NONE, READY};


void Parcours::exec(){
	
	uart::print("Début parcours\n");
	State state = NONE;
	bool isRunning = true;
	uint8_t cntReady = 0;
	light::green();
	uint8_t lastValue0 = 0;
	uint8_t lastValue1 = 0;
	uint8_t currentValue0 = 0;
	uint8_t currentValue1 = 0;
	while(isRunning){
		currentValue0 = sensor.read0();
		currentValue1 = sensor.read1();
		uart::print("0_");
		uart::print(currentValue0);
		uart::print(" ");
		uart::print("1_");
		uart::print(currentValue1);
		uart::println();

		switch(state){
			case NONE:
				cntReady++;
				if(cntReady >= 60)
					state = READY;
			case READY:
				if(currentValue0 > currentValue1){
					state = WALL_1;
				}else{
					state = WALL_0;
				}
				pwm::setA(defaultSpeed);
				pwm::setB(defaultSpeed);
			break;

			case WALL_0:
				if(currentValue0 > 16){
					light::red();
					pwm::setA(60);

					if(currentValue0 > lastValue0){
						pwm::setB(-40);
					}else{
						pwm::setB(defaultSpeed);
					}
				}
				else if(currentValue0 < 13){
					
					light::red();
					pwm::setB(60);
					
					//Mur à 90 le virage doit etre plus seree
					if(currentValue0 < lastValue0){
						pwm::setA(-40);
					}else{
						pwm::setA(defaultSpeed);
					}
				}
				else{
					light::green();
					pwm::setA(defaultSpeed);
					pwm::setB(defaultSpeed);
				}

			break;
			case WALL_1:
			break;
		}
		lastValue0 = currentValue0;
		lastValue1 = currentValue1;
		_delay_ms(50);
	}
}