#ifndef MUR_H
#define MUR_H

#include "ohBoy.h"
#include "tache.h"
#include "state.h"

class Mur : Tache{
public:
    Mur();
    void scrutation(Sensor& sensor, State& state, State& lastState);
    void reset();
private:    
    uint8_t tick;
    bool canSwitchWall;
};
#endif