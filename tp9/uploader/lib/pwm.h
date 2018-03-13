#ifndef PWM_H
#define PWM_H

#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
namespace pwm {

/**
 * Set register for pwm
 * @return void
 */
void init();

/**
 * Set pwm signal to OCR1A
 * @param signal (0 to 255)
 * @param wheelDirection (1 forward or 0 backward)
 * @return void
 */
void setA(uint8_t signal, bool wheelDirection);

/**
 * Set pwm signal to OCR1B
 * @param signal (0 to 255)
 * @param wheelDirection (1 forward or 0 backward)
 * @return void
 */
void setB(uint8_t signal, bool wheelDirection);

/**
 * Test all the functions in pwm.h
 * @return void
 */
void test();
}  // namespace pwm
#endif
