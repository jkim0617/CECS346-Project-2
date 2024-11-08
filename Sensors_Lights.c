// Sensors_Lights.c
// CECS 346 Project 2
// By: Jose Ambriz, Bronson Garel, Jonathan Kim, Dylan Ong, Kyle Wyckoff

#include "tm4c123gh6pm.h"
#include <stdint.h> // C99 data types

#define SENSORS_MASK 	0x03
#define LIGHTS_MASK 	0xFF
#define RESET_MASK 		0x04


// Initialize the two sensors, enable both edge edge-triggered interrupt for both sensors
void Sensors_Init(void){																							// PE0-PE1
	SYSCTL_RCGC2_R      	|= 			SYSCTL_RCGC2_GPIOE;    								// activates port E clock
  while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOE)!= SYSCTL_RCGC2_GPIOE){} 	// wait for clock to be ready
		
  GPIO_PORTE_AMSEL_R 		&=			~SENSORS_MASK;           							// Disable analog function on PE0-PE1
  GPIO_PORTE_PCTL_R  		&=			~0x000000FF;             							// Enable regular GPIO
  GPIO_PORTE_DIR_R   		&=			~SENSORS_MASK;           							// Inputs on PE0-PE1
  GPIO_PORTE_AFSEL_R 		&=			~SENSORS_MASK;           							// Regular function on PE0-PE1
	GPIO_PORTE_DEN_R 			|= 			SENSORS_MASK;													// Enable digital function on PE0-PE1
		
	GPIO_PORTE_IS_R     	&= 			~SENSORS_MASK;                      	// PE0-PE1 is edge sensitive
	GPIO_PORTE_IBE_R    	|= 			SENSORS_MASK;                       	// PE0-PE1 is both edges
	GPIO_PORTE_ICR_R    	|=  		SENSORS_MASK;                       	// clear flag on PE0-PE1
	GPIO_PORTE_IM_R     	|=  		SENSORS_MASK;                       	// arm interrupt on PE0-PE1
	NVIC_PRI1_R 					 = 			(NVIC_PRI1_R&0xFFFFFF1F)|0x00000040;	// PORTE Interrupt priority bits: 7-5, priority set to 2 
	NVIC_EN0_R 						|= 			0x00000010;                         	// enable interrupt 4 in NVIC
}

// Initialize the reset button: use level triggered interrupt   
void Reset_Init(void){																								// PA2
	SYSCTL_RCGC2_R      	|= 			SYSCTL_RCGC2_GPIOA;    								// activates port A clock
  while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOA)!= SYSCTL_RCGC2_GPIOA){} 	// wait for clock to be ready
		
	GPIO_PORTA_AMSEL_R 		&=			~RESET_MASK;           								// Disable analog function on PA2
	GPIO_PORTA_PCTL_R  		&=			~0x00000F00;             							// Enable regular GPIO
  GPIO_PORTA_DIR_R    	&=  		~RESET_MASK;                        	// make PA2 in (Lights)
  GPIO_PORTA_AFSEL_R  	&=  		~RESET_MASK;                        	// disable alt funct on PA2
  GPIO_PORTA_DEN_R    	|=   		RESET_MASK;                         	// enable digital I/O on PA2
		
	GPIO_PORTA_IS_R     	|= 			RESET_MASK;                      			// PA2 is level sensitive
	GPIO_PORTE_IBE_R    	&= 			~RESET_MASK;                       		// PA2 is not on both edges
	GPIO_PORTA_ICR_R    	|=  		RESET_MASK;                       		// clear flag on PA2
	GPIO_PORTA_IEV_R			|=			RESET_MASK;   												// High Level
	GPIO_PORTA_IM_R     	|=  		RESET_MASK;                       		// arm interrupt on PA2
	NVIC_PRI0_R 					 = 			(NVIC_PRI0_R&0xFFFFFF1F)|0x00000020;	// PORTA Interrupt priority bits: 23-21, priority set to 1
	NVIC_EN0_R 						|= 			0x00000001;                         	// enable interrupt 0 in NVIC
	
}

// Initialize 8 LEDs
void Lights_Init(void){
	//board LEDS PB0-7
  SYSCTL_RCGC2_R      	|= 			SYSCTL_RCGC2_GPIOB;    								// activates port B clock
  while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)!= SYSCTL_RCGC2_GPIOB){}	// wait for clock to be ready
		
	GPIO_PORTB_AMSEL_R 		&=			~LIGHTS_MASK;           							// Disable analog function on PB0-7
	GPIO_PORTB_PCTL_R  		&=			~0xFFFFFFFF;             							// Enable regular GPIO
  GPIO_PORTB_DIR_R    	|=  		LIGHTS_MASK;                          // make PB0-7 in (Lights)
  GPIO_PORTB_AFSEL_R  	&=  		~LIGHTS_MASK;                         // disable alt funct on PB0-7
  GPIO_PORTB_DEN_R    	|=   		LIGHTS_MASK;                          // enable digital I/O on PB0-7
}
