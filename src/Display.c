#include "stm32f10x.h"
#include "Main.h"
#include "MainProgram.h"
#include "Display.h"

extern union Struct_PORTD PORTD;
extern union Struct_PORTE PORTE;

uint8_t ALPH[256] = {
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0xDA, 0x3D, 0xD9, 0x3F,
	0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
	0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
	0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7E, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB0,
	0x99, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x41, 0xA0, 0x42, 0xA1, 0xE0, 0x45, 0xA3, 0xA4, 0xA5, 0xA6, 0x4B, 0xA7, 0x4D, 0x48, 0x4F, 0xA8, 
	0x50, 0x43, 0x54, 0xA9, 0xAA, 0x58, 0xE1, 0xAB, 0xAC, 0xE2, 0xAD, 0xAE, 0x08, 0xAF, 0xB0, 0xB1,
	0x61, 0xB2, 0xB3, 0xB4, 0xE3, 0x65, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0x6F, 0xBE,
	0x70, 0x63, 0xBF, 0x79, 0xE4, 0x78, 0xE5, 0xC0, 0xC1, 0xE6, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7};

void LCDInit(void) {
	PORTD.bit.E = 0; 
	GPIOD->ODR = PORTD.all;
	usDelay(20000);

//При необходимости настроить здесь шину данных на вывод
	PORTD.bit.R_W = 0;
	PORTD.bit.AO = 0;
	
	GPIOD->ODR = PORTD.all;
	//Установка типа интерфейса (8бит) (0х03 - старший полубайт команды 0х30) - в начале ВСЕГДА переключаем индикатор в 8-ми битный режим работы 
	
	sendHalfByte(0x03);
	usDelay(90);//Пауза между командами
	
	strobe();
	usDelay(90); //Пауза между командами
	
	strobe();
	usDelay(45); //Пауза между командами
	
	//Установка типа интерфейса (4 бит) (0x02 - старший полубайт команды 0x20) (при этом биты RW и AO ихмениться не должны!)
	sendHalfByte(0x02);
	usDelay(90);
	
	sendByte(0x28);
	usDelay(90);
	sendByte(0x08);
	usDelay(90);
	sendByte(0x0d);
	usDelay(90);
	sendByte(0x06);
	usDelay(90);
	 
	commandToDisplay(CLEAR_DISPLAY);
}

void strobe(void) {
	usDelay(10);	
	PORTD.bit.E = 1;
	GPIOD->ODR = PORTD.all;
	usDelay(10);	
	PORTD.bit.E = 0;
	GPIOD->ODR = PORTD.all;
	usDelay(10);	
}

void sendHalfByte(uint8_t hByte) {
	PORTE.bit.DB7 = (hByte & 0x08) ? 1 : 0;
	PORTE.bit.DB6 = (hByte & 0x04) ? 1 : 0;
	PORTE.bit.DB5 = (hByte & 0x02) ? 1 : 0;
	PORTE.bit.DB4 = (hByte & 0x01) ? 1 : 0;
	GPIOE->ODR = PORTE.all;
	strobe();
}
void sendByte(uint8_t byte) {
	uint8_t hByte = 0;
	hByte = (byte >> 4) & 0x0F;
	sendHalfByte(hByte);
	hByte = byte & 0x0F;
	sendHalfByte(hByte);
}



void displayConfig(void)
{
	
	// ïàóçû 20 ìñ è  120 ìñ
	msDelay(120);
	commandToDisplay(CURSOR_OFF); 
	commandToDisplay(CLEAR_DISPLAY); 		// 0x01 
	commandToDisplay(CURSOR_HOME); 			// 0x02
	commandToDisplay(FS_DL0_N1_BR00);		// 0x28
	commandToDisplay(CGRAM_ADDRESS_0);	// 0x40
	commandToDisplay(EMS_ID1_S0);				// 0x06
	
	commandToDisplay(FIRST_LINE); 		// 0x80
	commandToDisplay(CLEAR_DISPLAY); 	// 0x01
	
	return;
}

