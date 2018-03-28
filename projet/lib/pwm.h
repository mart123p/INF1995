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
 * @param wheelDirection (0 forward or 1 backward)
 * @return void
 */
void setA(int8_t signal);

/**
 * Set pwm signal to OCR1B
 * @param signal (0 to 255)
 * @param wheelDirection (0 forward or 1 backward)
 * @return void
 */
void setB(int8_t signal);

/**
 * Test all the functions in pwm.h
 * @return void
 */


/**
 * Turn off both wheels 
 * @return void
 */ 
void off();

void test();
}  // namespace pwm
#endif
