#include <stdint.h>
#include <stm32f0xx.h>
#define SYSCLOCK 48000000u

void RCC_Config48MHz(){
	SET_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON);//включить клок секъюрити и тактирование от внешнего кварца
	SET_BIT(RCC->CFGR, RCC_CFGR_PLLSRC_HSE_PREDIV | RCC_CFGR_PLLMUL6);
	SET_BIT(RCC->CR, RCC_CR_PLLON);
	SET_BIT(RCC->CFGR, RCC_CFGR_SW_PLL);
}

void SysTick_Init(void){
	MODIFY_REG(SysTick->LOAD, SysTick_LOAD_RELOAD_Msk, SYSCLOCK/480-1);//значение переполнения счетчика. 4 тика в секунду
	CLEAR_BIT(SysTick->VAL, SysTick_VAL_CURRENT_Msk);//очистка текущего значения
	SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk| SysTick_CTRL_TICKINT_Msk);//источник тактирования счетчика (тактирование от системной частоты), счетчик включить, разрешить прерывания

}

int16_t period = 200, duration = 0;
int8_t direction = 0;
void SysTick_Handler(void){
	if(duration == 0) direction = 1;
	if(duration == period) direction = 0;
	if(direction) duration++;
	else duration--;
	TIM14->CCR1 = duration;
}

void TIM14_PWM(){
	SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN);
	RCC->APB1ENR |=RCC_APB1ENR_TIM14EN;//тактирование таймера

	//GPIOB->MODER |= GPIO_MODER_MODER1_1;//режим альтернативной функции
	//GPIOB->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEEDR1_Pos;//ничего не меняет, но типа устанвливает высокую скорость работы пина
	//GPIOB->AFR[0] |= 0 << GPIO_AFRL_AFSEL7_Pos;//0 вариант альтернативной функции

	GPIOA->MODER |= GPIO_MODER_MODER7_1;//режим альтернативной функции
	GPIOA->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEEDR7_Pos;//ничего не меняет, но типа устанвливает высокую скорость работы пина
	GPIOA->AFR[0] |= 4 << GPIO_AFRL_AFSEL7_Pos;//4 вариант альтернативной функции


	CLEAR_BIT(TIM14->CCMR1, TIM_CCMR1_CC1S_0);//режим работы на выход
	TIM14->CCMR1 |= 6<< TIM_CCMR1_OC1M_Pos;// шим, высокий уровень  пока cnt < ccr1
	TIM14->PSC= 20;//делитель
	TIM14->ARR = period;//длительность периода
	TIM14->CCR1 = 0;//длительность положительной фазы
	TIM14->CCER |= TIM_CCER_CC1E;//output enable
	TIM14->CR1 |= TIM_CR1_CEN;//включить таймер
}

int main(void)
{
	RCC_Config48MHz();
	SysTick_Init();
	TIM14_PWM();

	while(1)
	{


	}
}
