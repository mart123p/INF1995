#include "light.h"
#include "uart.h"
void light::init() { DDRC |= 0xFF; }

void light::green() {
  PORTC |= (1 << PD2);
  PORTC &= ~(1 << PD3);
}

void light::red() {
  PORTC |= (1 << PD3);
  PORTC &= ~(1 << PD2);
}

void light::amber(uint16_t ms) {
  for (uint16_t i = 0; i < (ms / 3); i++) {
    light::red();
    _delay_ms(1);
    light::green();
    _delay_ms(2);
  }
}

void light::on(uint8_t pin) { PORTC |= pin; }

void light::off() {
  PORTC &= ~(1 << PD2);
  PORTC &= ~(1 << PD3);
}

void light::off(uint8_t pin) { PORTC &= ~(pin); }

void light::controleDeLaDel(uint8_t donnee) {
  switch (donnee) {
    case 0x00:
      light::off();
      break;
    case 0x01:
      light::green();
      break;
    case 0x02:
      light::red();
      break;
  }
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
