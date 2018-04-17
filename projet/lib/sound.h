#ifndef SOUND_H
#define SOUND_H

/**
 * Jouer un son de façon logiciel. Cette fonctionnalitée n'est pas utilisé dans
 * le projet final.
 */

#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>

// Déclaration des diverses notes sur plusieurs octaves
#define B6 236
#define A6_ 251
#define A6 268
#define G6_ 286
#define G6 305
#define F6_ 324
#define F6 348
#define E6 371
#define D6_ 394
#define D6 419
#define C6_ 446
#define C6 474
#define B5 507
#define A5_ 538
#define A5 570
#define G5_ 608
#define G5 645
#define F5_ 686
#define F5 729
#define E5 774
#define D5_ 822
#define D5 875
#define C5_ 928
#define C5 986
#define B4 1046
#define A4_ 1110
#define A4 1178
#define G4_ 1250
#define G4 1327
#define F4_ 1408
#define F4 1493
#define E4 1584
#define D4_ 1681
#define D4 1783
#define C4_ 1890
#define C4 2005

namespace sound {

/**
 * Initialiser la pin de son
 * @return void
 */
void init();

/**
 * Jouer une mélodie de succès
 * @return void
 */
void success();

/**
 * Jouer une mélodie d'échec
 * @return void
 */
void fail();

/**
 * Jouer la chanson thème de crazy frog
 * @return void
 */
void crazyfrog();

/**
 * Faire un bip sonore
 * @param Durée en ms de la tonalité
 * @return void
 */
void beep(const uint16_t duration);

/**
 * Tester toutes les fonctions de son
 * @return void
 */
void test();

}  // namespace sound

#endif