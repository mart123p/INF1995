#ifndef SOUNDPWM_H
#define SOUNDPWM_H

/**
 * Création de son à l'aide d'un signal PWM.
 */

#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
namespace soundpwm {

/**
 * Initialisation du son PWM et des registres nécessaires pour le bon 
 * fonctionnement du son.
 * @return void
 */
void init();

/**
 * Jouer une tonalité sonore
 * @param midi note
 * @return void
 */
void beep(const uint8_t note);

/**
 * Arrêter le son PWM
 * @return void
 */
void off();
/**
 * Tester toutes les fonctions PWM
 * @return void
 */
void test();

}  // namespace sound

#endif
