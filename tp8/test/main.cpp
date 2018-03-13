/*
 *main.cpp:
 *
 * OhBoy library test
 *
 *
 */
#include "ohBoy.h"

volatile uint16_t cmp = 0;
volatile bool isTestDone = false;

ISR(TIMER1_COMPA_vect) {
  cmp++;
  uart::print(cmp);
  uart::print(" ");
  if (cmp >= 15) {
    isTestDone = true;
  }
}

int main() {
  uart::test();

  uart::print("\nTesting light\n...");
  light::test();
  uart::print("\nDone testing light\n");

  // Memory test
  uart::print("\nTesting Memory\n...\n");
  mem::test();
  uart::print("\n\nDone testing Memory\n");

  // Testing can
  uart::print("\nTesting can\n");
  can::test();
  uart::print("Done testing can!\n");

  // PWM test
  uart::print("\nTesting PWM\n...");
  pwm::test();
  uart::print("\nDone testing PWM\n");

  // Testing sound
  uart::print("\nTesting sound\n...");
  sound::test();
  uart::print("\nDone testing sound!");

  // Testing timer
  uart::print("\n\nTesting the timer\n...");
  uart::print("\nTimer will count bellow\n");
  timer::test(100);

  while (!isTestDone)
    ;
  timer::off();

  uart::print("\n\nTests are all completed!");
  while (true)
    ;
}