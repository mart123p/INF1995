#include "parcours.h"

#define defaultSpeed 50
#define frein -50
#define acceleration 70

#define correctionIncertitude 1
#define vide_0 60
#define vide_1 60
#define borneInfAjustement 13
#define borneSupAjustement 35

// LE BON

Parcours::Parcours() {
  lastValue0 = 0;
  lastValue1 = 0;
  currentValue0 = 0;
  currentValue1 = 0;
  tick = 0;
}

void Parcours::exec() {

  soundpwm::init();
  state = READY;
  lastState = READY;
  light::green();
  canSwitchWall = true;
  bool isRunning = true;
  uart::parcoursDebug(sensor, state, "init");
  while (isRunning) {
    currentValue0 = sensor.read0();
    currentValue1 = sensor.read1();

    uart::parcoursDebug(sensor, state, "Top");

    wallScrutation();

    switch (state) {
    case READY:
      if (currentValue0 > currentValue1) { // Determiner quel
        state = WALL_1;                    // mur suivre
      } else {
        state = WALL_0;
      }
      _delay_ms(1000); // Attente de une seconde avant de commencer
                       // à rouler pour ne pas changer dans un mauvais
                       // état
      pwm::set1(defaultSpeed);
      pwm::set0(defaultSpeed);
      break;

    case SWITCH_WALL:
      changeWall();
      break;

    case WALL_0:
      /*if (currentValue0 > vide_0) // Si trop loin au mur, tourner
        uart::parcoursDebug(sensor, state, "state = BIG_TURN_0");
      // state = BIG_TURN_0;
      else {*/
        ajustement0(); // Pour rester entre 13 et 16 cm
      //}                // de distance.
      break;
    /*
    case BIG_TURN_0:
      virage90_0();
      state = WALL_0;
      break;
        */
    case WALL_1:
     /* if (currentValue1 > vide_1) {
        uart::parcoursDebug(sensor, state, "state = BIG_TURN_1");
        // state = BIG_TURN_1;
      } else {*/
        ajustement1();
      //}
      break;
      case BIG_TURN_1:
        virage90_1();
        state = WALL_1;
      break;
    }
    lastValue0 = currentValue0;
    lastValue1 = currentValue1;
    _delay_ms(50);
  }
}

bool Parcours::wallScrutation() {
  bool result = false;
  if (canSwitchWall && state == WALL_0 && sensor.read1() < vide_1) {
    if (tick > 5) {
      uart::parcoursDebug(sensor, state, "state = SWITCH_WALL");
      state = SWITCH_WALL;
      lastState = WALL_0;
      result = true;
      tick = 0;
    }
    tick++;
  } else if (canSwitchWall && state == WALL_1 && sensor.read0() < vide_0) {
    if (tick > 5) {
      uart::parcoursDebug(sensor, state, "state = SWITCH_WALL");
      state = SWITCH_WALL;
      lastState = WALL_1;
      result = true;
      tick = 0;
    }
    tick++;
  } else {
    tick = 0;
  }

  //Detection si le robot a le droit de changer de cote
  if(!canSwitchWall){
    if(state == WALL_0){
      if(sensor.read0() > vide_0){
        if(tick > 5){
          tick =0;
          canSwitchWall = true;
          uart::parcoursDebug(sensor, state, "canSwitchWall true");
        }
        tick++;
      }else{
        tick = 0;
      }
    }else if(state == WALL_1){
      if(sensor.read1() > vide_1){
        if(tick > 5){
          tick =0;
          canSwitchWall = true;
          uart::parcoursDebug(sensor, state, "canSwitchWall true");
        }
        tick++;
      }else{
        tick = 0;
      }
    }
  }

  return result;
}

void Parcours::changeWall() {
  uart::parcoursDebug(sensor, state, "changewall");
  if (lastState == WALL_0) {
    grosAjustement1();
    canSwitchWall = false;
    state = WALL_1;
  }
  if (lastState == WALL_1) {
    grosAjustement0();
    canSwitchWall = false;
    state = WALL_0;
  }
}

