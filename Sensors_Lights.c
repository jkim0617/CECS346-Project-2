// Sensors_Lights.c
// CECS 346 Project 2
// By: Jose Ambriz, Bronson Garel, Jonathan Kim, Dylan Ong, Kyle Wyckoff

#include "tm4c123gh6pm.h"
#include <stdint.h> // C99 data types
#include "Sensors_Lights.h"

#define SENSORS_MASK 	0x03
#define LIGHTS_MASK 	0xFF
#define RESET_MASK 		0x04

// Initialize the two sensors, enable both edge edge-triggered interrupt for both sensors
void Sensors_Init(void){
	SYSCTL_RCGC2_R 				|= 			SYSCTL_RCGC2_GPIOD;      							// Activate Port D clocks
	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOD)!= SYSCTL_RCGC2_GPIOD){} 	// wait for clock to be active
		
  GPIO_PORTD_AMSEL_R 		&=			~SENSORS_MASK;           							// Disable analog function on PD0-PD1
  GPIO_PORTD_PCTL_R  		&=			~0x000000FF;             							// Enable regular GPIO
  GPIO_PORTD_DIR_R   		&=			~SENSORS_MASK;           							// Inputs on PD0-PD1
  GPIO_PORTD_AFSEL_R 		&=			~SENSORS_MASK;           							// Regular function on PD0-PD1
	GPIO_PORTD_DEN_R 			|= 			SENSORS_MASK;													// Enable digital function on PD0-PD1
	GPIO_PORTD_IS_R     	&= 			~SENSORS_MASK;                      	// PD0-PD1 is edge sensitive
	GPIO_PORTD_IBE_R    	|= 			SENSORS_MASK;                       	// PD0-PD1 is both edges
	GPIO_PORTD_ICR_R    	|=  		SENSORS_MASK;                       	// clear flag on PD0-PD1
	GPIO_PORTD_IM_R     	|=  		SENSORS_MASK;                       	// arm interrupt on PD0-PD1
	NVIC_PRI0_R 					 = 			(NVIC_PRI0_R&0x1FFFFFFF)|0x40000000;	// PORTD Interrupt priority bits: 31-29, priority set to 2
	NVIC_EN0_R 						 = 			0x40000000;                         	// enable interrupt 30 in NVIC
}

// Initialize the reset button: use level triggered interrupt   
void Reset_Init(void){
	//PA2
	SYSCTL_RCGC2_R      	|= 			SYSCTL_RCGC2_GPIOA;    								// activates port A clock
  while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOA)!= SYSCTL_RCGC2_GPIOA){} 	// wait for clock to be ready
  GPIO_PORTA_DIR_R    	&=  		~RESET_MASK;                        	// make PA2 in (Lights)
  GPIO_PORTA_AFSEL_R  	&=  		~RESET_MASK;                        	// disable alt funct on PA2
  GPIO_PORTA_DEN_R    	|=   		RESET_MASK;                         	// enable digital I/O on PA2
	GPIO_PORTA_IS_R     	|= 			SENSORS_MASK;                      		// PA2 is edge sensitive
	GPIO_PORTA_ICR_R    	|=  		SENSORS_MASK;                       	// clear flag on PA2
	GPIO_PORTA_IM_R     	|=  		SENSORS_MASK;                       	// arm interrupt on PA2
	NVIC_PRI0_R 					 = 			(NVIC_PRI0_R&0xFFFFFF1F)|0x00000020;	// PORTA Interrupt priority bits: 23-21, priority set to 1
	NVIC_EN0_R 						 = 			0x40000000;                         	// enable interrupt 30 in NVIC
	
}

// Initialize 8 LEDs
void Lights_Init(void){
	//board LEDS PB0-7
  SYSCTL_RCGC2_R      	|= 			SYSCTL_RCGC2_GPIOB;    								// activates port B clock
  while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)!= SYSCTL_RCGC2_GPIOB){}	// wait for clock to be ready
	GPIO_PORTD_AMSEL_R 		&=			~LIGHTS_MASK;           							// Disable analog function on PB0-7
	GPIO_PORTD_PCTL_R  		&=			~0xFFFFFFFF;             							// Enable regular GPIO
  GPIO_PORTB_DIR_R    	|=  		LIGHTS_MASK;                          // make PB0-7 in (Lights)
  GPIO_PORTB_AFSEL_R  	&=  		~LIGHTS_MASK;                         // disable alt funct on PB0-7
  GPIO_PORTB_DEN_R    	|=   		LIGHTS_MASK;                          // enable digital I/O on PB0-7
}
