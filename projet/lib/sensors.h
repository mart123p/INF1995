#ifndef SENSORS_H
#define SENSORS_H

/**
 * Contrôle des capteurs infrarouge SHARP. Gestion d'une pile pour garder en
 * mémoire les valeurs précédentes. Conversion des valeurs du CAN vers une
 * distance en cm qui est plus facile à travailler avec.
 */

#define SENSOR_SIZE 18  // Taille de la pile pour les deux capteurs

#include "can.h"

class Sensor {
 public:
  /**
   * Constructeur de Sensor
   */
  Sensor();

  /**
   * Prendre une lecture du capteur 0
   * @return uint8_t distance en cm
   */
  uint8_t read0();

  /**
   * Prendre une lecture du capteur 1
   * @return uint8_t distance en cm
   */
  uint8_t read1();

  /**
   * Retourne la dernière valeur captée par le capteur 0
   * @return uint8_t distance en cm
   */
  uint8_t getValSensor0();

  /**
   * Retourne la dernière valeur captée par le capteur 1
   * @return uint8_t distance en cm
   */
  uint8_t getValSensor1();

  /**
   * Retourne une pile des dernières valeurs enregistrées par le capteur 0
   * @return uint8_t* pile des valeurs
   */
  uint8_t* getOldVals0();

  /**
   * Retourne une pile des dernières valeurs enregistrées par le capteur 1
   * @return uint8_t* pile des valeurs
   */
  uint8_t* getOldVals1();

  /**
   * Retourne la dernière valeur du capteur 0. Cette valeur est la valeur du
   * CAN. Elle est principalement utilisé pour les fonctions de débogage.
   * @return uint16_t dernière valeur du CAN
   */
  uint16_t getAdcSensor0();

  /**
   * Retourne la dernière valeur du capteur 1. Cette valeur est la valeur du
   * CAN. Elle est principalement utilisé pour les fonctions de débogage.
   * @return uint16_t dernière valeur du CAN
   */
  uint16_t getAdcSensor1();

 private:
  /**
   * S'occupe d'ajouter une valeur sur le dessus de la pile. Lorsque la pile
   * déborde les valeurs les plus anciennes sont retirées.
   * @param val Valeur à rajouter dans la pile
   * @param uint8_t* array Sur quelle pile les valeurs doivent être rajoutées
  */
  void push(uint8_t val, uint8_t* array);

  can::can can;  // Convertisseur analogique numérique

  uint16_t adcSensor0;  // Dernière valeur du CAN pour le capteur 0
  uint16_t adcSensor1;  // Dernière valeur du CAN pour le capteur 1

  uint8_t valSensor0;  // Dernière valeur du capteur 0
  uint8_t valSensor1;  // Dernière valeur du capteur 1

  uint8_t oldVals0[SENSOR_SIZE];  // Pile du capteur 0
  uint8_t oldVals1[SENSOR_SIZE];  // Pile du capteur 1
};

#endif
