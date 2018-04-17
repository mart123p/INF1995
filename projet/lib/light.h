#ifndef LIGHT_H
#define LIGHT_H

/**
 * Fonctions pour contrôler une ou des lumières sur le robot.
 * Permet de contrôler la DEL principale en plus de DEL individuels que l'on <
 * peut brancher sur le robot.
*/ 


#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>

/**
 * Cable rouge sur la pin 4 et sur le + du port C
 * pour respecter les couleurs
 */

namespace light {


/**
 * Mettre le port de la DEL en sortie
 * @return void
 */
void init();


/**
 * Mettre la couleur vert sur la DEL
 * @return void
 */
void green();


/**
 * Mettre la couleur de la DEL à rouge
 * @return void
 */
void red();


/**
 * Mettre la couleur de la DEL à ambre
 * @return void
 */
void amber(uint16_t ms);


/**
 * Mettre une pin à on pour faire alumer une DEL
 * @param pin
 * @return void
 */
void on(uint8_t pin);



/**
 * Changer la couleur de la DEL ou fermer la DEL en question
 * @param donnee
 * @return void
 */
void controleDeLaDel(uint8_t donnee);



/**
 * Fermer la DEL principale
 * @return void
 */
void off();


/**
 * Fermer la DEL de la pin en question
 * @param pin
 * return void
 */
void off(uint8_t pin);


/**
 * Tester les fonctions de la DEL
 * @return void
 */
void test();
}  // namespace light
#endif