void eDelay(void)
{
/*__IO uint8_t counter = 2;

	while(counter--);

	return;*/
	uint8_t del = 80;
	TIM1->CR1 = 0x00000000;
	TIM1->CNT = 0x00000000;
	TIM1->PSC = 0x00000001;
	TIM1->SR = 0;
	
	TIM1->ARR = del;
	TIM1->CR1 = 0x00000001;
	while (!(TIM1->SR & 0x1))		//åñëè òàéìåð îòñ÷èòàë óñòàâêó
		{

		}
	TIM1->CR1 = 0x00000000;
	return;
}

void commandToDisplay(uint8_t command)
{static uint8_t tmrCnt = 0;
	
	PORTD.bit.AO = 0;
	PORTD.bit.R_W = 0;
	PORTD.bit.E = 0;
	
	GPIOD->ODR = PORTD.all;
	
	//
	//sendByte(command);
	//---------------------------------
	PORTE.bit.DB7 = (command & 0x80) ? 1 : 0;
	PORTE.bit.DB6 = (command & 0x40) ? 1 : 0;
	PORTE.bit.DB5 = (command & 0x20) ? 1 : 0;
	PORTE.bit.DB4 = (command & 0x10) ? 1 : 0;
	GPIOE->ODR = PORTE.all;
	
	usDelay(1);	// 20ns
	
	PORTD.bit.E = 1;
	GPIOD->ODR = PORTD.all;
	
	usDelay(1);	// 250ns
		
	PORTD.bit.E = 0;
	GPIOD->ODR = PORTD.all;
	
	usDelay(1);	// 120ns
	
	PORTE.bit.DB7 = (command & 0x08) ? 1 : 0;
	PORTE.bit.DB6 = (command & 0x04) ? 1 : 0;
	PORTE.bit.DB5 = (command & 0x02) ? 1 : 0;
	PORTE.bit.DB4 = (command & 0x01) ? 1 : 0;
	GPIOE->ODR = PORTE.all;
	
	usDelay(1);	// 120ns
	
	PORTD.bit.E = 1;
	GPIOD->ODR = PORTD.all;
	
	usDelay(1);	// 250ns
		
	PORTD.bit.E = 0;
	GPIOD->ODR = PORTD.all;
	//---------------------------------
	
	usDelay(1);	// 20ns
	
	TIM1->SR = 0;
	TIM1->CR1 = 0x00000000;
	TIM1->CNT = 0x00000000;
	
	if (command == 1)
	{
		//òàéìåð 3 ìñ
		TIM1->PSC = 0x00000008;
		TIM1->ARR = 0xC350;
	}
	else
	{
		//òàéìåð 100 ìêñ
		TIM1->PSC = 0x00000001;
		TIM1->ARR = 0x1D4C;
			
	}

	/*if (pult.flags.bit.displayInit != 1)
	{
		TIM1->CR1 = 0x00000001;
	}*/
	
	if (TIM1->CNT > 0) tmrCnt++;
	
	PORTD.bit.AO = 0;
	PORTD.bit.R_W = 1;
	PORTD.bit.E = 0;
	//GPIO_Lock (GPIOE, 0xFBFF );
	GPIOE->CRL &= ~(0x03 << 10*2); // óñòàíîâêà 10 áèòà ïîðòà E íà âõîä, ÷òåíèå ôëàãà çàíÿòîñòè
	//GPIO_Lock (GPIOE, 0xFFFF );
	
	GPIOD->ODR = PORTD.all;
	
	usDelay(1);	// 20ns
// ============================================	
	PORTD.bit.E = 1;
	GPIOD->ODR = PORTD.all;
	eDelay();
	
/*	while (GPIOA->ODR & GPIO_IDR_IDR_10)
	{
		if (TIM1->SR & 0x1)		//åñëè òàéìåð îòñ÷èòàë óñòàâêó
		{
			errorCounter[0]++;
			break;
		}
	}*/
	//while((GPIOE->IDR & GPIO_IDR_IDR_10) > 0) {}
	while(PORTE.bit.DB7)
	{
		PORTE.all = GPIOE->IDR;
	}
	PORTD.bit.E = 0;
	GPIOD->ODR = PORTD.all;	
	PORTD.bit.E = 1;
	eDelay();
	GPIOD->ODR = PORTD.all;
	eDelay();
	PORTD.bit.E = 0;
		
	GPIOD->ODR = PORTD.all;
	//GPIO_Lock (GPIOE, 0xFBFF );
	GPIOE->CRL |= (0x01 << 10*2);			// Установка 10 бита порта E на вход
	//GPIO_Lock (GPIOE, 0xFFFF );
	TIM1->CR1 = 0x00000000;
		
	usDelay(20);
	return;
	
}

