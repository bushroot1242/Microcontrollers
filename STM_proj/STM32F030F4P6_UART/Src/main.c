#include <stdint.h>
#include <stm32f0xx.h>
#define SYSCLOCK 48000000u

void RCC_Config48MHz(){
	SET_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON);//включить клок секъюрити и тактирование от внешнего кварца
	SET_BIT(RCC->CFGR, RCC_CFGR_PLLSRC_HSE_PREDIV | RCC_CFGR_PLLMUL6);
	SET_BIT(RCC->CR, RCC_CR_PLLON);
	SET_BIT(RCC->CFGR, RCC_CFGR_SW_PLL);
}

void UART_Config(){
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |=(GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1);
	GPIOA->AFR[0] |=1 << GPIO_AFRL_AFRL2_Pos | 1<<GPIO_AFRL_AFRL3_Pos;
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	USART1->BRR = 5000;//48000000/(16*9600)*16
	//USART1->BRR = 0x1a1;//115200 бод
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE | USART_CR1_TCIE |//включить прием, передачу, ЮАРТ, минимальная настройка Уарт-------
	  USART_CR1_RXNEIE; //разрешить прерывания по приему, и еще какие то USART_CR1_TXEIE |
	USART1->CR3 |= USART_CR3_OVRDIS;
	//NVIC_SetPriority(USART1_IRQn, 0);
	NVIC_EnableIRQ(USART1_IRQn);
}

void SysTick_Init(void){
	MODIFY_REG(SysTick->LOAD, SysTick_LOAD_RELOAD_Msk, SYSCLOCK/4-1);//значение переполнения счетчика.200 переполнений в секунду
	CLEAR_BIT(SysTick->VAL, SysTick_VAL_CURRENT_Msk);//очистка текущего значения
	SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk);//источник тактирования счетчика (тактирование от системной частоты), разрешить прерывания

}

void LED_Config(){

	SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN);
	SET_BIT(GPIOA->OSPEEDR,GPIO_OSPEEDR_OSPEEDR4_0|GPIO_OSPEEDR_OSPEEDR4_1);
	SET_BIT(GPIOA->MODER, GPIO_MODER_MODER4_0);
	SET_BIT(GPIOA -> ODR,  GPIO_ODR_4);


}

void UART_write(char data){
	USART1->TDR = data;
}

uint8_t recived, blinkCnt;
void USART1_IRQHandler(){

	USART1->ICR |= USART_ICR_TCCF;//tranmission complite clear flag

	if(USART1->ISR & USART_ISR_ORE)//получено новое, старое еще не считано
	{
		USART1->ICR = USART_ICR_ORECF;
		return;

	}
	if(USART1->ISR & USART_ISR_FE)//нарушение фрейма
	{
		USART1->ICR = USART_ICR_FECF;
		return;

	}
	if((USART1->ISR & USART_ISR_TXE) & (USART1->CR1 & USART_CR1_TXEIE)){//буфер передачи пуст и готов к отправке след байта

	}

	if(USART1->ISR & USART_ISR_RXNE)//проверка наполнения буфера чтения
	{
		recived = USART1->RDR;
		SET_BIT(SysTick->CTRL,SysTick_CTRL_ENABLE_Msk);//счетчик включить

	}


}

void SysTick_Handler(void){

	if(blinkCnt)
	{
		GPIOA -> ODR ^=  GPIO_ODR_4;
		blinkCnt --;
	}
	else
	{
		CLEAR_BIT(SysTick->CTRL,SysTick_CTRL_ENABLE_Msk);//счетчик выключить
	}

}

int main(void)
{
	RCC_Config48MHz();
	UART_Config();
	LED_Config();
	SysTick_Init();

	while(1){
	if(recived)
	{
		if((recived > 48) & (recived<58))
			{
				blinkCnt = (recived-48)*2;
				SET_BIT(SysTick->CTRL,SysTick_CTRL_ENABLE_Msk);//счетчик включить
			}
			else
			{
				UART_write((uint8_t)'x');
				UART_write((uint8_t)'z');
			}
		recived = 0;
	}
  }
}



