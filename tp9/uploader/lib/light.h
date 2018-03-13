/**
 * Red cable on pin 4 and +
 */

#ifndef LIGHT_H
#define LIGHT_H

#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>

namespace light {

/**
 * Set PortD pin in output mode
 * @return void
 */
void init();

/**
 * Set the LED to green
 * @return void
 */
void green();

/**
 * Set the LED to red
 * @return void
 */
void red();

/**
 * Set the LED to amber
 * @return void
 */
void amber(uint16_t ms);

/**
 * Turn off the LED
 * @param time in ms the LED is amber
 * @return void
 */
void off();

/**
 * Test the LED functions
 * @return void
 */
void test();
}  // namespace light
#endif
