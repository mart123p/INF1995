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
      PORTD |= 0x80;  // set PIND8 to 1
    }
    else  {
        PORTD &= 0x7F;  // set PIND8 to 0
        signal *=-1;
    }
     
    switch (signal)
    {
    

        case 100:
          OCR1A = 255;
          break;

        case 75:
          OCR1A = 191;
          break;

        case 50:
          OCR1A = 128;
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
      PORTD |= 0x40;  // set PIND8 to 1
    }
    else  {
        PORTD &= 0xBF;  // set PIND8 to 0
        signal *=-1;
    }
     
    switch (signal)
    {
        case 100:
          OCR1A = 255;
          break;
        
        case 75:
          OCR1A = 191;
          break;
        
        case 50:
          OCR1A = 128;
          break;
        
        case 25:
          OCR1A = 64;
          break;
        
        case 0:
          OCR1A = 0;
          break;
    }
}


void pwm::off(){
	pwm::setA(0,0);
	pwm::setB(0,0);
}

void pwm::test() {
  pwm::init();

  pwm::setA(0, 1);
  pwm::setB(0, 1);
  _delay_ms(1);

  pwm::setA(170, 1);
  pwm::setB(170, 1);
  _delay_ms(4000);

  pwm::setA(170, 0);
  pwm::setB(170, 0);
  _delay_ms(4000);

  pwm::setA(255, 0);
  pwm::setB(255, 0);
  _delay_ms(8000);

  pwm::setA(0, 1);
  pwm::setB(0, 1);
}
