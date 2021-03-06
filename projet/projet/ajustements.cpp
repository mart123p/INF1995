#include "ajustements.h"
Ajustement::Ajustement(Sensor* sensor) {
  this->sensor = sensor;
  grosAjustement0IsAjusted = false;
  grosAjustement1IsAjusted = false;

  correctionFrein0 = false;
  correctionFrein1 = false;

  doitAttendre = true;
  tick = 0;
}

void Ajustement::ajuste0() {
  // Si le robot est trop loin du mur 0, on fait accélérer
  // le moteur 1 pour qu'il se rapproche
  if (sensor->getValSensor0() > 16) {
    uart::parcoursDebug(sensor, 255, "capteur 0 trop loin");
    light::red();
    pwm::set1(acceleration);
    correctionFrein0 = false;

    // On vérifie que le robot se rapproche bel et bien du mur.
    // Si ce n'est pas le cas, on augmente l'efficacité du virage
    if (sensor->getValSensor0() > sensor->getOldVals0()[1]) {
      pwm::set0(frein);
    } else {
      pwm::set0(defaultSpeed);
    }
  }
  // Si le robot est trop près dur mur 0, on fait accélérer
  // le moteur 0 pour s'éloigner du mur
  else if (sensor->getValSensor0() < 13) {
    uart::parcoursDebug(sensor, 255, "capteur 0 trop proche");
    light::red();
    pwm::set0(acceleration);
    correctionFrein0 = false;
    // On s'assure que le robot s'éloigne bel et bien du mur.
    // Sinon on augmente l'efficacité du virage
    if (sensor->getValSensor0() < sensor->getOldVals0()[1]) {
      pwm::set1(frein);
      correctionFrein0 = true;
    } else {
      pwm::set1(defaultSpeed);
    }
  }
  // Si le robot est à la bonne distance du mur on le fait
  // aller tout doit
  else {
    light::green();
    correctionFrein0 = false;
    pwm::set1(defaultSpeed);
    pwm::set0(defaultSpeed);
  }
}

void Ajustement::ajuste1() {
  // Si le robot est trop loin du mur 0, on fait accélérer
  // le moteur 1 pour qu'il se rapproche.
  if (sensor->getValSensor1() > 16) {
    uart::parcoursDebug(sensor, 255, "capteur 1 trop loin");
    light::red();
    pwm::set0(acceleration);
    correctionFrein1 = false;
    // On vérifie que le robot se rapproche bel et bien du mur
    // si ce n'est pas le cas on augmente l'efficacité du virage
    if (sensor->getValSensor1() > sensor->getOldVals1()[1]) {
      pwm::set1(frein);
    } else {
      pwm::set1(defaultSpeed);
    }
  }
  // Si le robot est trop près du mur 1, on fait accélérer
  // le moteur 1 pour s'éloigner du mur
  else if (sensor->getValSensor1() < 13) {
    light::red();
    uart::parcoursDebug(sensor, 255, "capteur 1 trop proche");
    pwm::set1(acceleration);
    correctionFrein1 = false;
    // On s'assure que le robot s'éloigne bel et bien du mur
    // sinon on augmente l'efficacité du virage
    if (sensor->getValSensor1() < sensor->getOldVals1()[1]) {
      pwm::set0(frein);
      correctionFrein1 = true;
    } else {
      pwm::set0(defaultSpeed);
    }
  }
  // Si le robot est à la bonne distance dur mur, on le fait
  // aller tout droit.
  else {
    light::green();
    correctionFrein1 = false;
    pwm::set0(defaultSpeed);
    pwm::set1(defaultSpeed);
  }
}

