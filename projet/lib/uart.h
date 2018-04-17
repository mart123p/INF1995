#ifndef UART_H
#define UART_H
/**
 * Envoyer et recevoir diverses messages avec l'interface de communication UART.
 * Ces messages peuvent être reçus à l'aide du programme sérieViaUSB. Certaines
 * méthodes ont été conçues pour être utilisé avec les logiciels de diagnostique
 * maison dont il est question dans readme.md
 */

#include <avr/io.h>
#include "sensors.h"

namespace uart {
/**
 * Configuration des registres nécessaire pour la communication UART
 * @return void
 */
void init();

/**
 * Afficher en UART un tableau de caractères ou une chaîne de caractères
 * @param c Chaîne de caractères
 * @param size Taille de la chaîne de caractère
 * @return void
 */
void print(const char *c, const uint8_t size);

/**
 * Afficher en UART un tableau de caractères ou une chaîne de caractères. La
 * taille n'est pas nécessaire, car le code regarde pour le caractère nul.
 * @param c Chaîne de caractères
 * @return void
 */
void print(const char *c);
/**
 * Afficher un entier en ascii sur UART
 * @param n L'entier à afficher
 * @return void
 */
void print(const int n);

/**
 * Afficher un gros entier en ascii sur UART
 * @param n Le long à afficher
 * @return void
 */
void print(const long n);

/**
 * Afficher un uint8_t en ascii sur UART
 * @param n L'octet à afficher
 * @return void
 */
void print(const uint8_t n);

/**
 * Afficher un uint16_t en ascii sur UART
 * @param n Le mot à afficher
 * @return void
 */
void print(const uint16_t n);

/**
 * Afficher un uint32_t en ascii sur UART
 * @param n Le uint32_t à afficher
 * @return void
 */
void print(const uint32_t n);

/**
 * Afficher un caractère sur UART
 * @param c Le caractère à afficher
 * @return void
 */
void print(const char c);

/**
 * Afficher un retour à la ligne sur UART
 * @return void
 */
void println();
/**
 * Lire les données en entrée sur UART
 * @return uint8_t L'octet
 */
uint8_t readData();

/**
 * Envoyer des données en UART
 * @param data L'octet à envoyer
 * @return void 
 */
void sendData(uint8_t data);


/**
 * Nettoyage de la console
 * @return void
 */
void clear();
/**
 * Tester toutes les méthodes UART
 * @return void
 */ 
void test();

//Débogage maison

/**
 * Rafraîchir le logiciel de débogage avec de nouvelles données, afficher en 
 * même temps une chaîne de caractère sur le moniteur série.
 * @param Sensor* sensor La classe Sensor pour obtenir les valeurs des capteurs
 * actuellement
 * @param uint8_t state État de la machine a état
 * @param const char *c Chaîne de caractère à afficher sur le moniteur série
 */
void parcoursDebug(Sensor &sensor, uint8_t state, const char *c);

/**
 * Rafraîchir le logiciel de débogage avec de nouvelles données, afficher en 
 * même temps une chaîne de caractère sur le moniteur série.
 * @param Sensor &sensor La classe Sensor pour obtenir les valeurs des capteurs
 * actuellement
 * @param uint8_t state État de la machine a état
 * @param const char *c Chaîne de caractère à afficher sur le moniteur série
 */
void parcoursDebug(Sensor* sensor, uint8_t state, const char *c);

/**
 * Rafraîchir le logiciel de débogage avec de nouvelles données, afficher en 
 * même temps une valeur sur le moniteur série.
 * @param Sensor &sensor La classe Sensor pour obtenir les valeurs des capteurs
 * actuellement
 * @param uint8_t state État de la machine a état
 * @param uint8_t num Valeur à afficher sur le moniteur série.
 */
void parcoursDebug(Sensor &sensor, uint8_t state, uint8_t num);

/**
 * Rafraîchir le logiciel de débogage avec de nouvelles données, afficher en 
 * même temps une valeur sur le moniteur série.
 * @param Sensor* sensor La classe Sensor pour obtenir les valeurs des capteurs
 * actuellement
 * @param uint8_t state État de la machine a état
 * @param uint8_t num Valeur à afficher sur le moniteur série.
 */
void parcoursDebug(Sensor* sensor, uint8_t state, uint8_t num);


}  // namespace uart



#endif
