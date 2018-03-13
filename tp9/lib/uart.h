#ifndef UART_H
#define UART_H
#include <avr/io.h>
/**
 * Send various messages via uart. Can be used with the program serieViaUSB
 */

namespace uart {
/**
 * Sets all the registers necessary for the UART.
 * @return void
 */
void init();

/**
 * Prints to UART an array of character or a string.
 * @param c The array of character or the string
 * @param size The size of the array or string
 * @return void
 */
void print(const char *c, const uint8_t size);

/**
 * Prints to UART an array of character or a string. This method will
 * look for the null byte at the end of the string to determine the size
 * of it.
 * @param c The array of character or the string
 * @return void
 */
void print(const char *c);
/**
 * Prints a int to UART
 * @param n The int to print
 * @return void
 */
void print(const int n);

/**
 * Prints a long to UART
 * @param n The long to print
 * @return void
 */
void print(const long n);

/**
 * Prints a uint8_t to UART
 * @param n The byte to print
 * @return void
 */
void print(const uint8_t n);

/**
 * Prints a uint16_t to UART
 * @param n The uint16_t to print
 * @return void
 */
void print(const uint16_t n);

/**
 * Prints a uint32_t to UART
 * @param n The uint32_t to print
 * @return void
 */
void print(const uint32_t n);

/**
 * Prints a char to UART
 * @param n The char to print
 * @return void
 */
void print(const char c);

/**
 * Prints a new line to UART
 * @return void
 */
void println();

/**
 * Test all the uart functions. This method is mainly used for developpement
 * @return void
 */
void test();
}  // namespace uart
#endif
