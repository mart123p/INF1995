#include "poteau.h"
#define SENSOR_SIZE 18
#define DISTANCE 10
// Nombre de points qui doivent être dans le bas de la courbe
#define POTEAU_SIZE 3
#define vide 130

Poteau::Poteau() {
  soundpwm::init();
  detected = false;
  stopBeep = false;
  tick = 0;
}

bool Poteau::getDetected() { return detected; }

void Poteau::reset() {
  detected = false;
  stopBeep = false;
}

void Poteau::cancelBeep() { stopBeep = true; }

int8_t Poteau::abs(int8_t abs) {
  if (abs < 0) {
    return abs * -1;
  }
  return abs;
}

void Poteau::wasDetected() {
  // On garde en mémoire les anciennes valeurs
  uint8_t oldSpeed1 = OCR1A;
  uint8_t oldSpeed0 = OCR1B;

  pwm::set0(25);
  pwm::set1(25);

  detected = true;
  for (uint8_t i = 0; i < 3; i++) {
    soundpwm::beep(81);
    // Delay 200 ms
    for (uint8_t j = 0; j < 10; j++) {
      _delay_ms(20);
      if (stopBeep) {
        // Le bouton de 180 a ete appuye, on doit retourner
        break;
      }
    }
    soundpwm::off();
    if (i < 2) {
      for (uint8_t j = 0; j < 10; j++) {
        _delay_ms(10);
        if (stopBeep) {
          // Le bouton de 180 a ete appuye, on doit retourner
          break;
        }
      }
    }
    if (stopBeep) {
      // Le bouton de 180 a ete appuye, on doit retourner
      reset();
      break;
    }
  }
  reset();
  // On donne une chance au robot pour le faire avancer
  if ((oldSpeed0 + 40) > 255) {
    OCR1B = 255;
  } else {
    OCR1B = (oldSpeed0 + 40);
  }

  if ((oldSpeed1 + 40) > 255) {
    OCR1A = 255;
  } else {
    OCR1A = (oldSpeed1 + 40);
  }
}

void Poteau::scrutation(Sensor& sensor, State& state, State& lastState) {
  // On peut detecter le poteau a 60 on la avant derniere
  // valeur pour regarder le poteau.
  // La valeur actuelle peut etre dans le vide.

  // On fait le code 2 fois, une fois pour chaque cote des capteurs.
  if (state != READY && tick > 25) {
    for (uint8_t i = 0; i < 2; i++) {
      if (i == 0 && (state == WALL_0 || state == BIG_TURN_0 ||
                     state == GROS_AJUSTEMENT_0)) {
        continue;  // On ignore la scrutation du capteur 0 s'il est dans les
                   // états précédents
      }
      if (i == 1 && (state == WALL_1 || state == BIG_TURN_1 ||
                     state == GROS_AJUSTEMENT_1)) {
        continue;  // On ignore la scrutation du capteur 1 s'il est dans les
                   // états précédents
      }
      uint8_t* oldVals;

      // Choisis si on est sur le cote 0 ou 1 du robot.
      if (i == 0) {
        oldVals = sensor.getOldVals0();
      } else {
        oldVals = sensor.getOldVals1();
      }

      if (oldVals[1] <= vide) {
        // On peut aller jusqu'a 2 donnee en arriere pour savoir si je la
        // difference de -10 existe
        bool poteauMaybe = false;
        uint8_t pos = 1;
        for (pos = 1; pos < 2; pos++) {
          if (((int8_t)oldVals[0] - (int8_t)oldVals[pos]) > DISTANCE) {
            poteauMaybe = true;
            break;
          }
        }
        uint8_t posMin = pos;
        pos++;

        if (poteauMaybe) {
          uart::parcoursDebug(sensor, state, "Poteaux maybe");
          uart::parcoursDebug(sensor, state, i);

          bool under60 = false;
          bool bigEnough = false;
          uint8_t lowNumber = 0;
          for (; pos < SENSOR_SIZE; pos++) {
            if (oldVals[pos] <= 60) {
              under60 = true;
            }
            if (abs((int8_t)oldVals[pos] - (int8_t)oldVals[posMin]) <
                DISTANCE) {
              lowNumber++;
            }
            if (lowNumber >= POTEAU_SIZE) {
              bigEnough = true;
            }
            if ((int8_t)oldVals[pos] - (int8_t)oldVals[posMin] > DISTANCE &&
                under60 && bigEnough) {
              // POTEAUX!!!!
              tick = 0;
              uart::parcoursDebug(sensor, state, "Poteaux!!");
              wasDetected();
              break;
            }
          }
        }
      }
      oldVals = 0;
    }
  }
  tick++;
  if (tick >= 250) {
    tick = 26;
  }
}

bool Poteau::isReady() { return tick > 25; }