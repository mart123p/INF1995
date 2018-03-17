/*
 *main.cpp:
 *
 * OhBoy library test
 *
 *
 */
#include "ohBoy.h"

int main() {
	soundpwm::init();
	soundpwm::beep(45);
	_delay_ms(5000);
	soundpwm::beep(75);
	_delay_ms(5000);
	soundpwm::off();
  	while (true);
}
