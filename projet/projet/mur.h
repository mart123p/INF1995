#ifndef MUR_H
#define MUR_H

#include "ohBoy.h"
#include "tache.h"
#include "state.h"

class Mur : Tache{
public:
    /**
     * Constructeur par défaut
     */
    Mur();

    /**
     * Scrute les deux côtés du robot à la recherche de murs pour changer de côté
     * @param sensor    Permet d'accéder au distance du robot par rapport au mur
     * @param state     Permet d'afficher l'état du robot
     * @param lastState Donne au robot le dernier état dans lequel il était
     */
    void scrutation(Sensor& sensor, State& state, State& lastState);

    /**
     * Remet le robot en état de changer de mur
     */
    void reset();

private:    
    uint8_t tick;
    bool canSwitchWall;
};
#endif