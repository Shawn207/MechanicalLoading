// This program moves the motor to the origin (run once), then repeatly drives the motor with
// the velocity function specified by MATLAB

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

void user()
{	
	
 	period=1000;
 	amplitude=400;
	Vm=2*3.14159*amplitude/period*60;
	od_write(0x3321, 0x02, -6-500);
	od_write(0x3322, 0x02, Vm);
	od_write(0x3323, 0x02, 500);
	od_write(0x6083, 0x00, 5000);
	od_write(0x6084, 0x00, 5000);
	// Set profile velocity in position mode to 2000
	od_write(0x6081, 0x00, 2000);
	
	
	//// Homing to the origin (displacement = 0)
	
	
	// Set mode "Profile position"
	od_write(0x6060, 0x00, 1);

	od_write(0x607A, 0x00, 0); // target position = 0
	
	/// Run finite state machine
	
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
	while ( (In.StatusWord & 0xEF) != 0x27) {
		yield();
	}
	
	od_write(0x6040, 0x00, 0x3F); // Run the motor 
	sleep(5000); // sleep for sufficiently long time to make sure homing finishing
		
	// Go back to profile velocity mode
	od_write(0x6060, 0x00, 3);
	// Request the state "Operation enabled"
	od_write(0x6040, 0x00, 0xF);
	
	// Wait until the requested state is reached
	while ( (In.StatusWord & 0xEF) != 0x27) {
		yield();
	}
	
	while(1) // main loop
	{
				
		velocity=od_read(0x3320, 0x02);
		if (velocity<(Vm-5)) // this if/else statement decides whether reaching the max velocity. This is hardcode actually. 5 is a parameter that may be adjusted
		// which corresponding to the displacement origin
		{
			// if not, keep reading and driving
			od_write(0x60FF, 0x00, velocity);
			sleep(1);
		}
		else
		{	// if reaching max velocity, start homing to correct error generated within one period
			/// Homing 
			// Set mode "Profile Position"
			od_write(0x6060, 0x00, 1);
			
			od_write(0x607A, 0x00, 0);
			
			od_write(0x6040, 0x00, 0xF);
			while ( (In.StatusWord & 0xEF) != 0x27) {
				yield();
			}
			
			od_write(0x6040, 0x00, 0x3F);	
			sleep(30); // Sleep for sufficiently long time to make sure homing finishing. the "30" is also a hardcode which could be adjusted
			
			od_write(0x6060, 0x00, 3); // Go back to velocity profile mode
			// Request the state "Operation enabled"
			od_write(0x6040, 0x00, 0xF);
		
			// Wait until the requested state is reached
			while ( (In.StatusWord & 0xEF) != 0x27) {
				yield();
			}
			od_write(0x60FF, 0x00, velocity);
			sleep(1);
			
			for (int i=0; i<(period/8); i++) // After homing once, to prevent homing multiple times 
			// in the following several data points (where velocity>=Vm-2 still holds), read 
			// and drive for the next T/8 without jumping out of this iteration
			{
				velocity=od_read(0x3320, 0x02);
				od_write(0x60FF, 0x00, velocity);
				sleep(1);
			}
		}
	}
 
}


