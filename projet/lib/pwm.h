#ifndef PWM_H
#define PWM_H

/**
 * Contrôle des signaux envoyés aux moteurs. Un signal PWM est utilisé pour 
 * faire ce contrôle.
 */
#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
namespace pwm {

/**
 * Activation des registres pour faire un PWM pour les moteurs
 * @return void
 */
void init();

/**
 * Activer le pwm pour le moteur 1. Utilisation d'un pourcentage pour le 
 * contrôle
 * @param signal (-100 to 100)
 * @return void
 */
void set1(int8_t signal);

/**
 * Activer le pwm pour le moteur 0. Utilisation d'un pourcentage pour le 
 * contrôle
 * @param signal (-100 to 100)
 * @return void
 */
void set0(int8_t signal);

/**
 * Fermer le PWM pour les deux roues
 * @return void
 */ 
void off();

/**
 * Tester les différentes fonction PWM
 * @return void
*/
void test();
}  // namespace pwm
#endif
