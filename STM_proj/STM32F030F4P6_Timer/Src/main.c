#include <stdint.h>
#include <stm32f0xx.h>
#define SYSCLOCK 48000000u


void RCC_Config48MHz(){
	SET_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON);//включить клок секъюрити и тактирование от внешнего кварца
	SET_BIT(RCC->CFGR, RCC_CFGR_PLLSRC_HSE_PREDIV | RCC_CFGR_PLLMUL6);
	SET_BIT(RCC->CR, RCC_CR_PLLON);
	SET_BIT(RCC->CFGR, RCC_CFGR_SW_PLL);
}

void LED_Config(){

	SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN);
	SET_BIT(GPIOA->OSPEEDR,GPIO_OSPEEDR_OSPEEDR4_0|GPIO_OSPEEDR_OSPEEDR4_1);
	SET_BIT(GPIOA->MODER, GPIO_MODER_MODER4_0);

}

void TIMER14_Start(){
	TIM14->CR1 |= TIM_CR1_CEN;//включить счетчик
}

void TIMER14_Config(){
	RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;//включить тактирование таймера
	TIM14->PSC = SYSCLOCK/1000;//делитель частоты счетчика
	TIM14->ARR = 1000;//загружаемое при переполнении значение
	TIM14->DIER |= TIM_DIER_UIE;//разрешить прерывания по переполнению
	NVIC_EnableIRQ(TIM14_IRQn);//разрешить внешнее прерывание
}
void TIM14_IRQHandler(){
	GPIOA -> ODR ^=  GPIO_ODR_4;//мигаем свевтодиодом
	TIM14->SR &=~TIM_SR_UIF;//сбрасываем флаг прерывания обновления

}


volatile uint16_t period = 7;//задержка 2 секунды (4*2-1)
void SysTick_Handler(void){
	if(period<1)
	{
		GPIOA -> ODR ^=  GPIO_ODR_4;
		CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
		TIMER14_Start();
	}
	else
		period --;
}

void SysTick_Init(void){
	MODIFY_REG(SysTick->LOAD, SysTick_LOAD_RELOAD_Msk, SYSCLOCK/4-1);//значение переполнения счетчика. 4 тика в секунду
	CLEAR_BIT(SysTick->VAL, SysTick_VAL_CURRENT_Msk);//очистка текущего значения
	SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk| SysTick_CTRL_TICKINT_Msk);//источник тактирования счетчика (тактирование от системной частоты), счетчик включить, разрешить прерывания

}

int main(void)
{
	RCC_Config48MHz();
	LED_Config();
	TIMER14_Config();
	SysTick_Init();
	while(1)
	{

	//	GPIOA -> ODR ^=  GPIO_ODR_4;
	//	for(int i = 0; i< 100000;i++);

	}
}
