#include "ohBoy.h"
#include <avr/interrupt.h>
bool wait = false;
uint8_t tick = 0;
uint8_t soundCount = 0;

void initTimer2(){
	TCCR2A = 0;
  	TCCR2B = 0;
  	// set prescaler to 64 in CTC mode
  	TCCR2B |=  (1 << CS21) | (1 << CS20) | (1 << CS22);

  	// initialiser le compteur
  	TCNT2 = 0;

  	OCR2A = 255;
  	TIMSK2 = 0;

  	// activate the interrupts
  	sei();
}
void enableTimer(){
	TIMSK2 |= (1 << OCIE2A);
}
void disableTimer(){
	TIMSK2 = 0;
}

void timer2Call(){
	if(!wait && soundCount < 3){
		//We do a sound
		soundpwm::beep(81);
		light::green();
		if(tick >= 6){
			light::off();
			soundpwm::off();
			tick = 0;
			soundCount++;
			wait = true;
		}
	}else if(soundCount >= 3){
		disableTimer();
	}else{
		if(tick >= 3){
			tick = 0;
			wait = false;
		}
		//We wait
	}
	tick++;
}

int main(){
	light::init();
	
	initTimer2();
	enableTimer();

	soundpwm::init();
	while(true);
}
ISR(TIMER2_COMPA_vect){
	timer2Call();
}