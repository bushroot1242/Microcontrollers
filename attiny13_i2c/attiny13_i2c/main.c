#define F_CPU 9600000UL
#include <avr/io.h>
#include <util/delay.h>

#define SCL_pin 0
#define SDA_pin 1
#define WAIT _delay_ms(10);


void init_i2c(){
	DDRB |= (1 << SDA_pin) | (1 << SCL_pin);
	SET_HIGH(SDA_pin);
	SET_HIGH(SCL_pin);
}

void SET_LOW(uint8_t pin_num){
	PORTB &= ~(1 << pin_num);
}

void SET_HIGH(uint8_t pin_num){
	PORTB |= (1 << pin_num);
}

void start_transmit(){
	SET_LOW(SDA_pin);
	WAIT;
	SET_LOW(SCL_pin);
}
void stop_transmit(){
	SET_LOW(SDA_pin);
	WAIT;
	SET_HIGH(SCL_pin);
	WAIT;
	SET_HIGH(SDA_pin);
}

uint8_t send_byte(uint8_t byte){
	int i = 8;
	while(i--){
		if(byte & 1 << i) SET_HIGH(SDA_pin);
		else SET_LOW(SDA_pin);
		WAIT;
		SET_HIGH(SCL_pin);
		WAIT;
		SET_LOW(SCL_pin);
	}
	SET_HIGH(SDA_pin);
	WAIT;
	SET_HIGH(SCL_pin);
	WAIT;
	i = (PINB & (1 << SDA_pin));
	SET_LOW(SCL_pin);
	return i;
	
}



int main(void)
{
	init_i2c();

    /* Replace with your application code */
    while (1) 
    {
		start_transmit();
		send_byte(0x91);
		//send_byte(0x41);
		stop_transmit();
		_delay_ms(2000);

    }
}

