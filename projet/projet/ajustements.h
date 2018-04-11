#ifndef AJUSTEMENTS_H
#define AJUSTEMENTS_H
#include "ohBoy.h"
#include "state.h"

class Ajustement{
    public:
       /**
        * Constructeur 
        */
        Ajustement(Sensor* sensor);
        
        /**
         * permet un ajustement sur le mur 0 à courte distance
         */
        void ajuste0();
        
        /**
         * permet un ajustement sur le mur 1 à courte distance
         */
        void ajuste1();
        
        /**
         * Permet un un ajustement sur le mur 0 à longue distance
         * @param  state Permet d'afficher l'état
         * @return       bool indiquand si l'ajustement est ok ou si il doit se réajuster
         */
        bool grosAjustement0(State state);
        

        /**
         * Permet un un ajustement sur le mur 1 à longue distance
         * @param  state Permet d'afficher l'état
         * @return       bool indiquand si l'ajustement est ok ou si il doit se réajuster
         */
        bool grosAjustement1(State state);
    private:
        Sensor* sensor;
        bool grosAjustement0IsAjusted;
        bool grosAjustement1IsAjusted;

};

#endif