#include "mur.h"
Mur::Mur(){
    tick = 0;
    canSwitchWall = false;
}
void Mur::scrutation(Sensor& sensor,State& state,State& lastState){
    if (canSwitchWall && state == WALL_0 && sensor.getValSensor1() < vide_1) {  // Capte un mur et qu'il est deja sur un mur
        if (tick > 15) {
            uart::parcoursDebug(sensor, state, "state = SWITCH_WALL");
            state = SWITCH_WALL;
            lastState = WALL_0;
            tick = 0;
        }
    tick++;
    } else if (canSwitchWall && state == WALL_1 && sensor.getValSensor0() < vide_0) {  // Capte un mur et qu'il est deja sur un mur
        if (tick > 15) {
            uart::parcoursDebug(sensor, state, "state = SWITCH_WALL");
            state = SWITCH_WALL;
            lastState = WALL_1;
            tick = 0;
        }
    tick++;
    } else {
        tick = 0;
    }

    // Detection si le robot a le droit de changer de cote
    // TODO Changement de mur non fonctionnel
    if (!canSwitchWall) {
        if (state == WALL_0) {
            if (sensor.read1() > vide_0) {
                if (tick > 5) {
                    tick = 0;
                    canSwitchWall = true;
                    uart::parcoursDebug(sensor, state, "canSwitchWall true");
                }
                tick++;
            } else {
                tick = 0;
            }
        } else if (state == WALL_1) {
            if (sensor.read0() > vide_1) {
                if (tick > 5) {
                    tick = 0;
                    canSwitchWall = true;
                    uart::parcoursDebug(sensor, state, "canSwitchWall true");
                }
                tick++;
            } else {
                tick = 0;
            }
        }
    }
}

void Mur::reset(){
    canSwitchWall = false;
}