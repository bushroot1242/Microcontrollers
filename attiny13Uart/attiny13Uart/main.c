#include "main.h"

int main(void)
{
	uart_init();
	uint8_t i = 65;
	_delay_ms(5);
	while (1)
	{


		uart_send(i++);
		_delay_ms(2);

	}
}

