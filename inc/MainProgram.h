#ifndef __MainProgram_H
#define __MainProgram_H
#include <stdint.h>

void MainProgram(void);
void usDelay(uint16_t delayCounter);
void msDelay(uint16_t delayCounter);

typedef struct {
	union {
		uint16_t all;
		struct {
			uint16_t render		: 1;
		} bit;
	} FLAGS;
} mainStruct_TypeDef;
#endif
