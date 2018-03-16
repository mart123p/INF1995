#include "sound.h"
void _delay_us_var(const uint16_t duration) {
  uint16_t i = 0;
  while (i != duration) {
    _delay_us(1);
    i++;
  }
}
/**
 * @brief      Plays a frequency
 *
 * @param[in]  freq      The frequency in hertz From (1000hz to 9999hz)
 * @param[in]  duration  The duration in ms
 */
void frequency(const uint16_t freq, const uint16_t duration) {
  uint16_t i = 0;
  uint16_t time = (1000 * (uint32_t)duration) / (freq + 60);
  while (i != time) {
    PORTC |= (1 << PD2);
    _delay_us(60);  // 60 is the volume...
    PORTC &= ~(1 << PD2);
    _delay_us_var(freq);
    i++;
  }
}

void sound::init() {
  DDRC |= 0x0C;
  PORTC &= ~(1 << PD3);
}

void sound::crazyfrog() {
  for (uint8_t i = 0; i < 2; i++) {
    frequency(C4, 54);
    _delay_ms(447);
    frequency(G4_, 48);
    _delay_ms(327);
    frequency(F4, 41);
    _delay_ms(209);
    frequency(F4, 50);
    _delay_ms(75);
    frequency(A4_, 40);
    _delay_ms(210);
    frequency(F4, 47);
    _delay_ms(203);
    frequency(D4_, 49);
    _delay_ms(201);
    frequency(F4, 51);
    _delay_ms(449);
    frequency(C5, 45);
    _delay_ms(330);
    frequency(F4, 43);
    _delay_ms(207);
    frequency(F4, 45);
    _delay_ms(80);
    frequency(C5_, 34);
    _delay_ms(216);
    frequency(C5, 34);
    _delay_ms(216);
    frequency(G4_, 27);
    _delay_ms(223);
    frequency(F4, 27);
    _delay_ms(223);
    frequency(C5, 27);
    _delay_ms(223);
    frequency(F5, 36);
    _delay_ms(214);
    frequency(F4, 29);
    _delay_ms(96);
    frequency(D4_, 41);
    _delay_ms(205);
    frequency(D4_, 41);
    _delay_ms(84);
    frequency(C4, 32);
    _delay_ms(218);
    frequency(G4, 43);
    _delay_ms(207);
    frequency(F4, 618);
    if (i == 0) _delay_ms(1632);
  }
  _delay_ms(357);
  frequency(F4, 136);
  _delay_ms(364);
  frequency(F4, 136);
}

void sound::fail() {
  frequency(C5, 102);
  _delay_ms(165);
  frequency(G4, 102);
  _delay_ms(165);
  frequency(E4, 155);
  _delay_ms(45);
  frequency(A4, 116);
  _delay_ms(38);
  frequency(B4, 120);
  _delay_ms(34);
  frequency(A4, 119);
  _delay_ms(35);
  frequency(G4_, 155);
  _delay_ms(46);
  frequency(A4_, 155);
  _delay_ms(46);
  frequency(G4_, 155);
  _delay_ms(46);
  frequency(G4, 77);
  _delay_ms(21);
  frequency(F4, 77);
  _delay_ms(25);
  frequency(G4, 933);
}

void sound::success() {
  frequency(C5, 82);
  _delay_ms(26);
  frequency(G4, 82);
  _delay_ms(26);
  frequency(E4, 82);
  _delay_ms(26);
  frequency(C5, 82);
  _delay_ms(26);
  frequency(G4, 82);
  _delay_ms(26);
  frequency(E4, 82);
  _delay_ms(26);
  frequency(C5, 503);
  _delay_ms(144);
  frequency(C5_, 82);
  _delay_ms(26);
  frequency(G4_, 82);
  _delay_ms(26);
  frequency(F4, 82);
  _delay_ms(26);
  frequency(C5_, 82);
  _delay_ms(26);
  frequency(G4_, 82);
  _delay_ms(26);
  frequency(F4, 82);
  _delay_ms(26);
  frequency(C5_, 503);
  _delay_ms(144);
  frequency(D5_, 82);
  _delay_ms(26);
  frequency(A4_, 82);
  _delay_ms(26);
  frequency(G4, 82);
  _delay_ms(26);
  frequency(D5_, 82);
  _delay_ms(26);
  frequency(A4_, 82);
  _delay_ms(26);
  frequency(G4, 82);
  _delay_ms(26);
  frequency(D5_, 125);
  _delay_ms(37);
  frequency(D5_, 39);
  _delay_ms(14);
  frequency(D5_, 39);
  _delay_ms(14);
  frequency(D5_, 39);
  _delay_ms(14);
  frequency(F5, 39);
  _delay_ms(69);
  frequency(F5, 39);
  _delay_ms(69);
  frequency(F5, 39);
  _delay_ms(69);
  frequency(G5, 505);
}

void sound::beep(const uint16_t duration) { frequency(G4, duration); }

void sound::test() {
  sound::init();

  sound::fail();
  _delay_ms(1000);
  sound::success();
  _delay_ms(1000);
  sound::crazyfrog();
}