void symbolIndication (uint8_t symbolCode, uint8_t symbolCounter) 
{
	if (symbolCounter > 20) return;
	
	PORTD.bit.AO = 1;
	PORTD.bit.R_W = 0;
	PORTD.bit.E = 0;
	
	//sendByte(command);
	//------------------------------------
	PORTE.bit.DB7 = (symbolCode & 0x80) ? 1 : 0;
	PORTE.bit.DB6 = (symbolCode & 0x40) ? 1 : 0;
	PORTE.bit.DB5 = (symbolCode & 0x20) ? 1 : 0;
	PORTE.bit.DB4 = (symbolCode & 0x10) ? 1 : 0;

	GPIOD->ODR = PORTD.all;
	GPIOE->ODR = PORTE.all;
	

	usDelay(1);//20ns

	PORTD.bit.E = 1;
	GPIOD->ODR = PORTD.all;

	usDelay(1);//250ns

	PORTD.bit.E = 0;
	GPIOD->ODR = PORTD.all;


	usDelay(1);//120ns

	PORTE.bit.DB7 = (symbolCode & 0x08) ? 1 : 0;
	PORTE.bit.DB6 = (symbolCode & 0x04) ? 1 : 0;
	PORTE.bit.DB5 = (symbolCode & 0x02) ? 1 : 0;
	PORTE.bit.DB4 = (symbolCode & 0x01) ? 1 : 0;

	GPIOE->ODR = PORTE.all;
	
	usDelay(1);//120ns

	PORTD.bit.E = 1;
	GPIOD->ODR = PORTD.all;
	
	usDelay(1);//250ns

	PORTD.bit.E = 0;
	GPIOD->ODR = PORTD.all;

	usDelay(1);//20ns
	//-------------------------------------
	TIM1->CR1 = 0x00000000;
	TIM1->CNT = 0x00000000;
	TIM1->PSC = 0x00000001;
	TIM1->SR = 0;
	TIM1->ARR = 0x1D4C;
	TIM1->CR1 = 0x00000001;
	

	GPIOE->CRL &= ~(0x3 << 20); // óñòàíîâêà 11 áèòà ïîðòà E íà âõîä

	PORTD.bit.AO = 0;
	PORTD.bit.R_W = 1;
	GPIOD->ODR = PORTD.all;

	usDelay(1);//20ns
// ======================================
	PORTD.bit.E = 1;
	GPIOD->ODR = PORTD.all;

/*	while (GPIOA->ODR & (0x1 << 3))
	{
		if (TIM1->STATUS & 0x2)		//åñëè òàéìåð îòñ÷èòàë óñòàâêó
		{errorCounter[1]++;
			TIM1->CNTRL = 0x00000000;
			break;
		}
	}*/
	while((GPIOA->IDR & GPIO_IDR_IDR10) > 0)
	{}
	PORTD.bit.E = 0;
	GPIOD->ODR = PORTD.all;

	GPIOE->CRL |= (0x1 << 20);			// óñòàíîâêà 11 áèòà ïîðòà e íà âûõîä

	usDelay(1);//20ns
		
	usDelay(20);

}

