#ifndef PARCOURS_H
#define PARCOURS_H
#include "ajustements.h"
#include "mur.h"
#include "ohBoy.h"
#include "poteau.h"
#include "state.h"
#include "tache.h"

class Parcours {
 public:
  Parcours();
  void exec();

  /**
   * Fait le changement de mur
   */
  void changeWall();

  /**
   * Permet de faire un virage à 180 vers le mur 0
   */
  void virage180_0();

  /**
   * Permet de faire un virage à 180 vers le mur 1
   */
  void virage180_1();

  /**
   * Permet de faire un 180 lorsque le bouton d'intérruption est utilisé
   */
  static void interrupt180();

 private:
  static Parcours* pThis;

  Sensor sensor;
  Poteau poteau;
  Mur mur;
  Ajustement ajustement;

  State state;
  State lastState;

  uint8_t compteurBigTurn;
  bool effectueVirage90;
};
#endif