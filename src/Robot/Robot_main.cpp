/*
 * Made By:	Patrick Sanford
 * Version:	0.0
 * Date:	Sept 15, 2016
 * GitHub:	https://github.com/PSanf2/BBB2
 * Desc:	This is my robot program.
 */
 
 /*
  * Dev Notes
  * I need to set up some kind of "interrupt" for the light sensors.
  * Create two threads, and start running a function in each of them.
  * Have this function check a threadRunning bool, and if the thread
  * is still running then poll the light sensor. If the light sensor
  * goes above a certain value then call another "interrupt" function.
  * Poll the light sensor going high, and going low.
  * I'd like to have a run() function in LightSensor.cpp that accepts
  * a callback parameter pointing to a function that can be used for
  * an interrupt handler. I'll need to do some more work on LightSensor.cpp.
  */

// Pull in C libraries
#include <iostream>		// pulls in cin and hex
#include <cstdio>		// pulls in printf()
#include <cstdlib>		// lets me use system()

// Pull in my object headers
#include "Button.h"
#include "LightSensor.h"
#include "Servo.h"
#include "IR_LED_Sensor.h"

// Define where my pins are
#define BUTTON_PIN		"P8_14"

#define LIGHT_SENSOR_PIN_1	"P9_38"
#define LIGHT_SENSOR_PIN_2	"P9_40"
#define SERVO_PIN_1			"P9_22"
#define SERVO_PIN_2			"P9_42" // don't put the second servo on the same PWM channel.
#define IR_LED_PIN_1		"P8_19"
#define IR_SENSOR_PIN_1		"P8_12"
#define IR_LED_PIN_2		"P9_14" // don't put the second IR LED on the same PWM channel.
#define IR_SENSOR_PIN_2		"P8_10"

// 'cause I got to
using namespace std;

// Declare and create my objects
PatricksDrivers::Button push_btn(BUTTON_PIN);
PatricksDrivers::LightSensor light_sensor_1(LIGHT_SENSOR_PIN_1);
PatricksDrivers::LightSensor light_sensor_2(LIGHT_SENSOR_PIN_2);
PatricksDrivers::Servo servo_1(SERVO_PIN_1);
PatricksDrivers::Servo servo_2(SERVO_PIN_2);
PatricksDrivers::IR_LED_Sensor ir_sensor_1(IR_LED_PIN_1, IR_SENSOR_PIN_1);
PatricksDrivers::IR_LED_Sensor ir_sensor_2(IR_LED_PIN_2, IR_SENSOR_PIN_2);

// Declare variables
unsigned int menu_choice;

// Functions to put things on the screen.
void printMenu() {
	printf("\n\t\t-----MAIN MENU-----");
	printf("\n\t 1) Print Menu");
	
	printf("\n\t 0) Quit");
	printf("\nInput selection ");
}

// Functions to get inputs
void getDecInput(unsigned int *ptr) {
	for (;;) {
		if (cin >> dec >> *ptr) {
			return;
		} else {
			printf("INVALID INPUT.");
			printf("\nInput a dec number: ");
			cin.clear();
			cin.ignore(80, '\n');
		}
	}
}

// Callback functions
int button_callback(int var) {
	printf("\nYou pushed the button!");
}

int ir_sensor_1_callback(int var) {
	
}

int ir_sensor_2_callback(int var) {
	
}

// Main function
int main(int argc, char* argv[]) {
	
	push_btn.wait(&button_callback);
	
	system("clear");
	
	do {
		
		printMenu();
		
		getDecInput(&menu_choice);
		
		system("clear");
		
		switch (menu_choice) {
			
			case 1:
				; // do nothing. Menu will automatically print on next loop.
			break;
			
			case 0:
				; // do nothing
			break;
			
			default:
				printf("\nINVALID SELECTION.");
			
		} // switch (menu_choice)
		
	} while (menu_choice != 0);
	
	printf("Goodbye!\n");
	
	return 0;
}