// Permet d'aller vers le mur 0
void Parcours::grosAjustement0() {
  if (currentValue0 > borneSupAjustement) {
    uart::parcoursDebug(sensor, state, "grosAjustement0");
    // Le robot tourne vers le mur 0
    light::red();
    pwm::set1(defaultSpeed);
    pwm::set0(frein);

    //Adjust in consequence if the wall is further the attack angle
    //should be bigger
    uint16_t angle = 0;
    if (currentValue0 > 50) {
      uart::parcoursDebug(sensor, state, "grosAjustement0 if1");
      angle = 800;
    } else if (currentValue0 > 40) {
      uart::parcoursDebug(sensor, state, "grosAjustement0 if2");
      angle = 700;
    } else {
      uart::parcoursDebug(sensor, state, "grosAjustement0 if3");
      angle = 600;
    }
    timer::delay(angle);

    // Le robot avance pendant une seconde
    while (sensor.read0() > 15) {
      pwm::set1(defaultSpeed); //TODO s'assurer qu'il suit une ligne droite
      pwm::set0(defaultSpeed); //Pas moyen de s'assurer qu'il suit une ligne droite
      _delay_ms(50);
      uart::parcoursDebug(sensor, state, "grosAjustement0 forward");
    }

    // le robot se replace droit
    pwm::set1(frein);
    pwm::set0(defaultSpeed);
    timer::delay(angle);


  }
}

// Permet d'aller vers le mur 1
void Parcours::grosAjustement1() {
  if (currentValue1 > borneSupAjustement) {
    uart::parcoursDebug(sensor, state, "grosAjustement1");

    // Le robot tourne vers le mur 1
    light::red();
    pwm::set0(defaultSpeed);
    pwm::set1(frein);

    //Adjust in consequence if the wall is further the attack angle
    //should be bigger
    uint16_t angle = 0;
    if (currentValue1 > 50) {
      angle = 800;
    } else if (currentValue1 > 40) {
      angle = 700;
    } else {
      angle = 600;
    }
    timer::delay(angle);

    while (sensor.read1() > 17) {
      // Le robot avance pendant une seconde
      pwm::set0(defaultSpeed); //TODO s'assurer qu'il suit une ligne droite
      pwm::set1(defaultSpeed); //Pas moyen de s'assurer qu'il suit une ligne droite
      _delay_ms(50);
      uart::parcoursDebug(sensor, state, "grosAjustement1 forward");
    }

    // le robot se replace droit
    pwm::set0(frein);
    pwm::set1(defaultSpeed);
    timer::delay(angle);

  }
}

void Parcours::virage90_0() {

  pwm::set1(defaultSpeed);
  pwm::set0(defaultSpeed);
  soundpwm::beep(61);
  _delay_ms(2500);
  currentValue0 = sensor.read0();
  if (currentValue0 > vide_0) {
    pwm::set1(80);
    pwm::set0(-80);
    _delay_ms(300);
    pwm::set1(defaultSpeed);
    pwm::set0(defaultSpeed);
    _delay_ms(1000);
  }
  soundpwm::off();
}

void Parcours::virage90_1() {

  pwm::set1(defaultSpeed);
  pwm::set0(defaultSpeed);
  soundpwm::beep(61);
  _delay_ms(2500);
  currentValue1 = sensor.read0();
  if (currentValue1 > vide_1) {
    pwm::set1(-80);
    pwm::set0(80);
    _delay_ms(300);
    pwm::set1(defaultSpeed);
    pwm::set0(defaultSpeed);
    _delay_ms(1000);
  }
  soundpwm::off();
}
void Parcours::ajustement0() {
  if (currentValue0 > 27) {
    Parcours::grosAjustement0();
  } else if (currentValue0 > 16) {
    light::red();
    pwm::set1(acceleration);
    if (currentValue0 > lastValue0) {
      pwm::set0(frein);
    } else {
      pwm::set0(defaultSpeed);
    }
  } else if (currentValue0 < 13) {
    light::red();
    pwm::set0(acceleration);
    // Mur à 90 le virage doit etre plus seree
    if (currentValue0 < lastValue0) {
      pwm::set1(frein);
    } else {
      pwm::set1(defaultSpeed);
    }
  } else {
    light::green();
    pwm::set1(defaultSpeed);
    pwm::set0(defaultSpeed);
  }
}

void Parcours::ajustement1() {
  if (currentValue1 > 27) {
    Parcours::grosAjustement1();
  } else if (currentValue1 > 16) {
    light::red();
    pwm::set0(acceleration);

    if (currentValue1 > lastValue1) {
      pwm::set1(frein);
    } else {
      pwm::set1(defaultSpeed);
    }
  } else if (currentValue1 < 13) {
    light::red();
    pwm::set1(acceleration);
    // Mur à 90 le virage doit etre plus seree
    if (currentValue1 < lastValue1) {
      pwm::set0(frein);
    } else {
      pwm::set0(defaultSpeed);
    }
  } else {
    light::green();
    pwm::set1(defaultSpeed);
    pwm::set0(defaultSpeed);
  }
}
