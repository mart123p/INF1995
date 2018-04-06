#include "parcours.h"
#define defaultSpeed 40

enum State {WALL_0,BIG_TURN_0,  WALL_1,BIG_TURN_1, READY};

// LE BON

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
		uart::sendData(state);

		switch(state){
			case READY:
				if(currentValue0 > currentValue1){ // Determiner quel 
					state = WALL_1;				   // mur suivre
				}else{
					state = WALL_0;
				}
				pwm::set1(defaultSpeed);
				pwm::set0(defaultSpeed);
			break;

			case WALL_0:
				if(currentValue0 > 60){ // Si trop loin au mur, tourner
					state = BIG_TURN_0;
				}else{
					ajustement0();	  	// Pour rester entre 13 et 16 cm
				}						// de distance.
			break;
			case BIG_TURN_0:
				Parcours::virage90_0();
				state = WALL_0;
			break;

	
			case WALL_1:
				if(currentValue1 > 60){
					state = BIG_TURN_1;
				}
				ajustement1();
			break;
			case BIG_TURN_1:
				Parcours::virage90_1();
				state = WALL_1;
			break;

		}
		lastValue0 = currentValue0;
		lastValue1 = currentValue1;
		_delay_ms(50);
	}
}

void Parcours::virage90_0() {
	pwm::set1(defaultSpeed);
	pwm::set0(defaultSpeed);
	soundpwm::beep(60);
	_delay_ms(2500);
	currentValue0 = sensor.read0();
	if(currentValue0 > 60){
		pwm::set1(80);
		pwm::set0(-80);
		_delay_ms(300);
		pwm::set1(defaultSpeed);
		pwm::set0(defaultSpeed);
		_delay_ms(1000);
	}
	soundpwm::off();
}

void Parcours::virage90_1() {
	pwm::set1(defaultSpeed);
	pwm::set0(defaultSpeed);
	soundpwm::beep(60);
	_delay_ms(2500);
	currentValue1 = sensor.read0();
	if(currentValue1 > 60){
		pwm::set1(-80);
		pwm::set0(80);
		_delay_ms(300);
		pwm::set1(defaultSpeed);
		pwm::set0(defaultSpeed);
		_delay_ms(1000);
	}
	soundpwm::off();
}
void Parcours::ajustement0(){
	if(currentValue0 > 16){
		light::red();
		pwm::set1(60);

	if(currentValue0 > lastValue0){
		pwm::set0(-40);
	}else{
		pwm::set0(defaultSpeed);
		}
	}
	else if(currentValue0 < 13){
		light::red();
		pwm::set0(60);
		//Mur à 90 le virage doit etre plus seree
		if(currentValue0 < lastValue0){
			pwm::set1(-40);
		}else{
			pwm::set1(defaultSpeed);
		}
	}
	else{
		light::green();
		pwm::set1(defaultSpeed);
		pwm::set0(defaultSpeed);
	}
}

void Parcours::ajustement1(){
	if(currentValue1 > 16){
		light::red();
		pwm::set0(60);

		if(currentValue1 > lastValue1){
			pwm::set1(-40);
		}else{
			pwm::set1(defaultSpeed);
			}
	}
	else if(currentValue1 < 13){
		light::red();
		pwm::set1(60);
		//Mur à 90 le virage doit etre plus seree
		if(currentValue1 < lastValue1){
			pwm::set0(-40);
		}else{
			pwm::set0(defaultSpeed);
		}
	}
	else{
		light::green();
		pwm::set1(defaultSpeed);
		pwm::set0(defaultSpeed);
	}

}
