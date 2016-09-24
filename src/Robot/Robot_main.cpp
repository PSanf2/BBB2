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
  * Screw that. Just spawn a couple of threads off main, poll the light sensors,
  * and then figure out what to do with them if needed.
  * The whole concept of interrupts off the light sensors really goes against
  * what they'll be used for.
  * 
  * IR sensors are so you don't bump into things, so they need to be able to
  * interrupt program execution.
  * The button is a primate interaction device, so that needs to be respond to
  * an input in meat-space time.
  * The light sensors are to be used to sense things.
  * Do something, take a reading, adjust your actions
  * The servos are essentially outputs. They'll do what they're told.
  * 
  * The big thing is really the ability to have interrupts.
  * I need to be able to halt execution of the program in response to an
  * event, and program execution, and possibly resume from where I was
  * interrupted.
  * 
  * Based on how Arduino based obstacle avoiding robots work, the entire concept
  * of an interrupt for this system might be overkill. Most of them are just moving
  * forward, scanning, and then reacting if they sense an object.
  * 
  * I should throw some LEDs on the robot.
  * 
  * I need to see about taking over the HTTP server on the BBB.
  * It looks like Cloud9 has been used for this, and quite a lot of other stuff.
  * It allows for coding in BoneScript, JavaScript, and allows for autostaring
  * stuff when the BBB starts. It doesn't offer a database. I can put up my own
  * web pages by dropping html files into /var/lib/cloud9
  * It may be too much of a pain in the ass to set up a new web server.
  * I'd need to take down the cloud9 stuff (which came w/ the BBB), and replace
  * it w/ XAMPP. Running XAMPP on the BBB won't be an issue, but getting cloud9
  * out of the way may be a pain.
  * The goal of the web server is to allow an easy interface for other systems.
  * Wrong! It shouldn't be hard to disable the cloud9 stuff. It's just a service.
  * The hard part is going to be getting the C++ code to interact with the database.
  * Looks like there's probably a mysql.h library out there. Don't know if g++ has it.
  * 
  * 
  * I NEED TO THROW AT LEAST ONE MORE BUTTON ON TO THE ROBOT!
  * I need to have a power button.
  * 
  * I can power the BBB by providing current to the VDD_5V rail on the P9 header.
  * I know the servos work when I do this if I use the SYS_5V pin to drive the transistors.
  * The power supply says the whole set up (low-voltage cutoff, buck converter, BBB) uses about
  * 1/4 - 1/3 Amps when the BBB is running. The current draw will vary depending on what
  * the BBB is doing. More intensive computations require more current to power the BBB.
  * With the buck converter set at 5V I don't seem to have any issue with the voltage spike
  * when the power is turned on. Giant capacitors for the win.
  * 
  */

// Pull in C libraries
#include <iostream>		// pulls in cin and hex
#include <cstdio>		// pulls in printf()
#include <cstdlib>		// lets me use system()
#include <ctime>		// lets me use clock_t and clock()

// Pull in my object headers
#include "Button.h"
#include "LightSensor.h"
#include "Servo.h"
//#include "IR_LED_Sensor.h"
#include "IR_LED.h"
#include "IR_Sensor.h"

// Define where my pins are
#define BUTTON_PIN		"P8_14"

#define LIGHT_SENSOR_PIN_1	"P9_38"
#define LIGHT_SENSOR_PIN_2	"P9_40"
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
PatricksDrivers::Button push_btn(BUTTON_PIN);
PatricksDrivers::LightSensor light_sensor_1(LIGHT_SENSOR_PIN_1);
PatricksDrivers::LightSensor light_sensor_2(LIGHT_SENSOR_PIN_2);
PatricksDrivers::Servo servo_1(SERVO_PIN_1);
PatricksDrivers::Servo servo_2(SERVO_PIN_2);
//PatricksDrivers::IR_LED_Sensor ir_sensor_1(IR_LED_PIN_1, IR_SENSOR_PIN_1);
//PatricksDrivers::IR_LED_Sensor ir_sensor_2(IR_LED_PIN_2, IR_SENSOR_PIN_2);
PatricksDrivers::IR_LED ir_led(IR_LED_PIN);
PatricksDrivers::IR_Sensor ir_sensor_1(IR_SENSOR_PIN_1);
PatricksDrivers::IR_Sensor ir_sensor_2(IR_SENSOR_PIN_2);

// Functions to put things on the screen.
void printMenu() {
	printf("\n\t\t-----MAIN MENU-----");
	printf("\n\t 1) Print Menu");
	printf("\n\t 2) IR LED Start");
	printf("\n\t 3) Stop IR callbacks");
	printf("\n\t 4) Start servos");
	printf("\n\t 5) Stop servos");
	printf("\n\t 6) Set all servo duty");
	printf("\n\t 7) Set servo 1 duty");
	printf("\n\t 8) Set servo 2 duty");
	printf("\n\t 9) Get analog sensor readings");
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

// button callback
int button_callback(int var) {
	printf("\nYou pushed the button!");
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
				ir_led.run();
				ir_sensor_1.run(&ir_sensor_1_callback);
				ir_sensor_2.run(&ir_sensor_2_callback);
			break;
			
			case 3:
				ir_led.stop();
				ir_sensor_1.stop();
				ir_sensor_2.stop();
			break;
			
			case 4:
				servo_1.start();
				servo_2.start();
			break;
			
			case 5:
				servo_1.stop();
				servo_2.stop();
			break;
			
			case 6:
				printf("\nInput duty: ");
				getDecInput(&n);
				servo_1.duty(n);
				servo_2.duty(n);
			break;
			
			case 7:
				printf("\nInput duty: ");
				getDecInput(&n);
				servo_1.duty(n);
			break;
			
			case 8:
				printf("\nInput duty: ");
				getDecInput(&n);
				servo_2.duty(n);
			break;
			
			case 9:
				printf("\nlight_sensor_1: %i", light_sensor_1.value());
				printf("\nlight_sensor_2: %i", light_sensor_2.value());
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
