/*
 * Reader: Interpréteur de bytecode du cours INF1995
 *
 * Créé par 
 * Cédric Tessier (1907831)
 * Martin Pouliot (1900045)
 * Pascal-Alexandre Morel (1907829)
 * Sean Costello (1902126)
 *
 * 20 mars 2018
 *
 * Description du programme:
 * Programme lisant un bytecode sur la mémoire externe du robot. Il évalue ces
 * instructions et les exécutent. Un programme nommé uploader se retrouve dans
 * un autre dossier pour téléverser le bytecode sur la mémoire externe.
 *
 */
#include <stdlib.h>
#include "ohBoy.h"

// Encodage des diverses instructions qui peuvent être représentées par le
// bytecode. Ces instructions sont utilisées dans la méthode 
// instructionExecute()
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

  //Extraction de la taille du bytecode. Cette valeur va agir comme limite
  //afin que le programme puisse arrêter même s'il n y a pas d'instruction qui
  //indique la fin du bytecode.
  uint8_t bytecodeHeader[2]; 
  memoire.lecture(0,bytecodeHeader,2);
  uint16_t tailleBytecode = bytecodeHeader[1] | (bytecodeHeader[0] << 8);
  
  uint16_t adresse = 2;
  uint8_t instruction[2];

  bool estTermine = false;
  bool debutCode = false;

  uart::clear();  // Nettoie la console.
  uart::print("Starting VirtualMachine\n");
  uart::print("Bytecode size: ");
  uart::print(tailleBytecode);
  uart::print("\n\n");
  while (!estTermine) {
    memoire.lecture(
        adresse, instruction,
        2);        // Lecture de la prochaine instruction qu'on met en mémoire.
    adresse += 2;  // Compteur d'adresse

    // Instructions de débogage. Utile pour vérifier le bon fonctionnement
    // du robot.
    uart::print("[Mem ");
    uart::print(adresse);
    uart::print("] => Instruction: ");
    uart::print(instruction[0]);
    uart::print(" ");
    uart::print(instruction[1]);
    uart::println();

    // Vérification si l'on est au début du programme. Si l'on n'est pas au 
    // début du programme, les instructions seront ignorées.
    if (!debutCode && instruction[0] == dbt) {
      debutCode = true;

      // Initialisation des différents périphériques sur le robot.
      light::init();
      pwm::init();
      soundpwm::init();
      continue;
    }

    // Une fois que l'instruction de début a été exécutée on peut commencer à
    // regarder pour des instructions de contrôle. Les instructions de
    // contrôle sont fin, dbc fbc.
    if (debutCode) {
      if (instruction[0] == fin || adresse >= tailleBytecode) {
        estTermine = true;
        uart::print("\n\nEOF");
        pwm::off();
        soundpwm::off();

      } else if (instruction[0] == dbc) {
        // On met toutes les instructions de la boucle dans la mémoire vive. On 
        // le fait seulement si plus d'une itération doit être faite.
        if (instruction[1] > 0) {
          uint16_t addresseBoucle = adresse;
          uint8_t instructionBoucle[2];

          uint8_t* instructionBuffer = 0;  // Buffer utilisé pour enregistrer
                                           // les instructions en mémoire.
          uint8_t instructionCount = 0;
          memoire.lecture(addresseBoucle, instructionBoucle, 2);

          // On itère sur la mémoire tant que le l'instruction n'est pas 
          // l'instruction fbc qui indique la fin de la boucle.
          while (instructionBoucle[0] != fbc) {
            instructionBuffer = (uint8_t*)realloc(
                instructionBuffer,
                instructionCount * 2 + 2);  // On fait de l'espace dans le
                                            // buffer à l'aide de realloc() pour
                                            // ajouter les deux prochains bytes
                                            // d'instructions.

            // Copie les deux bytes lus en mémoire et les rajoutent à la fin du
            // buffer.
            for (uint8_t i = 0; i < 2; i++) {
              instructionBuffer[instructionCount * 2 + i] =
                  instructionBoucle[i];
            }

            addresseBoucle += 2;
            instructionCount++;
            memoire.lecture(addresseBoucle, instructionBoucle,
                            2);  // Cherche la prochaine valeur dans la mémoire.
          }

          // On fait le nombre d'itérations nécessaires qui sont spécifiées par
          // l'opérande de dbc.
          for (uint8_t i = 0; i < instruction[1]; i++) {
            uart::print("\n\n*** loop ");
            uart::print(i);
            uart::print(") nb instruction: ");
            uart::print(instructionCount);
            uart::print(" ***\n");

            // On itère sur les instructions qui ont été enregistrées en mémoire
            for (uint8_t j = 0; j < instructionCount; j++) {
              uart::print("\n--executing instruction ");
              uart::print(j);
              uart::println();

              uint8_t* singleInstruction =
                  instructionBuffer + j * 2;  // On crée un nouveau pointeur qui
                                              // pointe à la bonne place dans le
                                              // buffer. On le passe à la
                                              // fonction instructionExecute() 
                                              // qui va interpréter ces deux 
                                              // bytes.
              instructionExecute(singleInstruction);

              uart::print("--instruction ");
              uart::print(singleInstruction[0]);
              uart::print(" ");
              uart::print(singleInstruction[1]);
              uart::print(" executed\n");
            }
            uart::print("*** end loop ***\n");
          }

          // Nettoyage du buffer pour récupérer la mémoire utilisée par 
          // celui-ci.
          free(instructionBuffer);
          instructionBuffer = 0;
        }

      } else if (instruction[0] != fbc) {
        // Si ce n'est pas une instruction de contrôle, on exécute l'instruction
        // en question.
        instructionExecute(instruction);
      }
    }
  }

  // Le programme a fini d'itérer sur le bytecode.
  while (true)
    ;
}



/**
 * Exécute l'instruction passée en paramètre
 * @param pointeur sur un tableau de deux bytes qui représente l'instruction à
 * exécuter
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
      uart::print("sgo\n");
      soundpwm::beep(instruction[1]);
      break;

    case sar:
      uart::print("sar\n");
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
    default:
      uart::print("Unknown instruction ");
      uart::print(instruction[0]);
      uart::println();
  }
}
