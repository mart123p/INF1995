#include "timer.h"

void timer::init(uint16_t ms) {
  TCCR1A = 0;
  TCCR1B = 0;
  // set prescaler to 64 in CTC mode
  TCCR1B |= (1 << WGM12) | (1 << CS12);

  // initialiser le compteur
  TCNT1 = 0;

  OCR1A = ms * 31;

  // activate the timer
  TIMSK1 |= (1 << OCIE1A);

  // activate the interrupts
  sei();
}

void timer::on() { sei(); }

void timer::off() { cli(); }

void timer::test(uint16_t ms) { timer::init(ms); }
