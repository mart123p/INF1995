#include "ajustements.h"
Ajustement::Ajustement(Sensor* sensor){
    this->sensor = sensor;
    grosAjustement0IsAjusted = false;
    grosAjustement1IsAjusted = false;
}

void Ajustement::ajuste0(){
    if(sensor->getValSensor0() > 16){
        light::red();
        pwm::set1(acceleration);
        
        if(sensor->getValSensor0() > sensor->getOldVals0()[1]){
            pwm::set0(frein);
        }else{
            pwm::set0(defaultSpeed);
        }
    }
    else if(sensor->getValSensor0() < 13){
        light::red();
        pwm::set0(acceleration);
        if(sensor->getValSensor0() < sensor->getOldVals0()[1]){
            pwm::set1(frein);
        }else{
            pwm::set1(defaultSpeed);
        }
    }
    else{
        light::green();
        pwm::set1(defaultSpeed);
        pwm::set0(defaultSpeed);
    } 
}

void Ajustement::ajuste1(){
    if (sensor->getValSensor1() > 16) {
        light::red();
        pwm::set0(acceleration);
    if (sensor->getValSensor1() > sensor->getOldVals1()[1]) {
        pwm::set1(frein);
    } else {
        pwm::set1(defaultSpeed);
    }
  } else if (sensor->getValSensor1() < 13) {
        light::red();
        pwm::set1(acceleration);
        // Mur à 90 le virage doit etre plus seree
    if (sensor->getValSensor1() < sensor->getOldVals1()[1]) {
        pwm::set0(frein);
    } else {
        pwm::set0(defaultSpeed);
    }
  } else {
        light::green();
        pwm::set0(defaultSpeed);
        pwm::set1(defaultSpeed);
  }
}

bool Ajustement::grosAjustement0(State state) {
  uart::parcoursDebug(sensor, state, "grosAjustement0");
  // Le robot tourne vers le mur 0
  light::red();
  pwm::set0(frein);
  pwm::set1(defaultSpeed);

  // Adjust in consequence if the wall is further the attack angle
  // should be bigger
  if(!grosAjustement0IsAjusted){
    uint16_t angle = 0;
    if (sensor->getValSensor0() > 50) {
      angle = 700;
    } else if (sensor->getValSensor0() > 40) {
      angle = 600;
    } else if (sensor->getValSensor0() > 30) {
      angle = 500;
    } else {
      angle = 400;
    }
    uart::parcoursDebug(sensor,state,"angle grosAjustement");  
    timer::delay(angle);
    grosAjustement0IsAjusted = true;

  }else{
    //The attack angle is ajusted. We need to go foward until the distance is normal
    pwm::set0(defaultSpeed);
    pwm::set1(45);
    if(sensor->getValSensor0() < 20){
      //We are good we need to change the state
      return true;
    }
  }
  return false;
}

bool Ajustement::grosAjustement1(State state){
  uart::parcoursDebug(sensor, state, "grosAjustement1");
  // Le robot tourne vers le mur 0
  light::red();
  pwm::set1(frein);
  pwm::set0(defaultSpeed);

  // Adjust in consequence if the wall is further the attack angle
  // should be bigger
  if(!grosAjustement1IsAjusted){
    uint16_t angle = 0;
    if (sensor->getValSensor1() > 50) {
      angle = 700;
    } else if (sensor->getValSensor1() > 40) {
      angle = 600;
    } else if (sensor->getValSensor1() > 30) {
      angle = 500;
    } else {
      angle = 400;
    }
    uart::parcoursDebug(sensor,state,"angle grosAjustement");  
    timer::delay(angle);
    grosAjustement1IsAjusted = true;
  }else{
    //The attack angle is ajusted. We need to go foward until the distance is normal
    pwm::set1(45);
    pwm::set0(defaultSpeed);

    if(sensor->getValSensor1() < 20){
      //We are good we need to change the state
      return true;
    }
  }
  return false;
}