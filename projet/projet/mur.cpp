#include "mur.h"
Mur::Mur(Poteau* poteau) {
  this->poteau = poteau;
  tick = 0;
  canSwitchWall = true;
}
void Mur::scrutation(Sensor& sensor, State& state, State& lastState) {
  if (canSwitchWall && (state == WALL_0 || state == GROS_AJUSTEMENT_0) &&
      sensor.getValSensor1() < vide_1 && poteau->isReady()) {
    // S'in capte un mur et qu'il est deja sur un mur
    tick++;
    if (tick > 20) {
      uart::parcoursDebug(sensor, state, "state = SWITCH_WALL");
      state = SWITCH_WALL;
      lastState = WALL_0;
      tick = 0;
      canSwitchWall = false;
    }
  } else if (canSwitchWall && (state == WALL_1 || state == GROS_AJUSTEMENT_1) &&
             sensor.getValSensor0() < vide_0 && poteau->isReady()) {
    // S'il capte un mur et qu'il est deja sur un mur
    tick++;
    if (tick > 20) {
      uart::parcoursDebug(sensor, state, "state = SWITCH_WALL");
      state = SWITCH_WALL;
      lastState = WALL_1;
      tick = 0;
      canSwitchWall = false;
    }
  }

  // Detection si le robot a le droit de changer de cote
  if (!canSwitchWall) {
    if (state == WALL_0 || state == GROS_AJUSTEMENT_0) {
      if (sensor.read1() > vide_1) {
        if (tick > 3) {  // On s'assure que c'est bien un vide
          tick = 0;
          canSwitchWall = true;
          uart::parcoursDebug(sensor, state, "canSwitchWall -> WaLL_1");
        }
        tick++;
      } else {
        tick = 0;
      }
    } else if (state == WALL_1 || state == GROS_AJUSTEMENT_1) {
      if (sensor.read0() > vide_0) {
        if (tick > 3) {
          tick = 0;
          canSwitchWall = true;
          uart::parcoursDebug(sensor, state, "canSwitchWall -> WALL_0");
        }
        tick++;
      } else {
        tick = 0;
      }
    }
  }
}

void Mur::reset() { canSwitchWall = true; }