#include "parcours.h"
#define defaultSpeed 40
#define vide_0 45
#define vide_1 45
#define borneInfAjustement 13
#define borneSupAjustement 20

// LE BON

Parcours::Parcours() {
  lastValue0 = 0;
  lastValue1 = 0;
  currentValue0 = 0;
  currentValue1 = 0;
}

void Parcours::exec() {

  soundpwm::init();
  state = READY;
  lastState = READY;
  light::green();
  canSwitchWall = true;
  bool isRunning = true;
  while (isRunning) {
    currentValue0 = sensor.read0();
    currentValue1 = sensor.read1();

    uart::parcoursDebug(currentValue0, currentValue1, state, "Top");

    wallScrutation();

    switch (state) {
    case READY:
      if (currentValue0 > currentValue1) { // Determiner quel
        state = WALL_1;                    // mur suivre
      } else {
        state = WALL_0;
      }
      pwm::set1(defaultSpeed);
      pwm::set0(defaultSpeed);
      break;
/*
    case SWITCH_WALL:
      changeWall();
      break;
*/
    case WALL_0:
      if (currentValue0 > vide_0) // Si trop loin au mur, tourner
        uart::parcoursDebug(currentValue0, currentValue1, state, "state = BIG_TURN_0");
        //state = BIG_TURN_0;
      else {
        ajustement0(); // Pour rester entre 13 et 16 cm
      }                // de distance.
      break;
    /*
    case BIG_TURN_0:
      virage90_0();
      state = WALL_0;
      break;
	*/
    case WALL_1:
      if (currentValue1 > vide_1) {
         uart::parcoursDebug(currentValue0, currentValue1, state, "state = BIG_TURN_1");
        //state = BIG_TURN_1;
      }
      ajustement1();
      break;
/*
    case BIG_TURN_1:
      virage90_1();
      state = WALL_1;
      break;
  */
    }
    lastValue0 = currentValue0;
    lastValue1 = currentValue1;
    _delay_ms(50);
  }
}

void Parcours::wallScrutation() {
  if (canSwitchWall && state == WALL_0 && sensor.read1() < vide_1) {
     uart::parcoursDebug(currentValue0, currentValue1, state, "state = SWITCH_WALL");
    //state = SWITCH_WALL;
    lastState = WALL_0;
  }

  if (canSwitchWall && state == WALL_1 && sensor.read0() < vide_0) {
     uart::parcoursDebug(currentValue0, currentValue1, state, "state = SWITCH_WALL");
	//state = SWITCH_WALL;
    lastState = WALL_1;
  }
}

void Parcours::changeWall() {
  if (lastState == WALL_0) {
    grosAjustement1();
  }
  if (lastState == WALL_1) {
    grosAjustement0();
  }
}

// Permet d'aller vers le mur 0
void Parcours::grosAjustement0() {
  canSwitchWall = false;
  if (currentValue0 > borneSupAjustement) {
    uart::parcoursDebug(currentValue0, currentValue1, state, "grosAjustement0");
    // Le robot tourne vers le mur 0
    light::red();
    pwm::set1(defaultSpeed);
    pwm::set0(-40);
    _delay_ms(1000);

    // Le robot avance pendant une seconde
    pwm::set1(defaultSpeed);
    pwm::set0(defaultSpeed);
    _delay_ms(1000);

    // le robot se replace droit
    pwm::set1(-40);
    pwm::set0(defaultSpeed);
    _delay_ms(1000);

  }

  else {
    pwm::set1(defaultSpeed);
    pwm::set0(defaultSpeed);
    state = WALL_0;
  }
}

// Permet d'aller vers le mur 1
void Parcours::grosAjustement1() {
  canSwitchWall = false;
  if (currentValue1 > borneSupAjustement) {
    uart::parcoursDebug(currentValue0, currentValue1, state, "grosAjustement1");

    // Le robot tourne vers le mur 1
    light::red();
    pwm::set0(defaultSpeed);
    pwm::set1(-40);
    _delay_ms(1000);

    // Le robot avance pendant une seconde
    pwm::set0(defaultSpeed);
    pwm::set1(defaultSpeed);
    _delay_ms(1000);

    // le robot se replace droit
    pwm::set0(-40);
    pwm::set1(defaultSpeed);
    _delay_ms(1000);
  }

  else {
    pwm::set0(defaultSpeed);
    pwm::set1(defaultSpeed);
    state = WALL_1;
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

  if (currentValue0 > 16 && currentValue0 <= borneSupAjustement) {
    //Entre 16 et 20 ajustement
    uart::parcoursDebug(currentValue0, currentValue1, state, "Ajustement0 if1");
    light::red();
    pwm::set1(60);
    if (currentValue0 > lastValue0) {
    	//Freine si il y a une variation de distance
      uart::parcoursDebug(currentValue0, currentValue1, state,
                          "Ajustement0 if2");
      pwm::set0(-40);
    } else {
      uart::parcoursDebug(currentValue0, currentValue1, state,
                          "Ajustement0 else1");
      pwm::set0(defaultSpeed);
    }
  } else if (currentValue0 < borneInfAjustement) {
    //Quand le robot est trop pres du mur  
    uart::parcoursDebug(currentValue0, currentValue1, state,
                        "Ajustement0 else if");
    light::red();
    pwm::set0(60);
    // Mur à 90 le virage doit etre plus seree
    if (currentValue0 < lastValue0) {
      uart::parcoursDebug(currentValue0, currentValue1, state,
                          "Ajustement0 if3");
      pwm::set1(-40);
    } else {
      uart::parcoursDebug(currentValue0, currentValue1, state,
                          "Ajustement0 else2");
      pwm::set1(defaultSpeed);
    }
  } else if (currentValue0 < borneSupAjustement){
    uart::parcoursDebug(currentValue0, currentValue1, state,
                        "Ajustement0 else3");
    light::green();
    pwm::set1(defaultSpeed);
    pwm::set0(defaultSpeed);
  }
}

void Parcours::ajustement1() {
  uart::parcoursDebug(currentValue0, currentValue1, state, "Ajustement1");
  if (currentValue1 > 16 && currentValue1 < borneSupAjustement) {
    light::red();
    pwm::set0(60);
    if (currentValue1 > lastValue1) {
      pwm::set1(-40);
    } else {
      pwm::set1(defaultSpeed);
    }
  } else if (currentValue1 < borneInfAjustement) {
    light::red();
    pwm::set1(60);
    // Mur à 90 le virage doit etre plus seree
    if (currentValue1 < lastValue1) {
      pwm::set0(-40);
    } else {
      pwm::set0(defaultSpeed);
    }
  } else {
    uart::parcoursDebug(currentValue0, currentValue1, state,
                        "Ajustement1 else");
    light::green();
    pwm::set1(defaultSpeed);
    pwm::set0(defaultSpeed);
  }
}
