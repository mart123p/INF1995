#ifndef AJUSTEMENTS_H
#define AJUSTEMENTS_H
#include "ohBoy.h"
#include "state.h"

class Ajustement{
    public:
        Ajustement(Sensor* sensor);
        
        void ajuste0();
        void ajuste1();
        
        bool grosAjustement0(State state);
        bool grosAjustement1(State state);
    private:
        Sensor* sensor;
        bool grosAjustement0IsAjusted;
        bool grosAjustement1IsAjusted;

};

#endif