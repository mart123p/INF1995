#include "parcours.h"

Parcours* Parcours::pThis;

Parcours::Parcours() : mur(&poteau), ajustement(&sensor) {
  state = READY;
  lastState = READY;
  compteurBigTurn = 0;
  effectueVirage90 = false;

  // Activation de l'interruption du bouton pour
  // faire la rotation du 180.
  DDRD &= ~(1 << PD2);    // Activer la pin PD2
  EICRA |= (1 << ISC00);  // Mettre INT0 de tel sorte qu'il reagit en
                          // fonction de n'importe quel changement logique.
  EIMSK |= (1 << INT0);   // Active INT0
  sei();

  pThis = this;  // On utilise cet attribut pour pouvoir interagir avec
  // la classe lors de l'interrupt
}

void Parcours::exec() {
  // Initialisation du code
  soundpwm::init();
  light::green();
  bool isRunning = true;

  uart::parcoursDebug(sensor, state, "====init====");
  uart::parcoursDebug(sensor, state, "====init====");

  while (isRunning) {
    sensor.read0();
    sensor.read1();

    uart::parcoursDebug(sensor, state, "Begin");

    // Execution des taches d'arriere plan
    mur.scrutation(sensor, state, lastState);
    // Cette fonction peut changer l'etat de la machine
    // A cet etat, il faut donc faire attention.
    poteau.scrutation(sensor, state, lastState);
    // Fin des taches de scrutation.

    // Debut de la machine a etat
    switch (state) {
      case READY:
        if (sensor.getValSensor0() > sensor.getValSensor1()) {
          // Determiner quel mur suivre
          state = WALL_1;
        } else {
          state = WALL_0;
        }
        for (uint8_t i = 0; i < 20; i++) {
          _delay_ms(50);
          sensor.read0();
          sensor.read1();
        }
        // Attente de une seconde avant de commencer
        // à rouler pour ne pas changer dans un mauvais
        // état
        pwm::set1(defaultSpeed);
        pwm::set0(defaultSpeed);
        break;

      case SWITCH_WALL:
        changeWall();
        break;

      case WALL_0:
        if (sensor.getValSensor0() > 45) {
          state = BIG_TURN_0;
        } else if (sensor.getValSensor0() > 27) {
          state = GROS_AJUSTEMENT_0;
          lastState = WALL_0;
        } else {
          ajustement.ajuste0();  // Rester entre 13 et 16cm
        }
        break;

      case WALL_1:
        if (sensor.getValSensor1() > 45) {
          state = BIG_TURN_1;
        } else if (sensor.getValSensor1() > 27) {
          state = GROS_AJUSTEMENT_1;
          lastState = WALL_1;
        } else {
          ajustement.ajuste1();
        }
        break;

      case GROS_AJUSTEMENT_0:
        if (ajustement.grosAjustement0(state)) {
          ajustement.reset0();
          state = WALL_0;
        }
        break;

      case GROS_AJUSTEMENT_1:
        if (ajustement.grosAjustement1(state)) {
          ajustement.reset1();
          state = WALL_1;
        }
        break;

      case BIG_TURN_0:
        virage180_0();
        break;

      case BIG_TURN_1:
        virage180_1();
        break;
    }
    _delay_ms(50);
  }
}

void Parcours::changeWall() {
  uart::parcoursDebug(sensor, state, "changewall");
  ajustement.neDoitPasAttendre();
  if (lastState == WALL_0) {
    state = GROS_AJUSTEMENT_1;
  }
  if (lastState == WALL_1) {
    state = GROS_AJUSTEMENT_0;
  }
}

void Parcours::virage180_0() {
  light::red();

  if (ajustement.didCorrectionFrein0()) {
    // On doit reajuster avant de faire le bigturn
    uart::parcoursDebug(sensor, state, "BIGTURN0 Fix Ajustement");
    pwm::set1(50);
    pwm::set0(-50);
    _delay_ms(500);
  }

  while (state != WALL_0) {
    // On fait tourner les roues et on active la scrutation des capteurs
    // tant que l'on ne capte pas a nouveau le mur.
    pwm::set0(40);
    pwm::set1(80);

    uart::parcoursDebug(sensor, state, "BIGTURN 0 (IN LOOP)");
    sensor.read1();
    sensor.read0();
    poteau.scrutation(sensor, state, lastState);
    _delay_ms(50);
    if (sensor.getValSensor0() < 21) {
      state = WALL_0;
    }
  }
  uart::parcoursDebug(sensor, state, "BIGTURN 0 (OUT LOOP)");
}

void Parcours::virage180_1() {
  light::red();
  if (ajustement.didCorrectionFrein1()) {
    // On doit réajuster avant de faire le bigturn
    uart::parcoursDebug(sensor, state, "BIGTURN1 Fix Ajustement");
    pwm::set0(50);
    pwm::set1(-50);
    _delay_ms(500);
  }

  while (state != WALL_1) {
    // On fait tourner les roues et on active la scrutation des capteurs
    // tant que l'on ne capte pas a nouveau le mur.
    pwm::set0(47);
    pwm::set1(34);

    uart::parcoursDebug(sensor, state, "BIGTURN 1 (IN LOOP)");
    sensor.read1();
    sensor.read0();
    poteau.scrutation(sensor, state, lastState);

    _delay_ms(50);

    if (sensor.getValSensor1() < 19) {
      state = WALL_1;
    }
  }
  uart::parcoursDebug(sensor, state, "BIGTURN 1 (OUT LOOP)");
}

void Parcours::interrupt180() {
  if (pThis->poteau.getDetected()) {
    pThis->poteau.cancelBeep();
    soundpwm::off();
  }

  pThis->mur.reset();

  pwm::set0(0);
  pwm::set1(0);
  _delay_ms(500);  // Le temps que le robot arrête

  pwm::set0(-80);
  pwm::set1(80);
  _delay_ms(850);  // 1100 batteries faibles, 800 nouvelles batteries

  pwm::set0(-40);
  pwm::set1(40);  // Un frein pour arreter l'inertie
  _delay_ms(200);

  pwm::set0(0);
  pwm::set1(0);  // Un autre frein pour mesure de securite
  _delay_ms(500);

  pwm::set0(defaultSpeed);
  pwm::set1(defaultSpeed);

  switch (pThis->state) {
    case GROS_AJUSTEMENT_1:
      pThis->state = GROS_AJUSTEMENT_0;
      break;
    case GROS_AJUSTEMENT_0:
      pThis->state = GROS_AJUSTEMENT_1;
      break;
    case WALL_0:
      pThis->state = WALL_1;
      break;
    case WALL_1:
      pThis->state = WALL_0;
      break;
    default:
      pThis->state = READY;
      break;
  }
}

// Interruption pour faire le 180
// lorsque le bouton est enfoncé
ISR(INT0_vect) {
  _delay_ms(5);
  if (PIND & 0x04) {
    Parcours::interrupt180();
  }
}