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
	MODIFY_REG(SysTick->LOAD, SysTick_LOAD_RELOAD_Msk, SYSCLOCK/4-1);//значение переполнения счетчика.200 переполнений в секунду
	CLEAR_BIT(SysTick->VAL, SysTick_VAL_CURRENT_Msk);//очистка текущего значения
	SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk);//источник тактирования счетчика (тактирование от системной частоты), разрешить прерывания

}



void TIM14_Capture_Config(){
	SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN);
	RCC->APB1ENR |=RCC_APB1ENR_TIM14EN;//тактирование таймера

	GPIOA->MODER |= GPIO_MODER_MODER7_1;//режим альтернативной функции
	GPIOA->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEEDR7_Pos;//ничего не меняет, но типа минимизирует время фронта сигнала
	GPIOA->AFR[0] |= 4 << GPIO_AFRL_AFSEL7_Pos;//4 вариант альтернативной функции

	TIM14->CCMR1 = 1 << TIM_CCMR1_CC1S_Pos;//режим работы на вход
	//TIM14->CCMR1 |= 3 << TIM_CCMR1_IC1F_Pos;//фильтр 8

	TIM14->CCER |= TIM_CCER_CC1E | TIM_CCER_CC1NP | 3 << TIM_CCER_CC1E_Pos;//запись счетчика в регистр ccr1
	TIM14->PSC = 480;
	NVIC_EnableIRQ(TIM14_IRQn);
	TIM14->DIER =TIM_DIER_CC1IE;//разрешить прерывания по захвату/сравнению
	TIM14-> CR1 |= TIM_CR1_CEN;//включить счетчик




}

int32_t cnt1, cnt2, impLen;
int8_t gap, mas_cnt;
int32_t mas[100];

void TIM14_IRQHandler(){

	//GPIOA -> ODR ^=  GPIO_ODR_4;
	//если разрешено прерывание по захвату и поднят флаг прерывания по захвату
	if((TIM14->DIER & TIM_DIER_CC1IE) & (TIM14->SR & TIM_SR_CC1IF)){

		//если  был перезахват
		if((TIM14->SR & TIM_SR_CC1OF) != 0){

			gap = 0;//обнуляем флаг
			CLEAR_REG(TIM14->SR);//снимаем флаги событий
			return;
		}
		if(gap == 0)
		{
			cnt1=TIM14->CCR1;//записываем счет
			gap=1;//поднимаем флаг начала отсчета
		}
		else
		{
			cnt2=TIM14->CCR1;
			if(cnt2>cnt1)
				impLen=cnt2-cnt1;
			else
				impLen = cnt2 + 0xffff - cnt1 + 1;
			mas[mas_cnt] = impLen;
			if(mas_cnt < 100) mas_cnt++;
			else
				mas_cnt = 0;
			cnt1=cnt2;

		}
		//CLEAR_BIT(TIM14->DIER, TIM_DIER_CC1IE);
	}
}

void LED_Config(){

	SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN);
	SET_BIT(GPIOA->OSPEEDR,GPIO_OSPEEDR_OSPEEDR4_0|GPIO_OSPEEDR_OSPEEDR4_1);
	SET_BIT(GPIOA->MODER, GPIO_MODER_MODER4_0);

}

void SysTick_Handler(void)
{
	GPIOA -> ODR ^=  GPIO_ODR_4;
}


int main(void)
{
	RCC_Config48MHz();
	LED_Config();
	SysTick_Init();
	TIM14_Capture_Config();

	for(int i = 0; i < 2;i++){
		for(int j = 0; j < 480000;j++){

		}
	}
	SET_BIT(SysTick->CTRL,SysTick_CTRL_ENABLE_Msk);//счетчик включить

	while(1)
	{


	}
}

