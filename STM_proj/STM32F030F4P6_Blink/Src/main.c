
#include <stdint.h>
#include <stm32f0xx.h>

int main(void)
{
	   RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	   GPIOA->MODER |= GPIO_MODER_MODER4_0;
	   GPIOA->MODER &= ~GPIO_MODER_MODER4_1;
	   GPIOA->MODER |= GPIO_MODER_MODER14_0;
	   GPIOA->MODER &= ~GPIO_MODER_MODER14_1;
	   GPIOA->MODER |= GPIO_MODER_MODER13_0;
	   GPIOA->MODER &= ~GPIO_MODER_MODER13_1;

	   //GPIOA->MODER |= 0x555;
   while(1){


	   //GPIOA->ODR |= GPIO_ODR_4;
	   //GPIOB->ODR |= GPIO_ODR_1;

	   //GPIOA->BSRR |=GPIO_BSRR_BS_4;
	   //for(int i = 0; i<1000000;i++){}
	  // GPIOA->BSRR |=GPIO_BSRR_BR_4;

	   GPIOA -> ODR ^=  GPIO_ODR_4;
	   GPIOA -> ODR ^=  GPIO_ODR_14;
	   GPIOA -> ODR ^=  GPIO_ODR_13;
	   for(int i = 0; i<1000000;i++){}
	   //GPIOA -> ODR |= GPIO_ODR_4;
	   //for(int i = 0; i<1000000;i++){}
	   //GPIOB->ODR &= ~GPIO_ODR_1;
	   //

	   //for(int i = 0; i<1000000;i++){}

   }
}
