#ifndef TACHE_H
#define TACHE_H
#include "ohBoy.h"
#include "state.h"
class Tache {
 public:
  /**
   * Méthode virtuelle pure pour les scrutatioins
   * @param sensor    Capteur donnant les distances
   * @param state     État de la machine à état
   * @param lastState Dernier état dans lequel le robot c'est retrouvé
   */
  virtual void scrutation(Sensor& sensor, State& state, State& lastState) = 0;
};
#endif