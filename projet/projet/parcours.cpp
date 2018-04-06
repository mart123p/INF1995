#include "parcours.h"
#define defaultSpeed 40
#define vide_0 60
#define vide_1 60
#define borneInfAjustement 13
#define borneSupAjustement 25




Parcours::Parcours(){
	lastValue0 = 0;
	lastValue1 = 0;
	currentValue0 = 0;
	currentValue1 = 0;
}

void Parcours::exec(){
	
	soundpwm::init();
	state = READY;
	lastState = READY;
	light::green();
	canSwitchWall = true;
	bool isRunning = true;
	while(isRunning){
		currentValue0 = sensor.read0();
		currentValue1 = sensor.read1();

		uart::sendData(0x11);
		uart::sendData(currentValue0);
		uart::sendData(currentValue1);

		

		wallScrutation();
		

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

			case SWITCH_WALL:
				changeWall();
			break;

			case WALL_0:
				if(currentValue0 > vide_0)
					state = BIG_TURN_0;
				else{
					ajustement0();
				}
			break;

			case BIG_TURN_0:
				virage90_0();
				state = WALL_0;
			break;

	
			case WALL_1:
				if(currentValue1 > vide_1){
					state = BIG_TURN_1;
				}
				ajustement1();
			break;

			case BIG_TURN_1:
				virage90_1();
				state = WALL_1;
			break;



		}
		lastValue0 = currentValue0;
		lastValue1 = currentValue1;
		_delay_ms(50);
	}
}



void Parcours::wallScrutation(){
	if (canSwitchWall && state == WALL_0 && sensor.read1() < vide_1){
		state = SWITCH_WALL;
		lastState = WALL_0;
	}
	
	if (canSwitchWall && state == WALL_1 && sensor.read0() < vide_0){
		state = SWITCH_WALL;
		lastState = WALL_1;
	}
}

void Parcours::changeWall(){
	if (lastState == WALL_0){
		grosAjustement1();
		
	}
	if (lastState == WALL_1){
		grosAjustement0();
	}

}

//Permet d'aller vers le mur 0
void Parcours::grosAjustement0()
{
	canSwitchWall = false;
	if (currentValue0  > borneSupAjustement){
		
		//Le robot tourne vers le mur 0
		light::red();
		pwm::setA(defaultSpeed);
		pwm::setB(-40);
		_delay_ms(1000);
		
		//Le robot avance pendant une seconde
		pwm::setA(defaultSpeed);
		pwm::setB(defaultSpeed);
		_delay_ms(1000);

		//le robot se replace droit
		pwm::setA(-40);
		pwm::setB(defaultSpeed);
		_delay_ms(1000);

	}

	else
	{
		pwm::setA(defaultSpeed);
		pwm::setB(defaultSpeed);
		state = WALL_0;
	}
	
}

//Permet d'aller vers le mur 1
void Parcours::grosAjustement1()
{
	canSwitchWall = false;
	if (currentValue1  > borneSupAjustement)
	{
		
		//Le robot tourne vers le mur 1
		light::red();
		pwm::setB(defaultSpeed);
		pwm::setA(-40);
		_delay_ms(1000);
		
		//Le robot avance pendant une seconde
		pwm::setB(defaultSpeed);
		pwm::setA(defaultSpeed);
		_delay_ms(1000);

		//le robot se replace droit
		pwm::setB(-40);
		pwm::setA(defaultSpeed);
		_delay_ms(1000);
	}

	else
	{
		pwm::setB(defaultSpeed);
		pwm::setA(defaultSpeed);
		state = WALL_1;
	}
}

void Parcours::virage90_0() {
	pwm::setA(defaultSpeed);
	pwm::setB(defaultSpeed);
	soundpwm::beep(61);
	_delay_ms(2500);
	currentValue0 = sensor.read0();
	if(currentValue0 > vide_0)
	{
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
	soundpwm::beep(61);
	_delay_ms(2500);
	currentValue1 = sensor.read0();
	if(currentValue1 > vide_1)
	{
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
	if(currentValue0 > 16 && currentValue0 < borneSupAjustement)
	{
		light::red();
		pwm::setA(60);

		if(currentValue0 > lastValue0)
		{
			pwm::setB(-40);
		}
		else
		{
			pwm::setB(defaultSpeed);
		}
	}
	else if(currentValue0 < borneInfAjustement)
		{
		light::red();
		pwm::setB(60);
		//Mur à 90 le virage doit etre plus seree
		if(currentValue0 < lastValue0){
			pwm::setA(-40);
		}
		else
		{
			pwm::setA(defaultSpeed);
		}
	}
	else
	{
		light::green();
		pwm::setA(defaultSpeed);
		pwm::setB(defaultSpeed);
	}
}

void Parcours::ajustement1()
{
	if(currentValue1 > 16  && currentValue1 < borneSupAjustement)
	{
		light::red();
		pwm::setB(60);

		if(currentValue1 > lastValue1)
		{
			pwm::setA(-40);
		}
		else
		{
			pwm::setA(defaultSpeed);
		}
	}
	else if(currentValue1 < borneInfAjustement)
	{
		light::red();
		pwm::setA(60);
		//Mur à 90 le virage doit etre plus seree
		if(currentValue1 < lastValue1)
		{
			pwm::setB(-40);
		}
		else
		{
			pwm::setB(defaultSpeed);
		}
	}
	else
	{
		light::green();
		pwm::setA(defaultSpeed);
		pwm::setB(defaultSpeed);
	}
}
