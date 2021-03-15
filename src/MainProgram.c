#include "MainProgram.h"
#include "stm32f10x.h"
#include "Display.h"

extern uint8_t ALPH[];

mainStruct_TypeDef petFeeder;

uint8_t buttonPoll(void);

void MainProgram(void) {
	uint32_t tempReg = 0; 
	uint8_t symbolCode = 0, symbolCounter = 0;
	char symbol = 0;
	
	GPIOA->BSRR |= 0x01;
	GPIOC->BSRR |= (0x01 << 13);
	LCDInit();
	while(1) {
		msDelay(100);
		if(GPIOC->ODR & GPIO_ODR_ODR13) {
			GPIOC->BSRR |= (0x01 << (13+16));
			petFeeder.FLAGS.bit.render = 1;
		} else {
			GPIOC->BSRR |= (0x01 << (13));
			petFeeder.FLAGS.bit.render = 0;
		}
		tempReg = GPIOB->IDR;
		tempReg &= 0x0800;
		
		if(!tempReg){//buttonPoll()) {
			tempReg = 0;
			GPIOB->BSRR |= (0x01 << 1);
		} else {
			GPIOB->BSRR |= (0x01 << (1+16));
		}
		
		if(petFeeder.FLAGS.bit.render) {
			
			commandToDisplay(CURSOR_OFF);
			lineIndication (1, " abcdE", 1);
			lineIndication (2, " F��hkl�Ԕ", 1);
			commandToDisplay(FIRST_LINE);
			commandToDisplay(CURSOR_ON);
		}
	}
}

uint8_t buttonPoll(void) {
	uint16_t i, j;
	uint8_t buttons = 0, buttonState = 0, btnPressed = 0;
	uint8_t checkArray[10] = {0,0,0,0,0,0,0,0,0,0};
	uint32_t tempReg = 0;
	
	for(i = 0; i < 10; i++) {
		tempReg = GPIOB->IDR;
		tempReg &= 0x01;
		if(!tempReg) {
			checkArray[i] = 1;
		} else {
			checkArray[i] = 0;
		}
		if(i == 9) {
			btnPressed = 1;
			for(j = 0; j < 10; j++) {
				if(checkArray[j] != 1) btnPressed = 0;
			}				
		}
	}
	
	if(btnPressed) buttons = 0x01;
	
	return buttons;
}


void usDelay(uint16_t delayCounter)
{
uint16_t i = 0; 
uint8_t usFlag = 0;
	
	TIM2->CR1 = 0x00000000;
	TIM2->SR = 0;
	TIM2->CNT = 0x00000000;
	TIM2->PSC = 0x00000000;

	TIM2->ARR = 0x00000008;
	TIM2->CR1 = 0x00000001;
	
	for (i=0; i < delayCounter; i++)
	{
		TIM2->CR1 = 0x00000000;
		TIM2->SR = 0;
		TIM2->CNT = 0x00000000;
		TIM2->CR1 = 0x00000001;
		
		while (!usFlag)		
		{
			if (TIM2->SR & 0x1)	//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
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
	
	TIM2->CR1 = 0x00000000;
	TIM2->SR = 0;
	TIM2->CNT = 0x00000000;
	TIM2->PSC = 0x00000000;

	TIM2->ARR = 0x00001F40;
	TIM2->CR1 = 0x00000001;
	
	for (i=0; i < delayCounter; i++)
	{
		TIM2->CR1 = 0x00000000;
		TIM2->SR = 0;
		TIM2->CNT = 0x00000000;
		TIM2->CR1 = 0x00000001;
		
		while (!msFlag)		
		{
			if (TIM2->SR & 0x1)	//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			{
				msFlag = 1;
			}
		}
		msFlag = 0;
	}
}


