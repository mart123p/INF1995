#include "parcours.h"

Parcours::Parcours() : ajustement(&sensor) {
  state = READY;
  lastState = READY;
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
        if (sensor.getValSensor0() > vide_0){
          state = BIG_TURN_1;
        }else if (sensor.getValSensor0() > 27) {
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
  // BIGTURN0
  pwm::set1(defaultSpeed);
  pwm::set0(defaultSpeed);
  _delay_ms(2500);
  if (sensor.read0() > vide_0) {
    pwm::set1(80);
    pwm::set0(-80);
    _delay_ms(300);
    pwm::set1(defaultSpeed);
    pwm::set0(defaultSpeed);
    _delay_ms(1000);
  }
}

void Parcours::virage90_1() {
  // BIGTURN1
  pwm::set1(defaultSpeed);
  pwm::set0(defaultSpeed);
  _delay_ms(2500);
  if (sensor.read1() > vide_1) {
    pwm::set1(-80);
    pwm::set0(80);
    _delay_ms(300);
    pwm::set1(defaultSpeed);
    pwm::set0(defaultSpeed);
    _delay_ms(1000);
  }
  soundpwm::off();
}
