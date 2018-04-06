#include "parcours.h"
#define defaultSpeed 40

enum State {WALL_0, WALL_1, READY};

Parcours::Parcours(){
	lastValue0 = 0;
	lastValue1 = 0;
	currentValue0 = 0;
	currentValue1 = 0;
}

void Parcours::exec(){
	
	soundpwm::init();
	State state = READY;
	light::green();
	
	bool isRunning = true;
	while(isRunning){
		currentValue0 = sensor.read0();
		currentValue1 = sensor.read1();

		uart::sendData(0x11);
		uart::sendData(currentValue0);
		uart::sendData(currentValue1);

		switch(state){
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
				if(currentValue0 > 60){
					//Trou
					Parcours::virage90_0();
				}else{
					ajustement0();
				}
			break;
			case WALL_1:
				if(currentValue1 > 60){
					//Trou
					Parcours::virage90_1();
				}
				ajustement1();
			break;
		}
		lastValue0 = currentValue0;
		lastValue1 = currentValue1;
		_delay_ms(50);
	}
}

void Parcours::virage90_0() {
	pwm::setA(defaultSpeed);
	pwm::setB(defaultSpeed);
	soundpwm::beep(60);
	_delay_ms(2500);
	currentValue0 = sensor.read0();
	if(currentValue0 > 60){
		pwm::setA(80);
		pwm::setB(-80);
		_delay_ms(300);
		pwm::setA(defaultSpeed);
		pwm::setB(defaultSpeed);
		_delay_ms(1000);
	}
	soundpwm::off();
}

void Parcours::virage90_1() {
	pwm::setA(defaultSpeed);
	pwm::setB(defaultSpeed);
	soundpwm::beep(60);
	_delay_ms(2500);
	currentValue1 = sensor.read0();
	if(currentValue1 > 60){
		pwm::setA(-80);
		pwm::setB(80);
		_delay_ms(300);
		pwm::setA(defaultSpeed);
		pwm::setB(defaultSpeed);
		_delay_ms(1000);
	}
	soundpwm::off();
}
void Parcours::ajustement0(){
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
}

void Parcours::ajustement1(){
	if(currentValue1 > 16){
		light::red();
		pwm::setB(60);

		if(currentValue1 > lastValue1){
			pwm::setA(-40);
		}else{
			pwm::setA(defaultSpeed);
			}
	}
	else if(currentValue1 < 13){
		light::red();
		pwm::setA(60);
		//Mur à 90 le virage doit etre plus seree
		if(currentValue1 < lastValue1){
			pwm::setB(-40);
		}else{
			pwm::setB(defaultSpeed);
		}
	}
	else{
		light::green();
		pwm::setA(defaultSpeed);
		pwm::setB(defaultSpeed);
	}

}