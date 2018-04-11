#ifndef STATE_H
#define STATE_H

#define defaultSpeed 40
#define frein -40
#define acceleration 50

#define vide_0 60
#define vide_1 60

/**
 * États de la machine à état
 */
enum State {WALL_0,BIG_TURN_0, SWITCH_WALL, WALL_1, BIG_TURN_1, READY, GROS_AJUSTEMENT_0, GROS_AJUSTEMENT_1}; //Rajouter des etats a la fin

#endif