uint8_t lineSelection(uint8_t lineNumber)
{
uint8_t command=0;

	switch (lineNumber)
	{
		case 1:
			command = FIRST_LINE;	// 1000000
		break;
		case 2:
			command = SECOND_LINE;	
		break;
		case 3:
			command = THIRD_LINE;
		break;
		case 4:
			command = FOURTH_LINE;
		break;
	}
	
	return command;
}

void lineFilling(uint8_t symbolCounter)
{
uint8_t symbolCode = 0;
char symbol = 0;
	// îêîí÷àíèå ñòðîêè
	while ((20 - symbolCounter) > 0)
	{
		symbol = ' ';
		symbolCode = ALPH[symbol];		
		symbolIndication(symbolCode, symbolCounter);
		symbolCounter++;
	}
}

uint8_t lineIndication (uint8_t displayLineNumber, char *tablesLine, uint8_t lineFillingFlag)
{
uint8_t symbolCode = 0, command = 0, symbolCounter = 0;
char symbol;

	command = lineSelection(displayLineNumber);

	// âûâîä ñòðîêè
	commandToDisplay(command);
	while (symbol = (*tablesLine++))
	{
		symbolCounter++;
		symbolCode = ALPH[symbol];
		symbolIndication(symbolCode, symbolCounter);	
	}
	
	// îêîí÷àíèå ñòðîêè
	if (lineFillingFlag) 
	{
		lineFilling(symbolCounter);
	}
	return (symbolCounter);
}

void oneInt(uint16_t number, uint8_t symbolCounter)
{
int16_t mod = 0;
uint8_t symbolCode = 0;

	mod = number % 10;
	symbolCode = ALPH[mod];
	symbolIndication(symbolCode, symbolCounter);
}

void tenInt(uint16_t number, uint8_t symbolCounter)
{
int16_t mod = 0, temp = 0;
uint8_t symbolCode = 0;
	
	mod = number % 100;
	temp = mod / 10;
	symbolCode = ALPH[temp];
	symbolIndication(symbolCode, symbolCounter);
}

void hundredInt(uint16_t number, uint8_t symbolCounter)
{
int16_t mod = 0, temp = 0;
uint8_t symbolCode = 0;
	
	mod = number % 1000;
	temp = mod / 100;
	symbolCode = ALPH[temp];
	symbolIndication(symbolCode, symbolCounter);
}

void thousandInt(uint16_t number, uint8_t symbolCounter)
{
int16_t mod = 0, temp = 0;
uint8_t symbolCode = 0;
	
	mod = number % 10000;
	temp = mod / 1000;
	symbolCode = ALPH[temp];
	symbolIndication(symbolCode, symbolCounter);
}

void tenThousandInt(uint16_t number, uint8_t symbolCounter)
{
uint32_t temp = 0;
uint8_t symbolCode = 0;
	
	//mod = number % 100000;
	//temp = mod / 10000;
	temp = number / 10000;
	symbolCode = ALPH[temp];
	symbolIndication(symbolCode, symbolCounter);
}

