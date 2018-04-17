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
         * Permet au robot de rester à une distance de 15 cm du mur
         * lorsqu'il suit un mur situé à sa gauche.
         */
        void ajuste0();
        
        /**
         * Permet au robot de rester à une distance de 15 cm du mur
         * lorsqu'il suit un mur situé à sa droite.
         */
        void ajuste1();
        
        /**
         * Permet un ajustement sur le mur 0 à longue distance
         * Cette méthode est principalement utilisée lorsqu'on doit changer de mur
         * @param  state Permet d'afficher l'état
         * @return       bool indiquand si l'ajustement est ok ou si il doit se réajuster
         */
        bool grosAjustement0(State state);
        

        /**
         * Permet un ajustement sur le mur 1 à longue distance
         * Cette méthode est principalement utilisée lorsqu'on doit changer de mur
         * @param  state Permet d'afficher l'état
         * @return       bool indiquand si l'ajustement est ok ou si il doit se réajuster
         */
        bool grosAjustement1(State state);

         /**
         * Met l'attribut grosAjustement1IsAjusted à false 
         * pour permettre au robot de tourner quand il tombe dans l'état de 
         * changement de mur
         */
        void reset0();

         /**
         * Met l'attribut grosAjustement0IsAjusted à false 
         * pour permettre au robot de tourner quand il tombe dans l'état de 
         * changement de mur
         */
        void reset1();


         /**
         * Permet de vérifier si le robot a corrigé sa position avant
         * qu'il effectue un déplacement comme faire un virage de 180 degrés
         */
        bool didCorrectionFrein0();

         /**
         * Permet de vérifier si le robot a corrigé sa position avant
         * qu'il effectue un déplacement comme faire un virage de 180 degrés
         */
        bool didCorrectionFrein1();

        /**
         * Met à false doitAttendre
         */
        void neDoitPasAttendre();


    private:
        Sensor* sensor;
        bool grosAjustement0IsAjusted;
        bool grosAjustement1IsAjusted;

        bool correctionFrein0; //Utilise dans bigturn
        bool correctionFrein1; //Utilise dans bigturn

        bool doitAttendre;
        uint8_t tick;

};

#endif