#ifndef TIMER_H
#define TIMER_H
/**
 * Fonctions pour simplifier l'utilisation du timer 1. Cette fonctionnalité ne
 * peut pas être utilisé en même temps que le PWM pour les roues.
 * Par conséquent, elle n'est pas utilisé pour dans le projet final.
 */

#include <avr/interrupt.h>
#define F_CPU 8000000
#include <util/delay.h>

namespace timer {

/**
 * Initialiser le timer 1
 * @param ms délai entre chaque intéruption
 * @return void
 */
void init(uint16_t ms);

/**
 * Activer le timer 1
 * @return void
 */
void on();

/**
 * Désactiver le timer 1 et les intéruptions
 * @return void
 */
void off();

void delay(uint16_t time);

/**
 * Tester toutes les fonctionnalités du capteur 1
 * @return void
 */
void test(uint16_t ms);

}  // namespace timer

#endif
