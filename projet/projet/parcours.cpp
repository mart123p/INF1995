#include "parcours.h"

Parcours* Parcours::pThis;


Parcours::Parcours() : ajustement(&sensor) {
  
  state = READY;
  lastState = READY;
  compteurBigTurn =0;
  effectueVirage90 = false;

  //Activation de l'interruption du bouton pour
  //faire la rotation du 180.
  DDRD &= ~(1 << PD2);     // Clear the PD2 pin 
  EICRA |= (1 << ISC00);    // set INT0 to trigger on ANY logic change
  EIMSK |= (1 << INT0);     // Turns on INT0
  sei();


  pThis = this; //On utilise cet attribut pour pouvoir interagir avec
                //la classe lors de l'interrupt

}

void Parcours::exec() {
  // Initialisation du code
  soundpwm::init();
  light::green();
  bool isRunning = true;

  uart::parcoursDebug(sensor, state, "init");

  while (isRunning) {
    
    sensor.read0();
    sensor.read1();

    uart::parcoursDebug(sensor, state, "Begin");

    //Execution des taches d'arriere plan
    mur.scrutation(sensor,state,lastState); //Cette fonction peut changer l'etat de la machine
                                            //A etat, il faut donc faire attention.
    poteau.scrutation(sensor, state,lastState);
   // bouton.scrutation(sensor, state,lastState);
    //Fin des taches de scrutation.


    //Debut de la machine a etat
    switch (state) {
      case READY:
        if (sensor.getValSensor0() > sensor.getValSensor1()) {  // Determiner quel
          state = WALL_1;              // mur suivre
        } else {
          state = WALL_0;
        }
        for (uint8_t i = 0; i < 20; i++) {
          _delay_ms(50);
          sensor.read0();
          sensor.read1();
        }  // Attente de une seconde avant de commencer

        // à rouler pour ne pas changer dans un mauvais
        // état
        pwm::set1(defaultSpeed);
        pwm::set0(defaultSpeed);
        break;

      case SWITCH_WALL:
        changeWall();
        break;

      case WALL_0:
        if (sensor.getValSensor0() > vide_0){
          state = BIG_TURN_0;
        }else if (sensor.getValSensor0() > 27) {
          state = GROS_AJUSTEMENT_0;
          lastState = WALL_0;
        }else{ 
          ajustement.ajuste0(); //Rester entre 13 et 16cm
        }
        break;

      case WALL_1:
        if (sensor.getValSensor1() > vide_0){
          state = BIG_TURN_1;
        }else if (sensor.getValSensor1() > 27) {
          state = GROS_AJUSTEMENT_1;
          lastState = WALL_1;
        }else{ 
          ajustement.ajuste1();
        }
        break;

      case GROS_AJUSTEMENT_0:
          if(ajustement.grosAjustement0(state)){
            state = WALL_0;
          } 
        break;

      case GROS_AJUSTEMENT_1:
          if(ajustement.grosAjustement1(state)){
            state = WALL_1;
          }
        break;

      case BIG_TURN_0:
        virage90_0();
        break;

      case BIG_TURN_1:
        virage90_1();
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

void Parcours::virage90_0() {
  uart::parcoursDebug(sensor, state, "BIGTURN0");
  // Le robot tourne vers le mur 0
  light::red();
  
  pwm::set0(defaultSpeed);
  pwm::set1 (defaultSpeed);
  timer::delay(2000);
  if (sensor.getValSensor0() > vide_0 ){
    pwm::set0(frein);
    pwm::set1(defaultSpeed);
    timer::delay(800);
   } 
  state = WALL_0;
}


void Parcours::virage90_1() {

  pwm::set1(defaultSpeed);
  pwm::set0(defaultSpeed);
  timer::delay(2000);
  if (sensor.getValSensor1() > vide_0 ){
     pwm::set1(frein);
    pwm::set0(defaultSpeed);
    timer::delay(800);
  }
  state = WALL_1;
}

void Parcours::interrupt180(){
  if(pThis->poteau.getDetected()){
    pThis->poteau.cancelBeep();
    soundpwm::off();
  }

  pwm::set0(0);
  pwm::set1(0);
  _delay_ms(500); //Le temps que le robot arrête

  pwm::set0(-80);
  pwm::set1(80);
  _delay_ms(1100);

  pwm::set0(-40);
  pwm::set1(40);
  _delay_ms(200);

  pwm::set0(0);
  pwm::set1(0);
  _delay_ms(500);

  switch(pThis->state){
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

//Interruption pour faire le 180
//lorsque le bouton est enfoncé
ISR(INT0_vect)
{
  _delay_ms(5);
  if ( PIND & 0x04 ){
    Parcours::interrupt180();
  }
}