/*void oneFloat(uint16_t number, uint8_t parameterNumber, uint8_t symbolCounter, union numberAttributesTypeDef tempPar)
{
int16_t mod = 0, temp = 0;
uint8_t symbolCode = 0;

	switch(tempPar.bit.fraction)
	{
		case 1:
			mod = number % 100;
			temp = mod / 10;
			symbolCode = ALPH[temp];
			symbolIndication(symbolCode, symbolCounter);
		break;
		case 2:
			mod = number % 1000;
			temp = mod / 100;
			symbolCode = ALPH[temp];
			symbolIndication(symbolCode, symbolCounter);
		break;
		case 3:
			mod = number % 10000;
			temp = mod / 1000;
			symbolCode = ALPH[temp];
			symbolIndication(symbolCode, symbolCounter);
		break;
	}
}

void tenFloat(uint16_t number, uint8_t parameterNumber, uint8_t symbolCounter, union numberAttributesTypeDef tempPar)
{
int16_t mod = 0, temp = 0;
uint8_t symbolCode = 0;
	
	switch(tempPar.bit.fraction)
	{
		case 1:
			mod = number % 1000;
			temp = mod / 100;
			symbolCode = ALPH[temp];
			symbolIndication(symbolCode, symbolCounter);
		break;
		case 2:
			mod = number % 10000;
			temp = mod / 1000;
			symbolCode = ALPH[temp];
			symbolIndication(symbolCode, symbolCounter);
		break;
		case 3:
			//mod = number % 100000;
			//temp = mod / 10000;
			temp = number / 10000;
			symbolCode = ALPH[temp];
			symbolIndication(symbolCode, symbolCounter);
		break;
	}
}

void hundredFloat(uint16_t number, uint8_t parameterNumber, uint8_t symbolCounter, union numberAttributesTypeDef tempPar)
{
int16_t mod = 0, temp = 0;
uint8_t symbolCode = 0;
	
	switch(tempPar.bit.fraction)
	{
		case 1:
			mod = number % 10000;
			temp = mod / 1000;
			symbolCode = ALPH[temp];
			symbolIndication(symbolCode, symbolCounter);
		break;
		case 2:
			//mod = number % 100000;
			//temp = mod / 10000;
			//temp = (mod >> 8) / 39;
			temp = number / 10000;
			symbolCode = ALPH[temp];
			symbolIndication(symbolCode, symbolCounter);
		break;

	}
}

void thousandFloat(uint16_t number, uint8_t parameterNumber, uint8_t symbolCounter, union numberAttributesTypeDef tempPar)
{
int32_t mod = 0, temp = 0;
uint8_t symbolCode = 0;
	

	mod = number % 100000;
	temp = mod / 10000;
	symbolCode = ALPH[temp];
	symbolIndication(symbolCode, symbolCounter);

}

uint8_t floatIndication(uint16_t number, uint8_t symbolCounter, uint8_t parameterNumber, union numberAttributesTypeDef tempPar)
{
int16_t mod = 0, temp = 0;
char symbol  = 0; 
uint8_t symbolCode = 0;
	
	if (number > 9999)  
	{
		switch(tempPar.bit.fraction)
		{
			case 1:
				thousandFloat(number, parameterNumber, symbolCounter, tempPar);
				hundredFloat(number, parameterNumber, symbolCounter, tempPar);
				tenFloat(number, parameterNumber, symbolCounter, tempPar);
				oneFloat(number, parameterNumber, symbolCounter, tempPar);
				symbolCounter += 4;
			break;
			case 2:
				hundredFloat(number, parameterNumber, symbolCounter, tempPar);
				tenFloat(number, parameterNumber, symbolCounter, tempPar);
				oneFloat(number, parameterNumber, symbolCounter, tempPar);
				symbolCounter += 3;
			break;
			case 3:
				tenFloat(number, parameterNumber, symbolCounter, tempPar);
				oneFloat(number, parameterNumber, symbolCounter, tempPar);
				symbolCounter += 2;
			break;
		}

	}
	else if (number > 999)
	{
		switch(tempPar.bit.fraction)
		{
			case 1:
				hundredFloat(number, parameterNumber, symbolCounter, tempPar);
				tenFloat(number, parameterNumber, symbolCounter, tempPar);
				oneFloat(number, parameterNumber, symbolCounter, tempPar);
				symbolCounter += 3;
			break;
			case 2:
				tenFloat(number, parameterNumber, symbolCounter, tempPar);
				oneFloat(number, parameterNumber, symbolCounter, tempPar);
				symbolCounter += 2;
			break;
			case 3:
				oneFloat(number, parameterNumber, symbolCounter, tempPar);
				symbolCounter += 1;
			break;
		}

	}
	else if (number > 99) 
	{
		switch(tempPar.bit.fraction)
		{
			case 1:
				tenFloat(number, parameterNumber, symbolCounter, tempPar);
				oneFloat(number, parameterNumber, symbolCounter, tempPar);
				symbolCounter += 2;
			break;
			case 2:
				oneFloat(number, parameterNumber, symbolCounter, tempPar);
				symbolCounter += 1;
			break;
			case 3:
				symbol = '0';
				symbolCode = ALPH[symbol];
				symbolIndication(symbolCode, symbolCounter);
				symbolCounter += 1;
			break;
		}
	}
	else if (number > 9) 
	{
		switch(tempPar.bit.fraction)
		{
			case 1:
				oneFloat(number, parameterNumber, symbolCounter, tempPar);
				symbolCounter += 1;
			break;
			case 2:
				symbol = '0';
				symbolCode = ALPH[symbol];
				symbolIndication(symbolCode, symbolCounter);
				symbolCounter += 1;
			break;
			case 3:
				symbol = '0';
				symbolCode = ALPH[symbol];
				symbolIndication(symbolCode, symbolCounter);
				symbolCounter += 1;
			break;
		}
	}
	else
	{
		symbol = '0';
		symbolCode = ALPH[symbol];
		symbolIndication(symbolCode, symbolCounter);
		symbolCounter += 1;
	}
	
	symbol = '.';
	symbolCode = ALPH[symbol];
	symbolIndication(symbolCode, symbolCounter);
	symbolCounter++;
	
	switch(tempPar.bit.fraction)
	{
		case 1:
			mod = number % 10;
			symbolCode = ALPH[mod];
			symbolIndication(symbolCode, symbolCounter);
			symbolCounter++;
		break;
		case 2:
			if (number < 10)
			{
				symbol = '0';
				symbolCode = ALPH[symbol];
				symbolIndication(symbolCode, symbolCounter);
				symbolCounter++;
				
				mod = number % 10;
				symbolCode = ALPH[mod];
				symbolIndication(symbolCode, symbolCounter);
				symbolCounter++;
			}
			else
			{
				mod = number % 100;
				temp = mod / 10;
				symbolCode = ALPH[temp];
				symbolIndication(symbolCode, symbolCounter);
				symbolCounter++;
				
				temp = mod % 10;
				symbolCode = ALPH[temp];
				symbolIndication(symbolCode, symbolCounter);
				symbolCounter++;
			}
		break;
		case 3:
			if (number < 100)
			{
				if (number < 10)
				{
					symbol = '0';
					symbolCode = ALPH[symbol];
					symbolIndication(symbolCode, symbolCounter);
					symbolCounter++;
					
					symbol = '0';
					symbolCode = ALPH[symbol];
					symbolIndication(symbolCode, symbolCounter);
					symbolCounter++;
					
					mod = number % 10;
					symbolCode = ALPH[mod];
					symbolIndication(symbolCode, symbolCounter);
					symbolCounter++;
				}
				else
				{
					symbol = '0';
					symbolCode = ALPH[symbol];
					symbolIndication(symbolCode, symbolCounter);
					symbolCounter++;
					
					mod = number % 100;
					temp = mod / 10;
					symbolCode = ALPH[temp];
					symbolIndication(symbolCode, symbolCounter);
					symbolCounter++;
					
					temp = mod % 10;
					symbolCode = ALPH[temp];
					symbolIndication(symbolCode, symbolCounter);
					symbolCounter++;
				}
			}
			else
			{
				mod = number % 1000;
				temp = mod / 100;
				symbolCode = ALPH[temp];
				symbolIndication(symbolCode, symbolCounter);
				symbolCounter++;
				
				temp = mod % 100;
				temp = temp / 10;
				symbolCode = ALPH[temp];
				symbolIndication(symbolCode, symbolCounter);
				symbolCounter++;
				
				temp = mod % 10;
				symbolCode = ALPH[temp];
				symbolIndication(symbolCode, symbolCounter);
				symbolCounter++;
			}
		break;
	}	
	return(symbolCounter);
}*/

