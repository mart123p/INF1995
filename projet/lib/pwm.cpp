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
     
    switch (signal)
    {
        case 100:
          OCR1A = 255; // 255 * (100/100)
          break;

        case 75:
          OCR1A = 191; // 255 * (75/100)
          break;

        case 50:
          OCR1A = 128; // etc...
          break;

        case 25:
          OCR1A = 64;
          break;

        case 0:
          OCR1A = 0;
          break;
    }
}

void pwm::setB(int8_t signal) {
    if (signal >= 0){
      PORTD &= 0xBF; 

    }
    else  {
      PORTD |= 0x40;  
      signal *=-1;
    }
     
    switch (signal)
    {
        case 100:
          OCR1B = 255;
          break;
        
        case 75:
          OCR1B = 191;
          break;
        
        case 50:
          OCR1B = 128;
          break;
        
        case 25:
          OCR1B = 64;
          break;
        
        case 0:
          OCR1B = 0;
          break;
    }
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
