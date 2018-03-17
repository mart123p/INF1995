/*
 * Uploader de pseudo-code binaire vers la carte RS-232
 *
 * Créé par
 * Cédric Tessier (1907831)
 * Martin Pouliot (1900045)
 * Pascal-Alexandre Morel (xxxxxxx)
 * Sean Costello (xxxxxxx)
 *
 * 20 mars 2018
 *
 * Description du programme:
 * Programme permettant de lire en uart la liste des instructions
 * binaires envoyées par serieViaUSB et de les enregistrer en mémoire externe.
 *
 */
#include <avr/interrupt.h>
#include <avr/io.h>
#include <ohBoy.h>
mem::Memoire24CXXX memoire;
int main() {
  uart::init();
  light::init();

  // Initialisation des interupts
  DDRD &= ~(2 << DDD2);
  EICRA |= (1 << ISC00);
  EIMSK |= (1 << INT0);
  // Activation des interrupts
  sei();

  //Écriture des données dans la mémoire
  uint8_t data = 0;
  uint16_t address = 0;
  light::red();
  while (true) {
    data = uart::readData();
    light::green();
    memoire.ecriture(address, data);
    address++;
  }
}
// Routine d'interruption pour INT0
// Lecture des données écrites dans la mémoire
ISR(INT0_vect) {
  _delay_ms(5);
  if (PIND & 0x04) {
    light::red();
    uint8_t data = 0;
    for (uint16_t i = 0; i < 65535; i++) {
      memoire.lecture(i, &data);
      uart::print(data);
      uart::print(" ");
      if (i % 2 == 1) uart::println();
    }
    while (true)
      ;
  }
}