uint8_t integerIndication(uint16_t number, uint8_t symbolCounter)
{

	if (number > 9999) 
	{
		tenThousandInt(number, symbolCounter);
		thousandInt(number, symbolCounter);
		hundredInt(number, symbolCounter);
		tenInt(number, symbolCounter);
		oneInt(number, symbolCounter);
		symbolCounter += 5;
	}
	else if (number > 999) 
	{
		thousandInt(number, symbolCounter);
		hundredInt(number, symbolCounter);
		tenInt(number, symbolCounter);
		oneInt(number, symbolCounter);
		symbolCounter += 4;
	}
	else if (number > 99)
	{
		hundredInt(number, symbolCounter);
		tenInt(number, symbolCounter);
		oneInt(number, symbolCounter);
		symbolCounter += 3;
	}
	else if (number > 9)
	{
		tenInt(number, symbolCounter);
		oneInt(number, symbolCounter);
		symbolCounter += 2;
	}
	else
	{
		oneInt(number, symbolCounter);
		symbolCounter += 1;
	}

	return symbolCounter;
}

/*uint8_t numIndication(uint16_t number, uint8_t symbolCounter, uint8_t parameterNumber, uint8_t imitationFlag)
{
char symbol;
uint16_t symbolCode;
union numberAttributesTypeDef tempPar;

	if(imitationFlag)
	{
		tempPar.all = imitationParameters[parameterNumber].all;
	}
	else
	{
		tempPar.all = parameters[parameterNumber].all;
	}*/
