/*
 * Made By:	Patrick Sanford
 * GitHub:	https://github.com/PSanf2/BBB2
 */
 
//Pull in C libraries
#include <iostream>		// pulls in cin and hex
#include <cstdio>		// pulls in printf()
#include <cstdlib>		// lets me use system()
#include <ctime>		// lets me use clock_t and clock()

// Pull in my object headers
#include "Servo.h"
#include "IR_LED.h"
#include "IR_Sensor.h"

// Define where my pins are

#define SERVO_PIN_1			"P9_22"
#define SERVO_PIN_2			"P9_14" // don't put the second servo on the same PWM channel.
//#define IR_LED_PIN_1		"P8_19"
#define IR_LED_PIN		"P8_19"
//#define IR_SENSOR_PIN_1		"P8_12"
#define IR_SENSOR_PIN_1		"P8_07" // Put the IR sensor inputs on pins w/ internal pull-ups
//#define IR_LED_PIN_2		"P9_14" // don't put the second IR LED on the same PWM channel.
#define IR_SENSOR_PIN_2		"P8_10"

// 'cause I got to
using namespace std;

// Declare and create my objects
PatricksDrivers::Servo servo_1(SERVO_PIN_1); // right, less = forward
PatricksDrivers::Servo servo_2(SERVO_PIN_2); // left, more = forward
PatricksDrivers::IR_LED ir_led(IR_LED_PIN);
PatricksDrivers::IR_Sensor ir_sensor_1(IR_SENSOR_PIN_1);
PatricksDrivers::IR_Sensor ir_sensor_2(IR_SENSOR_PIN_2);

// Functions to put things on the screen.
void printMenu() {
	printf("\n\t\t-----MAIN MENU-----");
	printf("\n\t 1) Print Menu");
	printf("\n\t 2) Forward");
	printf("\n\t 3) Backward");
	printf("\n\t 4) Left");
	printf("\n\t 5) Right");
	printf("\n\t 6) Stop");
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

void forward() {
	servo_1.duty(1450000);
	servo_2.duty(1550000);
}

void backward() {
	servo_1.duty(1550000);
	servo_2.duty(1450000);
}

void left() {
	servo_1.duty(1450000);
	servo_2.duty(1500000);
}

void right() {
	servo_1.duty(1500000);
	servo_2.duty(1550000);
}

void stop() {
	servo_1.duty(1500000);
	servo_2.duty(1500000);
}

// IR sensor callbacks
const int debounceDelay = 5000; // why not just use #define?
clock_t ir_1_debounce;
int ir_sensor_1_callback(int var) {
	if ((clock() - ir_1_debounce) > debounceDelay) {
		printf("\nPing from IR Sensor 1");
	}
	ir_1_debounce = clock();
}

clock_t ir_2_debounce;
int ir_sensor_2_callback(int var) {
	if ((clock() - ir_2_debounce) > debounceDelay) {
		printf("\nPing from IR Sensor 2");
	}
	ir_2_debounce = clock();
}

// Main function
int main(int argc, char* argv[]) {
	
	unsigned int menu_choice;
	unsigned int n;
	
	system("clear");
	
	do {
		
		printMenu();
		
		getDecInput(&menu_choice);
		
		system("clear");
		
		switch (menu_choice) {
			
			case 1:
				; // do nothing. Menu will automatically print on next loop.
			break;
			
			case 2:
				forward();
			break;
			
			case 3:
				backward();
			break;
			
			case 4:
				left();
			break;
			
			case 5:
				right();
			break;
			
			case 6:
				stop();
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
