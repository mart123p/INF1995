#include "soundpwm.h"
#include <avr/pgmspace.h>
const uint8_t notesEncoded[] PROGMEM = { 140,133,126,118,110,104,99,94,89,84,79,74,109,66,63,59,56,52,
					49,46,43,41,39,36,34,32,31,29,27,25,24,23,21,20,19,
					18,17};

void soundpwm::init(){
	//Set the ground pin
	DDRB |= 0x0C;	
	PORTB &= ~(1 << PB2);
	//Reinitialise le compteur
    	TCNT0 = 0;

	TCCR0A |= _BV(COM0A0) | _BV(WGM01);
	TCCR0B = (1 << CS02);

}

void soundpwm::beep(uint8_t note){
	TCCR0A |= _BV(COM0A0) | _BV(WGM01);
	if(note >= 45 &&  note <= 81)
		OCR0A = pgm_read_byte(&notesEncoded[note-45]);
}
void soundpwm::off(){
	TCCR0A = 0;
}
