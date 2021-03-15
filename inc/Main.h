#ifndef __Main_H
#define __Main_H
#include <stdint.h>

union Struct_PORTA { // Порт A
	uint32_t all;
	struct {
		uint32_t DB0				: 1;
		uint32_t DB1				: 1;
		uint32_t DB2				: 1;
		uint32_t DB3				: 1;
		uint32_t DB4				: 1;
		uint32_t DB5				: 1;
		uint32_t DB6				: 1;
		uint32_t DB7				: 1;
		uint32_t reserved		: 24;
	} bit;
};

union Struct_PORTB { // Порт A
	uint32_t all;
	struct {
		uint32_t reserved0	: 12;
		uint32_t AO					: 1;
		uint32_t RW					: 1;
		uint32_t E					: 1;
		uint32_t reserved1	: 12;
	} bit;
};

union Struct_PORTD { // Порт D
	uint32_t all;
	struct {
	uint32_t DB2		 	: 1;	
	uint32_t DB3		 	: 1;	
	uint32_t E			 	: 1;	
	uint32_t noDef1	 		: 2;	
	uint32_t R_W			: 1;
	uint32_t noDef2			: 5;
	uint32_t AO				: 1;
	uint32_t noDef3			: 2;
	uint32_t DB0			: 1;
	uint32_t DB1			: 1;
	uint32_t noDef4			: 16;
	} bit;
};

union Struct_PORTE { // Порт E
	uint32_t all;
	struct {
		uint32_t BT_G2		: 1;
		uint32_t BT_G1		: 1;
		uint32_t noDef1		: 1;
		uint32_t L_7_DV 	: 1;
		uint32_t L_6_GA		: 1;
		uint32_t L_5_AG		: 1;
		uint32_t L_4_AS		: 1;
		uint32_t DB4			: 1;
		uint32_t DB5			: 1;
		uint32_t DB6			: 1;
		uint32_t DB7			: 1;
		uint32_t noDef2		: 21;
	} bit;
};

#endif
