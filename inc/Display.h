#ifndef __Display_H
#define __Display_H

#include <stdint.h>

// Дисплей
#define CLEAR_DISPLAY 	0x01
#define CURSOR_HOME 		0x02	// команда возвращает дисплей в начальную позицию
#define	DISP_CONTR_D1_C0_B1	0x0F
#define FS_DL0_N1_BR00 	0x28	
#define FS_DL0_N1_BR01 	0x29
#define FS_DL0_N1_BR10 	0x2A
#define FS_DL0_N1_BR11 	0x2B
#define EMS_ID1_S0			0x06	// Entry mode set 
															// I/D = 0 : The AC decrements after each DDRAM or CGRAM access. If S=1, the
															// information on the display shifts to the right by one character position after
															// each DDRAM write.
															// I/D = 1 : The AC increments after each DDRAM or CGRAM access. If S=1, the
															// information on the display shifts to the left by one character position after
															// each DDRAM write.
															// S = 0 : The display shift function is disabled.
															// S = 1 : The display shift function is enabled.*/
#define CGRAM_ADDRESS_0	0x40
#define FIRST_LINE			0x80	// код указателя на первый символ первой строки дисплея
#define SECOND_LINE			0xC0	// код указателя на первый символ второй строки дисплея
#define THIRD_LINE			0x94	// код указателя на первый символ третьей строки дисплея
#define FOURTH_LINE			0xD4	// код указателя на первый символ четвертой строки дисплея
#define CURSOR_OFF			0x0C	// команда отключения курсора
#define CURSOR_ON				0x0D	// команда включения курсора
#define ONE 						0xD2	// позиция курсора при выводе единиц
#define TEN 	 					0xD1	// позиция курсора при выводе десятков
#define HUNDRED  				0xD0	// позиция курсора при выводе сотен
#define THOUSAND 				0x89	// позиция курсора при выводе тысяч
#define PARAM_1ST				0xC0
#define PARAM_2ND				0xC4
#define PARAM_3RD				0xC8
#define PARAM_4TH				0xCC
#define PARAM_5TH				0xD0
#define PARAM_6TH				0x94
#define PARAM_7TH				0x98
#define PARAM_8TH				0x9C
#define PARAM_9TH				0xA0
#define PARAM_10TH			0xA4


void LCDInit(void);
void strobe(void);
void sendHalfByte(uint8_t hByte);
void sendByte(uint8_t byte);
void commandToDisplay(uint8_t command);
void displayConfig(void);
void eDelay(void);
void symbolIndication (uint8_t symbolCode, uint8_t symbolCounter);
uint8_t lineSelection(uint8_t lineNumber);
void lineFilling(uint8_t symbolCounter);
uint8_t lineIndication (uint8_t displayLineNumber, char *tablesLine, uint8_t lineFillingFlag);
void oneInt(uint16_t number, uint8_t symbolCounter);
void tenInt(uint16_t number, uint8_t symbolCounter);
void hundredInt(uint16_t number, uint8_t symbolCounter);
void thousandInt(uint16_t number, uint8_t symbolCounter);
void tenThousandInt(uint16_t number, uint8_t symbolCounter);
uint8_t integerIndication(uint16_t number, uint8_t symbolCounter);
void emptyLine(uint8_t lineNumber);

#endif
