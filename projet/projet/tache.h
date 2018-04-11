#ifndef TACHE_H
#define TACHE_H
#include "ohBoy.h"
#include "state.h"
class Tache{
    public:
       	virtual void scrutation(Sensor& sensor,State& state, State& lastState);
};
#endif