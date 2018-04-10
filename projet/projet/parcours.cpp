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
  tick = 0;
  tickGrosAjustement = 0;
  canSwitchWall = true;
  state = READY;
  lastState = READY;
  angle =0;
  aTrouveUnAngle = false;
}

void Parcours::exec() {

  //Initialisation du code
  soundpwm::init();
  light::green();
  bool isRunning = true;
  
  

  uart::parcoursDebug(sensor, state, "init");


  while (isRunning) {
     sensor.read0();
     sensor.read1();

    uart::parcoursDebug(sensor, state, "Begin");

    wallScrutation();
    poteau.scrutation(sensor,state);


    switch (state) {
    
    case READY:
      if ( sensor.getValSensor0() > sensor.getValSensor1()) { // Determiner quel
        state = WALL_1;                    // mur suivre
      } else {
        state = WALL_0;
      }
      for(uint8_t i = 0; i < 20; i++){
         _delay_ms(50); 
         sensor.read0();
         sensor.read1();
      }// Attente de une seconde avant de commencer
     
                       // à rouler pour ne pas changer dans un mauvais
                       // état
      pwm::set1(defaultSpeed);
      pwm::set0(defaultSpeed);
      break;


    case SWITCH_WALL:
        changeWall();
        break;


    case WALL_0:
         ajustement0(); // Pour rester entre 13 et 16 cm
                       // de distance.
      break;
  
    
    case WALL_1:
         ajustement1();
         break;

   
    case GROS_AJUSTEMENT_0:
         if (sensor.getValSensor0() > vide_0){
          state = BIG_TURN_0;
         }
         grosAjustement0();
         break;

    case GROS_AJUSTEMENT_1:
         if (sensor.getValSensor0() > vide_1){
          state = BIG_TURN_0;
         }
         grosAjustement1();
         break;
   
    case BIG_TURN_0:
        virage90_0();
        break;

     case BIG_TURN_1:
        virage90_1();
        break;

    }
    lastValue0 = sensor.getValSensor0();
    lastValue1 = sensor.getValSensor1();
    _delay_ms(50);
  }
}

bool Parcours::wallScrutation() {
  bool needsToChangeWall = false;
  if (canSwitchWall && state == WALL_0 &&  sensor.getValSensor1() < vide_1) { //Capte un mur et qu'il est deja sur un mur
    if (tick > 15) {
      uart::parcoursDebug(sensor, state, "state = SWITCH_WALL");
      state = SWITCH_WALL;
      lastState = WALL_0;
      needsToChangeWall = true;
      tick = 0;
    }
    tick++;
  } else if (canSwitchWall && state == WALL_1 && sensor.getValSensor0() < vide_0) { //Capte un mur et qu'il est deja sur un mur
    if (tick > 15) {
      uart::parcoursDebug(sensor, state, "state = SWITCH_WALL");
      state = SWITCH_WALL;
      lastState = WALL_1;
      needsToChangeWall = true;
      tick = 0;
    }
    tick++;
  } else {
    tick = 0;
  }

  //Detection si le robot a le droit de changer de cote
  //TODO Changement de mur non fonctionnel
  if(!canSwitchWall){
    if(state == WALL_0){
      if(sensor.read1() > vide_0){
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
      if(sensor.read0() > vide_1){
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

  return needsToChangeWall;
}

void Parcours::changeWall() {
  uart::parcoursDebug(sensor, state, "changewall");
  if (lastState == WALL_0) {
    grosAjustement1();
    canSwitchWall = false;
    state = WALL_1; //TODO add changing wall state
  }
  if (lastState == WALL_1) {
    grosAjustement0();
    canSwitchWall = false;
    state = WALL_0; //TODO add changing wall state
  }
}

// Permet d'aller vers le mur 0
void Parcours::grosAjustement0() {
  
  uart::parcoursDebug(sensor, state, "grosAjustement0");
  // Le robot tourne vers le mur 0
  light::red();
  tickGrosAjustement ++;
  
  if (!aTrouveUnAngle) {
    angle =50;
    aTrouveUnAngle = true;
    pwm::set1(defaultSpeed);
    pwm::set0(frein);
    /*
      angle = 800;
    } else if (sensor.getValSensor1() > 40) {
      angle = 700;
    } else {
      angle = 600;
    */}
    state = WALL_0;

  if (tickGrosAjustement >= angle){
      pwm::set0(60);
      pwm::set1(defaultSpeed);

  }
}

// Permet d'aller vers le mur 1
void Parcours::grosAjustement1() {
  if (sensor.getValSensor1() > borneSupAjustement) {
    uart::parcoursDebug(sensor, state, "grosAjustement1");

    // Le robot tourne vers le mur 1
    light::red();
    pwm::set0(defaultSpeed);
    pwm::set1(frein);

    //Adjust in consequence if the wall is further the attack angle
    //should be bigger
    uint16_t angle = 0;
    if (sensor.getValSensor1() > 50) {
      angle = 800;
    } else if (sensor.getValSensor1() > 40) {
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
  //BIGTURN0
  pwm::set1(defaultSpeed);
  pwm::set0(defaultSpeed);
  _delay_ms(2500);
  if (sensor.read0() > vide_0) {
    pwm::set1(80);
    pwm::set0(-80);
    _delay_ms(300);
    pwm::set1(defaultSpeed);
    pwm::set0(defaultSpeed);
    _delay_ms(1000);
  }
}

void Parcours::virage90_1() {
  //BIGTURN1
  pwm::set1(defaultSpeed);
  pwm::set0(defaultSpeed);
  _delay_ms(2500);
  if (sensor.read1() > vide_1) {
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
  if (sensor.getValSensor0() > 27) {
    //Faire autre chose que gros ajustement?
    state = GROS_AJUSTEMENT_0;
    lastState = WALL_0;
  } else if (sensor.getValSensor0() > 16) {
    light::red();
    pwm::set1(acceleration);
    if (sensor.getValSensor0() > lastValue0) {
      pwm::set0(frein);
    } else {
      pwm::set0(defaultSpeed);
    }
  } else if (sensor.getValSensor0() < 13) {
    light::red();
    pwm::set0(acceleration);
    // Mur à 90 le virage doit etre plus seree
    if (sensor.getValSensor0() < lastValue0) {
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
  if (sensor.getValSensor1() > 27) {
    //Faire autre chose que gros ajustement?
    state = GROS_AJUSTEMENT_1;
    lastState = WALL_1;

  } else if (sensor.getValSensor1() > 16) {
    light::red();
    pwm::set0(acceleration);

    if (sensor.getValSensor1() > lastValue1) {
      pwm::set1(frein);
    } else {
      pwm::set1(defaultSpeed);
    }
  } else if (sensor.getValSensor1() < 13) {
    light::red();
    pwm::set1(acceleration);
    // Mur à 90 le virage doit etre plus seree
    if (sensor.getValSensor1() < lastValue1) {
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
