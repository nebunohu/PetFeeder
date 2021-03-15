#include "stm32f10x.h"
#include "Main.h"
#include "MainProgram.h"

union Struct_PORTA PORTA;
union Struct_PORTB PORTB;
union Struct_PORTD PORTD;

union Struct_PORTE PORTE;

void GlobalInit(void);
void GPIOInit(void);

int main(void) {
	
	__disable_irq ();
	
	SystemInit();
	GlobalInit();
	GPIOInit();
	
	__enable_irq ();
	
	MainProgram();
	
}

void GlobalInit (void) {

	EXTI->IMR = EXTI->FTSR = EXTI->RTSR = 0;
	RCC->AHBENR |= RCC_AHBENR_CRCEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM3EN;
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;


	EXTI->IMR |= EXTI_IMR_MR16;
	EXTI->RTSR |= EXTI_RTSR_TR16;
	



}

void GPIOInit(void) {
	uint32_t tempReg = 0;
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN;
	
	tempReg = GPIOA->CRL;
	tempReg &= 0x00000000;
	tempReg |= (0x01 << (0*4)) | (0x00 << ((0*4)+2)) | (0x01 << (1*4)) | (0x00 << ((1*4)+2)) 
					|  (0x01 << (2*4)) | (0x00 << ((2*4)+2)) | (0x01 << (3*4)) | (0x00 << ((3*4)+2))
					|  (0x01 << (4*4)) | (0x00 << ((4*4)+2)) | (0x01 << (5*4)) | (0x00 << ((5*4)+2)) 
					|  (0x01 << (6*4)) | (0x00 << ((6*4)+2)) | (0x01 << (7*4)) | (0x00 << ((7*4)+2));
	GPIOA->CRL = tempReg;
	
	tempReg = GPIOB->CRL;
	tempReg &= 0xFFFFFF00;
	tempReg |= /*(0x00 << (0*4)) | (0x01 << ((0*4)+2)) |*/ (0x01 << (1*4)) | (0x00 << ((1*4)+2));
	GPIOB->CRL = tempReg;
	
	tempReg = GPIOB->CRH;
	tempReg &= 0xFF0000FFF;
	tempReg |= (0x00 << ((11-8)*4)) | (0x02 << (((11-8)*4)+2)) | (0x01 << ((12-8)*4)) | (0x00 << (((12-8)*4)+2)) | (0x01 << ((13-8)*4)) | (0x00 << (((13-8)*4)+2)) 
					|  (0x01 << ((14-8)*4)) | (0x00 << (((14-8)*4)+2));
	GPIOB->CRH = tempReg;
	
	tempReg = GPIOC->CRH;
	tempReg &= 0xff0ffffF;
	tempReg |= (0x01 << ((13-8)*4)) | (0x00 << ((13-8)*4+2));
	GPIOC->CRH = tempReg;
}
