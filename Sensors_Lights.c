// Sensors_Lights.c
// Starter file for CECS346 Project 2
// By Dr. Min He
#include "tm4c123gh6pm.h"
#include <stdint.h> // C99 data types
#include "Sensors_Lights.h"

// Initialize the two sensors, enable both edge edge-triggered interrupt for both sensors
void Sensors_Init(void){
}

// Initialize the reset button: use level triggered interrupt
void Reset_Init(void){
}

// Initialize 8 LEDs
void Lights_Init(void){
	//board LEDS PA0-7
  SYSCTL_RCGC2_R      |= 0x00000001;    //activates port A clock
  while ((SYSCTL_RCGCGPIO_R & 0x00000001) != 0x00000001) {} //wait for clock to be ready
  GPIO_PORTF_DIR_R    &=  ~0xFF;                                         //make PA0-7 in (Lights)
  GPIO_PORTF_AFSEL_R  &= ~0xFF;                                         //disable alt funct on PA0-7
  GPIO_PORTF_DEN_R    |=  0xFF;                                         //enable digital I/O on PF0-7
}
