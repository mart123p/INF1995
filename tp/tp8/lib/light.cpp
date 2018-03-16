#include "light.h"
#include "uart.h"
void light::init() { DDRB |= 0x0c; }

void light::green() {
  PORTB |= (1 << PD2);
  PORTB &= ~(1 << PD3);
}

void light::red() {
  PORTB |= (1 << PD3);
  PORTB &= ~(1 << PD2);
}

void light::amber(uint16_t ms) {
  for (uint16_t i = 0; i < (ms / 3); i++) {
    light::red();
    _delay_ms(1);
    light::green();
    _delay_ms(2);
  }
}

void light::off() {
  PORTB &= ~(1 << PD2);
  PORTB &= ~(1 << PD3);
}
void light::test() {
  light::init();
  light::green();
  _delay_ms(5000);
  light::red();
  _delay_ms(5000);
  light::amber(5000);
  light::off();
}
