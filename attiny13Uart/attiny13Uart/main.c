#include "main.h"


int main(void)
{
	uart_init();
	_delay_ms(50);
	uint8_t *rb = 0;
	while (1)
	{
		uart_recieve(rb);
		if (*rb>0){
			uart_send(*rb);
			*rb=0;
		}

	}
}

