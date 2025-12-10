/*Настройка источника тактирование (внешний кварц)
 * мигание светодиодом по событию переполнения системного таймера
 *
 *
 */
#include <stdint.h>
#include <stm32f0xx.h>
#define SYSCLOCK 48000000u

void SysTick_Init(void){
	MODIFY_REG(SysTick->LOAD, SysTick_LOAD_RELOAD_Msk, SYSCLOCK/1-1);//значение начала отсчета счетчика
	CLEAR_BIT(SysTick->VAL, SysTick_VAL_CURRENT_Msk);//очистка текущего значения
	SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk| SysTick_CTRL_TICKINT_Msk);//источник тактирования счетчика (PLL), разрешить прерывания
}

//uint16_t period = 0;
void SysTick_Handler(void){
	//MODIFY_REG(SysTick->VAL, SysTick_VAL_CURRENT_Msk, SYSCLOCK);//взвели систик на 1000
	//if(period>0) period --;
	//else
		{
			//period = 500;
			GPIOA -> ODR ^=  GPIO_ODR_4;
		}
}


int main(void)
{
	 RCC->CR |= RCC_CR_HSEON | RCC_CR_CSSON ;//включить клок секъюрити и тактирование от внешнего кварца
	 RCC->CFGR  |= RCC_CFGR_PLLSRC_HSE_PREDIV | RCC_CFGR_PLLMUL6;
	 RCC->CR |= RCC_CR_PLLON;
	 RCC->CFGR |= RCC_CFGR_SW_PLL;
	 SysTick_Init();




	 uint32_t delay = 100000, sh_delay = 1, l_delay = 1;



	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;
	GPIOA->MODER |= GPIO_MODER_MODER4_0;
	GPIOA->MODER &= ~GPIO_MODER_MODER4_1;

	for(;;){






	}

}





