#include "pwm.h"

void pwm::init() {
  TCCR1A |= (1 << COM1A1) | (1 << COM1B1);
  // SET NON-INVERTING MODE
  TCCR1A |= (1 << WGM10);
  TCCR1B |= (1 << CS11);

  DDRD = 0xff;
}

void pwm::setA(int8_t signal) {
    
    if (signal >= 0){
      PORTD &= 0x7F;  // Reculer si signal negatif
    }
    else  {
        PORTD |= 0x80;    // Avancer si signal positif
        signal *=-1;	// Pour diminuer le nombre de case dans le switch
    }
     
    OCR1A = (int)(255 * signal) / 100;
}

void pwm::setB(int8_t signal) {
    if (signal >= 0){
      PORTD &= 0xBF; 

    }
    else  {
      PORTD |= 0x40;  
      signal *=-1;
    }
    
    OCR1B = (int)(255 * signal) /100;
}


void pwm::off(){
	pwm::setA(0);
	pwm::setB(0);
}

void pwm::test() {
  pwm::init();

  pwm::setA(0);
  pwm::setB(0);
  _delay_ms(1);

  pwm::setA(75);
  pwm::setB(75);
  _delay_ms(4000);

  pwm::setA(-75);
  pwm::setB(-75);
  _delay_ms(4000);

  pwm::setA(100);
  pwm::setB(100);
  _delay_ms(8000);

  pwm::setA(0);
  pwm::setB(0);
}
