#include "MainProgram.h"
#include "stm32f10x.h"
#include "Display.h"

void MainProgram(void) {

	LCDInit();
}


void usDelay(uint16_t delayCounter)
{
uint16_t i = 0; 
uint8_t usFlag = 0;
	
	TIM1->CR1 = 0x00000000;
	TIM1->SR = 0;
	TIM1->CNT = 0x00000000;
	TIM1->PSC = 0x00000000;

	TIM1->ARR = 0x00000096;
	TIM1->CR1 = 0x00000001;
	
	for (i=0; i < delayCounter; i++)
	{
		TIM1->CR1 = 0x00000000;
		TIM1->SR = 0;
		TIM1->CNT = 0x00000000;
		TIM1->CR1 = 0x00000001;
		
		while (!usFlag)		
		{
			if (TIM1->SR & 0x1)	//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			{
				usFlag = 1;
			}
		}
		usFlag = 0;
	}
}

void msDelay(uint16_t delayCounter)
{
	uint16_t i = 0; 
	uint8_t msFlag = 0;
	
	TIM1->CR1 = 0x00000000;
	TIM1->SR = 0;
	TIM1->CNT = 0x00000000;
	TIM1->PSC = 0x00000002;

	TIM1->ARR = 0x0000C350;
	TIM1->CR1 = 0x00000001;
	
	for (i=0; i < delayCounter; i++)
	{
		TIM1->CR1 = 0x00000000;
		TIM1->SR = 0;
		TIM1->CNT = 0x00000000;
		TIM1->CR1 = 0x00000001;
		
		while (!msFlag)		
		{
			if (TIM1->SR & 0x1)	//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			{
				msFlag = 1;
			}
		}
		msFlag = 0;
	}
}


