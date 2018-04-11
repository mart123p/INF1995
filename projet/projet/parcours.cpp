#include "parcours.h"

Parcours::Parcours() : ajustement(&sensor) {
  state = READY;
  lastState = READY;
  compteurBigTurn =0;
  effectueVirage90 = false;
}

void Parcours::exec() {
  // Initialisation du code
  soundpwm::init();
  light::green();
  bool isRunning = true;

  uart::parcoursDebug(sensor, state, "init");

  while (isRunning) {
    sensor.read0();
    sensor.read1();

    uart::parcoursDebug(sensor, state, "Begin");

    //Execution des taches d'arriere plan
    mur.scrutation(sensor,state,lastState); //Cette fonction peut changer l'etat de la machine
                                            //A etat, il faut donc faire attention.
    poteau.scrutation(sensor, state,lastState);
    //Fin des taches de scrutation.


    //Debut de la machine a etat
    switch (state) {
      case READY:
        if (sensor.getValSensor0() > sensor.getValSensor1()) {  // Determiner quel
          state = WALL_1;              // mur suivre
        } else {
          state = WALL_0;
        }
        for (uint8_t i = 0; i < 20; i++) {
          _delay_ms(50);
          sensor.read0();
          sensor.read1();
        }  // Attente de une seconde avant de commencer

        // à rouler pour ne pas changer dans un mauvais
        // état
        pwm::set1(defaultSpeed);
        pwm::set0(defaultSpeed);
        break;

      case SWITCH_WALL:
        changeWall();
        break;

      case WALL_0:
        if (sensor.getValSensor0() > vide_0){
          state = BIG_TURN_0;
        }else if (sensor.getValSensor0() > 27) {
          state = GROS_AJUSTEMENT_0;
          lastState = WALL_0;
        }else{ 
          ajustement.ajuste0(); //Rester entre 13 et 16cm
        }
        break;

      case WALL_1:
        if (sensor.getValSensor1() > vide_0){
          state = BIG_TURN_1;
        }else if (sensor.getValSensor1() > 27) {
          state = GROS_AJUSTEMENT_1;
          lastState = WALL_1;
        }else{ 
          ajustement.ajuste1();
        }
        break;

      case GROS_AJUSTEMENT_0:
          if(ajustement.grosAjustement0(state)){
            state = WALL_0;
          } 
        break;

      case GROS_AJUSTEMENT_1:
          if(ajustement.grosAjustement1(state)){
            state = WALL_1;
          }
        break;

      case BIG_TURN_0:
        virage90_0();
        break;

      case BIG_TURN_1:
        virage90_1();
        break;
    }
    _delay_ms(50);
  }
}

void Parcours::changeWall() {
  uart::parcoursDebug(sensor, state, "changewall");
  if (lastState == WALL_0) {
    mur.reset();
    state = GROS_AJUSTEMENT_1;
  }
  if (lastState == WALL_1) {
    mur.reset();
    state = GROS_AJUSTEMENT_0;  
  }
}

void Parcours::virage90_0() {

//bool effectueVirage90 = false; // On veut que le robot avance un peu avant de tourner pour ne pas foncer dans le mur
  uart::parcoursDebug(sensor, state, "state = BIGTURN1");
  if (compteurBigTurn >= 250 && !effectueVirage90) {   // Attend un certain temps, puis permet au robot de tourner
    effectueVirage90 = true;
    compteurBigTurn = 0;                                     // On reset le compteur pour l'utiliser pour faire le virage
  }

  if (effectueVirage90) {                 
    pwm::set1(80);
    pwm::set0(-80);                 // Ajuster la vitesse des roues pendant un certain temps pour tourner
    if (compteurBigTurn >= 30)             // ration : 300/2500 * 25 = 30
      effectueVirage90 = false;
  }

  compteurBigTurn ++; 
}


void Parcours::virage90_1() {

//bool effectueVirage90 = false; // On veut que le robot avance un peu avant de tourner pour ne pas foncer dans le mur
  uart::parcoursDebug(sensor, state, "state = BIGTURN1");
  if (compteurBigTurn >= 250 && !effectueVirage90) {   // Attend un certain temps, puis permet au robot de tourner
    effectueVirage90 = true;
    compteurBigTurn = 0;                                     // On reset le compteur pour l'utiliser pour faire le virage
  }

  if (effectueVirage90) {                 
    pwm::set0(80);
    pwm::set1(-80);                 // Ajuster la vitesse des roues pendant un certain temps pour tourner
    if (compteurBigTurn >= 30)             // ration : 300/2500 * 25 = 30
      effectueVirage90 = false;
  }

  compteurBigTurn ++; 
}
