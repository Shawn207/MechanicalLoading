// In this NanoJ program, we will make the motor turn shortly back and forth.
// The language used for NanoJ programs is C, with a few specific extensions,
// like the mappings (see below).
// Please refer to the product manual for more details about NanoJ and the
// object dictionary.

// You can map frequently used objects to be able to read or write them
// using In.* and Out.*. Here we map the object 6041:00 as "In.StatusWord".
map U16 StatusWord as input 0x6041:00

// Include the definition of NanoJ functions and symbols
#include "wrapper.h"

int period;
int Vm; //define max velocity
int num; //number of points
int sampling_period;

// velocity profile function clarification
int velocityprofile(int t); // ***merely adjust this function to satisfy different signal specification***

void user()
{
 	period=2500;
	Vm=200; //define max velocity
	num=201; //number of points
	sampling_period = period / (num - 1);
	
	/* // return to 0 displacement

	// Set mode "Profile position"
	od_write(0x6060, 0x00, 1);

	// Set the target position
	od_write(0x607A, 0x00, 0);

 	// Request state "Ready to switch on"
	od_write(0x6040, 0x00, 0x6);

	// Wait until the requested state is reached
	while ( (In.StatusWord & 0xEF) != 0x21) {
		yield(); // Wait for the next cycle (1ms)
	}

	// Request the state "Switched on"
	od_write(0x6040, 0x00, 0x7);

	// Wait until the requested state is reached
	while ( (In.StatusWord & 0xEF) != 0x23) {
		yield();
	}

	// Request the state "Operation enabled"
	od_write(0x6040, 0x00, 0xF);

	// Wait until the requested state is reached
	while ( (In.StatusWord & 0xEF) != 0x27) {
		yield();
	}

	// Let the motor run for a while
	sleep(5000);

	// Stop the motor
	od_write(0x6040, 0x00, 0x0);

	*/
	
	// velocity profile

	// signal generation
	int time[num];

	for (int n = 0; n < num; n++)
	{
		time[n] = sampling_period * n;
		yield();
	}

	int velocity[num];

	for (int n = 0; n < num; n++)
	{
		velocity[n] = velocityprofile(time[n]); // the right-hand side function can be replaced by any user-defined function
		yield();
	}

	// Set mode "Profile velocity"
	od_write(0x6060, 0x00, 3);

	// Remember target velocity before overwriting, so we can reset it later.
	// U32 targetVelocity = od_read(0x60FF, 0x00);

	// Set the target velocity
	od_write(0x60FF, 0x00, 0);

	// Request state "Ready to switch on"
	od_write(0x6040, 0x00, 0x6);

	// Wait until the requested state is reached
	while ((In.StatusWord & 0xEF) != 0x21) {
		yield(); // Wait for the next cycle (1ms)
	}

	// Request the state "Switched on"
	od_write(0x6040, 0x00, 0x7);

	// Wait until the requested state is reached
	while ((In.StatusWord & 0xEF) != 0x23) {
		yield();
	}

	// Request the state "Operation enabled"
	od_write(0x6040, 0x00, 0xF);

	// Wait until the requested state is reached
	while ((In.StatusWord & 0xEF) != 0x27) {
		yield();
	}

	sleep(100);

	while (1) //main loop
	{
		for (int i = 0; i < num; i++)
		{
			// Set the target velocity
			od_write(0x60FF, 0x00, velocity[i]);

			// Let the motor run for a while
			sleep(sampling_period);
		}
	} 
}

int velocityprofile(int t)
{
	int vel;
	Vm = 200;
	vel = Vm*cos(2 * 3.14159 / period * t);
	return vel;
}

