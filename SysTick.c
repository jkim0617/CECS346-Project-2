// SysTick.c
// Runs on TM4C123
// Starter file for CECS346 Project 2
// By Dr. Min He


// oscilloscope or LED connected to PF2 for period measurement

#include "tm4c123gh6pm.h"
#include <stdint.h> // C99 data types


#define HALF_S 8000000U

void SysTick_Init(void) {	
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = HALF_S - 1;
	NVIC_ST_CURRENT_R = 0;
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF) | 0x00100000;
	NVIC_ST_CTRL_R = 0x07;
}

void SysTick_Start(uint32_t period) {	
}

void SysTick_Stop(void) {
}