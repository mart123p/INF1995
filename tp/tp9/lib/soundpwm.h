#ifndef SOUNDPWM_H
#define SOUNDPWM_H

#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>

/**
 * Methods to make some sound and music
 */

namespace soundpwm {

/**
 * Initialize the sound pin
 * @return void
 */
void init();

/**
 * Play a beep sound
 * @param duration in ms to play beep sound
 * @return void
 */
void beep(const uint8_t note);

/**
 * Turn off the sound
 * @return void
 */
void off();
/**
 * Test all the sound functions
 * @return void
 */
void test();

}  // namespace sound

#endif
