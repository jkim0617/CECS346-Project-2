// PLL.c
// Runs on TM4C123
// Starter file for CECS346 Project 2
// By Dr. Min He
 
#include "PLL.h"
#include "tm4c123gh6pm.h"

// The #define statement SYSDIV2 in PLL.h
// initializes the PLL to the desired frequency.

// bus frequency is 400MHz/(SYSDIV2+1) = 400MHz/(7+1) = 50 MHz
// see the table at the end of this file

// configure the system to get its clock from the PLL
void PLL_Init(void){
}
