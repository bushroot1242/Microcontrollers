
#include <avr/io.h>
#include <avr/interrupt.h>


#define BAUD_RATE 9600
#define F_CPU 9600000
#define DIV 8
#define TXPORT 0
#define RXPORT 1

volatile uint8_t txByte;

void uart_init();
void uart_send(uint8_t);