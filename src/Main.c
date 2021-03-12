#include "stm32f10x.h"
#include "Main.h"
#include "MainProgram.h"

union Struct_PORTD PORTD;
union Struct_PORTE PORTE;

void GlobalInit(void);

int main(void) {
	
	__disable_irq ();
	
	SystemInit();
	GlobalInit();
	
	__enable_irq ();
	
	MainProgram();
	
}

void GlobalInit (void) {

	EXTI->IMR = EXTI->FTSR = EXTI->RTSR = 0;
	RCC->AHBENR |= RCC_AHBENR_CRCEN;


	EXTI->IMR |= EXTI_IMR_MR16;
	EXTI->RTSR |= EXTI_RTSR_TR16;



}
