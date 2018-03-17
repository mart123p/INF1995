/*
 * Reader Interpreteur de byte code
 *
 * Créé par 
 * Cédric Tessier (1907831)
 * Martin Pouliot (1900045)
 * Pascal-Alexandre Morel (xxxxxxx)
 * Sean Costello (xxxxxxx)
 *
 * 1er mars 2018
 *
 * Description du programme:
 * Programme modifiant la couleur de la DEL en fonction de l'intensité lumineuse 
 * ambiante captée par le photorésistance.
 *
 */
#include <stdlib.h>
#include "ohBoy.h"

// Encodage des diverses instructions qui peuvent etre represente par le
// bytecode. Ces instructions sont utilises dans la methode
// instruction execute.
#define dbt 0x01
#define att 0x02
#define dal 0x44
#define det 0x45
#define sgo 0x48
#define sar 0x09
#define mar 0x60
#define mar1 0x61
#define mav 0x62
#define mre 0x63
#define trd 0x64
#define trg 0x65
#define dbc 0xC0
#define fbc 0xC1
#define fin 0xFF

void instructionExecute(uint8_t* instruction);

int main() {
  uart::init();
  mem::Memoire24CXXX memoire;
  uint16_t adresse = 2;
  uint8_t instruction[2];

  bool estTermine = false;
  bool debutCode = false;

  uart::clear();  // Nettoye la console.
  uart::print("Starting VirtualMachine\n\n");

  while (!estTermine) {
    memoire.lecture(
        adresse, instruction,
        2);        // Lecture de la prochaine instruction qu'on met en memoire
    adresse += 2;  // Compteur d'adresse

    // Instructions de debogage. Utile pour verifier le bon fonctionnement
    // du robot.
    uart::print("[Mem] => Instruction: ");
    uart::print(instruction[0]);
    uart::print(" ");
    uart::print(instruction[1]);
    uart::println();

    // Verification si on est au debut du programme si on n'est pas au debut
    // du programme les instructions sont ignorees
    if (!debutCode && instruction[0] == dbt) {
      debutCode = true;

      // Intialisation des differents peripheriques sur le robot.
      light::init();
      pwm::init();
      soundpwm::init();
    }

    // Une fois que l'instruction de debut a ete execute on peut commencer a
    // regarder pour des instructions de controle. Les instructions de
    // controle sont fin, dbc fbc.
    if (debutCode) {
      if (instruction[0] == fin) {
        estTermine = true;
        pwm::off();
        soundpwm::off();

      } else if (instruction[0] == dbc) {
        // On met toutes les instructions de la boucle dans la mémoire vive. Si
        // on doit faire plus d'une itération sur la boucle.
        if (instruction[1] > 0) {
          uint16_t addresseBoucle = adresse;
          uint8_t instructionBoucle[2];

          uint8_t* instructionBuffer = 0;  // Buffer utilise pour enregistrer
                                           // les instructions en memoire.
          uint8_t instructionCount = 0;
          memoire.lecture(addresseBoucle, instructionBoucle, 2);

          // On itere tant que le l'instruction n'est pas l'instruction fbc qui
          // est l'instruction qui indique la fin de la boucle
          while (instructionBoucle[0] != fbc) {
            instructionBuffer = (uint8_t*)realloc(
                instructionBuffer,
                instructionCount * 2 + 2);  // On fait de l'espace dans le
                                            // buffer a l'aide de realloc pour
                                            // ajouter les deux prochains bytes
                                            // d'instructions.

            // Copie les deux bytes lues en memoire et les rajoute a la fin du
            // buffer
            for (uint8_t i = 0; i < 2; i++) {
              instructionBuffer[instructionCount * 2 + i] =
                  instructionBoucle[i];
            }

            addresseBoucle += 2;
            instructionCount++;
            memoire.lecture(addresseBoucle, instructionBoucle,
                            2);  // Cherche la prochaine valeur dans la memoire.
          }

          // On fait le nombre d'ittérations nécessaires qui sont spécifiées par
          // l'opérande de dbc.
          for (uint8_t i = 0; i < instruction[1]; i++) {
            uart::print("\n\n*** loop ");
            uart::print(i);
            uart::print(") nb instruction: ");
            uart::print(instructionCount);
            uart::print(" ***\n");

            // On itere sur les instructions qui ont ete enregistres en memoire
            for (uint8_t j = 0; j < instructionCount; j++) {
              uart::print("\n--executing instruction ");
              uart::print(j);
              uart::println();

              uint8_t* singleInstruction =
                  instructionBuffer + j * 2;  // On cree un nouveau pointeur qui
                                              // pointe a la bonne place dans le
                                              // buffer et on le passe a la
                                              // fonction instructionExecute qui
                                              // va interpreter ces deux bytes
              instructionExecute(singleInstruction);

              uart::print("--instruction ");
              uart::print(singleInstruction[0]);
              uart::print(" ");
              uart::print(singleInstruction[1]);
              uart::print(" executed\n");
            }
            uart::print("*** end loop ***\n");
          }

          // Nettoyage du buffer pour recuperer la memoire utilise par le buffer
          free(instructionBuffer);
          instructionBuffer = 0;
        }

      } else if (instruction[0] != fbc) {
        // Si ce n'est pas une instruction de controle, on execute l'instruction
        // en question
        instructionExecute(instruction);
      }
    }
  }

  // Le programme a fini d'iterer sur le byte code.
  while (true)
    ;
}

/**
 * Execute l'instruction en question.
 * @param pointeur sur un tableau de 2 bytes qui reprensente l'instruction a
 * executer
 */
void instructionExecute(uint8_t* instruction) {
  switch (instruction[0]) {
    case att:
      uart::print("att\n");
      for (uint8_t i = 0; i < instruction[1]; i++) {
        _delay_ms(25);
      }
      break;

    case dal:
      uart::print("dal\n");
      light::on(instruction[1]);
      break;

    case det:
      uart::print("det\n");
      light::off(instruction[1]);
      break;

    case sgo:
      soundpwm::beep(instruction[1]);
      break;

    case sar:
      soundpwm::off();
      break;

    case mar:
    case mar1:
      uart::print("mar1\n");
      pwm::off();
      break;

    case mav:
      uart::print("mav\n");
      pwm::setA(instruction[1], 0);
      pwm::setB(instruction[1], 0);
      break;

    case mre:
      uart::print("mre\n");
      pwm::setA(instruction[1], 1);
      pwm::setB(instruction[1], 1);
      break;

    case trg:
      uart::print("trg\n");
      pwm::setA(255, 0);
      pwm::setB(255, 1);
      _delay_ms(425);
      pwm::off();
      break;
    case trd:
      uart::print("trd\n");
      pwm::setA(255, 1);
      pwm::setB(255, 0);
      _delay_ms(425);
      pwm::off();
      break;
  }
}