bool Ajustement::grosAjustement0(State state) {
  // On vérifie que le robot est dans les bornes de grosAjustement
  // ou s'il est autorisé à faire son grosAjustement sans attendre
  // un certain nombre de tick
  if ((sensor->getValSensor0() > 27 && sensor->getValSensor0() < vide_0) ||
      !doitAttendre) {
    tick++;
    // Après 15 tick le robot est certain qu'il doit faire un gros ajustement
    if (tick >= 15 || !doitAttendre) {
      uart::parcoursDebug(sensor, state, "grosAjustement0");
      // Le robot tourne vers le mur 0
      light::red();

      // Plus le mur est loin, plus on fait tourner le robot
      // pour qu'il s'y rend plus rapidement
      if (!grosAjustement0IsAjusted) {
        uint16_t angle = 0;
        if (sensor->getValSensor0() > 50) {
          angle = 750;
        } else if (sensor->getValSensor0() > 40) {
          angle = 550;
        } else if (sensor->getValSensor0() > 30) {
          angle = 475;
        } else if (sensor->getValSensor0() > 20) {
          angle = 425;
        } else {
          angle = 0;
        }
        uart::parcoursDebug(sensor, state, "angle grosAjustement");
        pwm::set0(-50);
        pwm::set1(50);
        timer::delay(angle);
        grosAjustement0IsAjusted = true;

        pwm::set0(defaultSpeed);
        pwm::set1(defaultSpeed);
      } else {
        // Une fois l'angle ajusté, le robot avance tout droit.
        pwm::set1(45);
        pwm::set0(35);

        if (sensor->getValSensor0() < 18) {
          pwm::set0(-85);
          pwm::set1(-85);
          // Une fois rendu à l'autre mur, le robot change d'état.
          // Le robot freine aussi pour ne pas rentrer dans le mur.
          tick = 0;
          doitAttendre = true;
          grosAjustement0IsAjusted = false;
          return true;
        }
      }
      return false;
    }
    return false;
  } else {
    grosAjustement0IsAjusted = false;
    // Si le robot n'est plus dans les bonnes bornes, il change d'état.
    tick = 0;
    uart::parcoursDebug(sensor, state, "Faux gros Ajustement");
    // L'état devient Wall_1 puisque la fonction return true
    return true;
  }
}

bool Ajustement::grosAjustement1(State state) {
  // On vérifie que le robot est dans les bornes de grosAjustement
  // Ou si il est autorisé à faire son grosAjustement sans attendre
  // un certain nombre de tick
  if ((sensor->getValSensor1() > 27 && sensor->getValSensor1() < vide_1) ||
      !doitAttendre) {
    tick++;
    // Après 15 tick le robot est certain qu'il doit faire un gros ajustement
    if (tick >= 15 || !doitAttendre) {
      uart::parcoursDebug(sensor, state, "grosAjustement1");
      // Le robot tourne vers le mur 0
      light::red();

      // Plus le mur est loin, plus on fait tourner le robot
      // pour qu'il s'y rend plus rapidement
      // Les roues ne tournent pas au meme vitesses
      // ce qui explique le delay moins long pour tourner.
      if (!grosAjustement1IsAjusted) {
        uint16_t angle = 0;
        if (sensor->getValSensor1() > 50) {
          angle = 600;
        } else if (sensor->getValSensor1() > 40) {
          angle = 415;
        } else if (sensor->getValSensor1() > 30) {
          angle = 325;
        } else if (sensor->getValSensor1() > 20) {
          angle = 300;
        } else {
          angle = 0;
        }
        uart::parcoursDebug(sensor, state, "angle grosAjustement");
        pwm::set1(-50);
        pwm::set0(50);
        timer::delay(angle);
        pwm::set0(defaultSpeed);
        pwm::set1(defaultSpeed);
        grosAjustement1IsAjusted = true;
      } else {
        // Une fois l'ange ajusté, le robot avance tout droit
        pwm::set0(55);
        pwm::set1(65);

        if (sensor->getValSensor1() < 18) {
          // Nous sommes assez proche du mur, on peut changer d'état.
          pwm::set0(-85);
          pwm::set1(-85);
          tick = 0;
          doitAttendre = true;
          grosAjustement1IsAjusted = false;
          return true;
        }
      }
      return false;
    }
    return false;
  } else {
    grosAjustement1IsAjusted = false;
    // Si le robot n'est plus dans les bonnes bornes il change de state
    tick = 0;
    // L'état devient Wall_1 puisque la fonction return true
    uart::parcoursDebug(sensor, state, "Faux gros Ajustement");
    return true;
  }
}

void Ajustement::neDoitPasAttendre() { doitAttendre = false; }

void Ajustement::reset0() {
  // Permet au robot d'ajuster son angle
  // lorsqu'il sort de cet état.
  tick = 0;
  grosAjustement0IsAjusted = false;
}

void Ajustement::reset1() {
  tick = 0;
  grosAjustement1IsAjusted = false;
}

bool Ajustement::didCorrectionFrein0() { return correctionFrein0; }

bool Ajustement::didCorrectionFrein1() { return correctionFrein1; }