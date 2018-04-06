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
 * @param signal (-100 to 100)
 * @return void
 */
void set1(int8_t signal);

/**
 * Set pwm signal to OCR1B
 * @param signal (-100 to 100)
 * @return void
 */
void set0(int8_t signal);

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
