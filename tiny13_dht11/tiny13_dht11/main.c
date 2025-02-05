/*
 * tiny13_dht11.c
 *
 * Created: 31.01.2025 9:46:13
 * Author : User
 */ 
#define F_CPU 9600000UL
#include <avr/io.h>
#include "util/delay.h"

void LOW(){
	DDRB|= 1 << PB0;
}
void HIGH(){
	DDRB &= ~(1 << PB0);
}

uint8_t readBit(){
	return PINB & (1 << PB0);
}

volatile uint8_t byte = 0, i;
uint8_t readByte(){
	byte = 0;
	i = 8;
	while(i--){
		while(!(readBit()));
		_delay_us(40);
		if(readBit()){
			byte |= (1 << i);
			while(readBit());
		}
		
	}
	return byte;
}

uint8_t start(){
	LOW();
	_delay_ms(20);
	HIGH();
	_delay_us(40);
	if(readBit())return 1;
	_delay_us(80);
	if(!(readBit()))return 1;
	while(readBit());
	return 0;
	
}

//volatile uint8_t answer_ = 0, i = 0;
volatile uint8_t data[5], result;
int main(void)
{

    while (1) 
    {
		_delay_ms(2000);
		 if(!start())
		 {
			 data[0] = readByte();
			 data[1] = readByte();
			 data[2] = readByte();
			 data[3] = readByte();
			 data[4] = readByte();
			 
		 }
		 
    }
}

