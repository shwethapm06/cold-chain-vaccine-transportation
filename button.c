#include "stm32f401xe.h"
#include <stdint.h>
#include "delay.h"
#include <stdio.h>
#include "button.h"



void Init_button(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA-> MODER &= ~(1<<20);
	GPIOA-> MODER &= ~(1<<21);

	GPIOA-> PUPDR |=(1<<21);
	GPIOA-> PUPDR &=~(1<<20);
}

int Read_button(void)
{
	if(GPIOA-> IDR &(1<<10))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
