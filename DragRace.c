// DragRace.c
// Starter file for CECS346 Project 2
// By Dr. Min He
// 3/21/2024

#include "tm4c123gh6pm.h"
#include <stdint.h> // C99 data types
#include <stdbool.h> // provides boolean data type
#include "PLL.h"
#include "SysTick.h"
#include "Sensors_Lights.h"

// Function Prototypes
// External functions from startup.s
extern void DisableInterrupts(void); // Disable interrupts
extern void EnableInterrupts(void);  // Enable interrupts
extern void WaitForInterrupt(void);  // Go to low power mode while waiting for the next interrupt

// Functions defined in this file.
void System_Init(void);

// (DONE) TODO: define bit addresses for two sensors, 8 color lights, and one reset button 
#define SENSORS 								(*((volatile unsigned long *)    0x4000700C     )) // bit addresses for 2 sensors,			PD0-PD1
#define LIGHTS              		(*((volatile unsigned long *)    0x400053FC     )) // bit addresses for 8 Race Lights, 	PB0-PB7
#define RESET                   (*((volatile unsigned long *)    0x40006004     )) // bit address for one reset button, PC0

// (DONE) TODO: define number of states for FSM
#define NUM_STATE            (11U)

// (DONE) TODO: FSM definition
struct State { 
	uint8_t Out;
	uint8_t Time;     // multiple of 0.5 second
	uint8_t Next[NUM_STATE];  
};

typedef const struct State STyp;

// (DONE) TODO: define reload value for half second
#define HALF_SEC   (8000000U)

// (DONE) TODO: assign a value to all states in Drag Race FSM
// use all upper case for constants defined here
enum DragRace_states {INIT, WFS, CY1, CY2, GO, FSR, FSB, FSL, WL, WR, WB};

// (DONE) TODO: define Outputs for the FSM 
#define ALL_ON 	    0xFF // Turns on all the LEDs 
#define ALL_OFF 	  0x00 // Turns off all the LEDs 
#define YELLOW1_ON	0x11 // Turns on 1st set of yellow LEDs 
#define YELLOW2_ON  0x22 // Turns on 2nd set of yellow LEDs
#define GREEN_BOTH  0x44 // Turns on the green LEDs 
#define GREEN_LEFT  0x04 // Turns on left green LED
#define GREEN_RIGHT 0x40 // Turns on right green LED
#define RED_BOTH    0x88 // Turns on both red LEDS
#define RED_LEFT    0x08 // Turns on left red LED
#define RED_RIGHT   0x80 // Turns on right red LED 

//(DONE) TODO: Define Drag Race FS
STyp DragRace_FSM[11] = {
{ALL_ON, 			2, {WFS, 	WFS, 	WFS, 	WFS}},
{ALL_OFF, 		1, {CY1, 	WFS, 	WFS, 	WFS}},
{YELLOW1_ON, 	1, {CY2, 	FSR, 	FSL, 	FSB}},
{YELLOW2_ON, 	1, {GO, 	FSR, 	FSL, 	FSB}},
{GREEN_BOTH, 	1, {GO, 	WR, 	WL, 	WB}},
{RED_LEFT, 		2, {WFS, 	WFS, 	WFS, 	WFS}},
{RED_RIGHT, 	2, {WFS, 	WFS, 	WFS, 	WFS}},
{RED_BOTH, 		2, {WFS, 	WFS, 	WFS, 	WFS}},
{GREEN_LEFT, 	2, {WFS, 	WFS, 	WFS, 	WFS}},
{GREEN_RIGHT, 2, {WFS, 	WFS, 	WFS, 	WFS}},
{GREEN_BOTH, 	2, {WFS, 	WFS, 	WFS, 	WFS}}
};
	
// (DONE) TODO: define bit positions for left, right and reset buttons
#define RESET_MASK  			0x01 // bit position for reset button
#define LEFT_SENSOR_MASK  0x04 // bit position for left sensor
#define RIGHT_SENSOR_MASK 0x02 // bit position for left sensor
	
uint8_t Input;
bool timesup;
bool reset;  // flag to reset the system, set by the reset button located at breadboard, not the launchpad reset button.
	
int main(void){
  uint8_t S;  // current state index
	
	System_Init();
		
  while(1){
    // (DONE) TODO: reset FSM to its Initial state, reset globals to default values
    S = INIT;
		reset = false ;
		Input = WFS ;	
		
		while (!reset) {
			// TO Do: take care of FSM outputs and time in state.
			LIGHTS = DragRace_FSM[S].Out;
      SysTick_Start(DragRace_FSM[S].Time*HALF_SEC);
			while((!timesup)&&(!reset)){
			  WaitForInterrupt();
			}
			timesup=false;
			S = DragRace_FSM[S].Next[Input];
		}
  }
}

// Initialize the following hardware modules: PLL, Sensors, Reset button, Lights, SysTick timer
// Initialize the following golbal variables: timesup for FSM state time, reset for reset the whole system  
void System_Init(void) {
	DisableInterrupts();
	PLL_Init();
  Sensors_Init(); 
	Reset_Init(); 
	Lights_Init();
	SysTick_Init(); 
  // (DONE) TODO: reset global variables: timesup, reset, Input 
	reset = false;
	timesup = false;
	Input = WFS;
	EnableInterrupts();
}

void GPIOPortA_Handler(void){
	for (uint32_t i=0;i<160000;i++) {}//debounce
	if(GPIO_PORTA_RIS_R & LEFT_SENSOR_MASK){
		GPIO_PORTA_ICR_R |= LEFT_SENSOR_MASK;		// clear flag
		Input = SENSORS;
	}
	if(GPIO_PORTA_RIS_R & RIGHT_SENSOR_MASK){
		GPIO_PORTA_ICR_R |= RIGHT_SENSOR_MASK; // clear flag
		Input = SENSORS;
	}
	
}
// Interrupt handler for reset button (PF0):  
// update global variable: reset
void GPIOPortF_Handler(void) {
	for (uint32_t i=0;i<160000;i++) {} //debounce
	if (GPIO_PORTF_RIS_R & RESET_MASK){
		GPIO_PORTF_ICR_R |= RESET_MASK;
		reset = true;
	}
}
// Systick interrupt handler:
// Stop systick timer and update global variable: timesup 
void SysTick_Handler(void) {
	timesup = true;	
}