/*	symbol = ' ';
	symbolCode = ALPH[symbol];
	symbolIndication(symbolCode);
	symbolCounter++;*/
/*	if (!tempPar.bit.fraction)
	{
		if(tempPar.bit.sign)
		{
			if (number & 0x8000)
//			if (number < 0)
			{
				symbol = '-';
				symbolCode = ALPH[symbol];
				symbolIndication(symbolCode, symbolCounter);
				symbolCounter++;
				if (!(number == 0x8000))
				{
					number ^= 0xFFFF;
					number ++;
					number &= 0x7FFF;
				}
				symbolCounter = integerIndication(number, symbolCounter);
			}
			else
			{
				symbolCounter = integerIndication(number, symbolCounter);
			}
		}
		else
		{
			symbolCounter = integerIndication(number, symbolCounter);
		}

	}
	else
	{
		if(tempPar.bit.sign)
		{
			if (number & 0x8000)
//			if (number < 0)
			{
				symbol = '-';
				symbolCode = ALPH[symbol];
				symbolIndication(symbolCode, symbolCounter);
				symbolCounter++;
				if (!(number == 0x8000))
				{
					number ^= 0xFFFF;
					number ++;
					number &= 0x7FFF;
				}
					symbolCounter = floatIndication(number, symbolCounter, parameterNumber, tempPar);
				
			}
			else
			{
				symbolCounter = floatIndication(number, symbolCounter, parameterNumber, tempPar);
			}
		}
		else
		{
			symbolCounter = floatIndication(number, symbolCounter, parameterNumber, tempPar);
		}
	}
	return (symbolCounter);
}

void paramValueIndication(uint8_t lineNumber, char *tablesLine, uint8_t parameterNumber, int16_t value, uint8_t imitationFlag)
{
uint8_t command = 0, symbolCounter = 0;
//char *string = tableName[tablesLine];
	
	command = lineSelection(lineNumber);
	
	commandToDisplay(command);
	
	symbolCounter = lineIndication(lineNumber, tablesLine, 0);
	if(imitationFlag)
	{
		if (imitationParameters[parameterNumber].bit.numOutput) 
		{
			symbolCounter = numIndication(value, symbolCounter, parameterNumber, 1);
			symbolCounter = writeUnitOfMeasure(parameterNumber, symbolCounter, 1);
		}
	}
	else
	{
		if (parameters[parameterNumber].bit.numOutput) 
		{
			symbolCounter = numIndication(value, symbolCounter, parameterNumber, 0);
			symbolCounter = writeUnitOfMeasure(parameterNumber, symbolCounter, 0);
		}
	}
	
	lineFilling(symbolCounter);		//!!!
		
}*/

void emptyLine(uint8_t lineNumber)
{
uint8_t command;

	command = lineSelection(lineNumber);
	
	commandToDisplay(command);
	
	lineFilling(0);
}
