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

// The user() function is the entry point of the NanoJ program. It is called
// by the firmware of the controller when the NanoJ program is started.
void user()
{
	// Set profile velocity in position mode to 1000
	od_write(0x6081, 0x00, 1000);
	// Set profile acceleration in position mode to 3000
	od_write(0x6083, 0x00, 3000);
	// Set profile deceleration in position mode to 3000
	od_write(0x6084, 0x00, 3000);
	
	// Set mode "Profile velocity"
	od_write(0x6060, 0x00, 3);

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

while(1)
	{
	
	// Set mode "Profile velocity"
	od_write(0x6060, 0x00, 3);
	// Request the state "Operation enabled"
	od_write(0x6040, 0x00, 0xF);

	// Wait until the requested state is reached
	while ( (In.StatusWord & 0xEF) != 0x27) {
		yield();
	}
	od_write(0x60FF, 0x00, 100);
	// Let the motor run for a while
	sleep(5000);

	// Set mode "Profile Position"
	od_write(0x6060, 0x00, 1);
	
	od_write(0x607A, 0x00, 0);
	
	od_write(0x6040, 0x00, 0xF);
	while ( (In.StatusWord & 0xEF) != 0x27) {
		yield();
	}
	
	od_write(0x6040, 0x00, 0x3F);	
	sleep(1000);

	}
	
}
