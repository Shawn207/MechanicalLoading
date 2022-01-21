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
int velocity;
int amplitude;
int x;

// velocity profile function clarification
int velocityprofile(int t); // ***merely adjust this function to satisfy different signal specification***

void user()
{	
	
 	period=1000;
 	amplitude=400;
	Vm=2*3.14159*amplitude/period*60;
	
	//od_write(0x60A8, 0x00, 0xFA010000);
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

	sleep(1);
	
	//sign=1;
	/* do //main loop
	{
		x1=od_read(0x3220, 0x02);
		sleep(1);
		x2=od_read(0x3220, 0x02);
		sleep(1);
		x3=od_read(0x3220, 0x02);
		velocity=x2*Vm/500;
		if ((x1>x2) & (x3>x2))
		{
			sign=-sign;
		}
		velocity=sign*velocity;
		// Set the target velocity
		od_write(0x60FF, 0x00, velocity);
		// Let the motor run for a while
	}
	while (1); */
	
	while(1)
	{
		
		x=od_read(0x3220, 0x02);
		velocity=(x-480)*Vm/480;
		od_write(0x60FF, 0x00, velocity);
		sleep(1);
	}
 
}


