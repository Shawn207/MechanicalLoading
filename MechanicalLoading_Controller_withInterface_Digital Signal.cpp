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

int Vm; //define max velocity
int velocity;
int amplitude;
U32 offset;
volatile U32 upperbytePeriod;
volatile U32 lowerbytePeriod;
volatile U32 period;

void preset();

void user()
{

	preset();//// Homing to the origin (displacement = 0)

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
	while ((In.StatusWord & 0xEF) != 0x27) {
		yield();
	}

	od_write(0x6040, 0x00, 0x3F); // Run the motor 
	sleep(5000); // sleep for sufficiently long time to make sure homing finishing

	// Go back to profile velocity mode
	od_write(0x6060, 0x00, 3);
	// Request the state "Operation enabled"
	od_write(0x6040, 0x00, 0xF);

	// Wait until the requested state is reached
	while ((In.StatusWord & 0xEF) != 0x27) {
		yield();
	}

	while (1)
	{
		od_write(0x60FF, 0x00, lowerbytePeriod);
		sleep(1);
	}

}

void preset()
{
	// clear DOs
	od_write(0x60FE, 0x01, 0x00);
	od_write(0x60FE, 0x01, 0x40000);// set 1 to DO3, telling Arduino that controller has been in preset(), please go on. 

	while ((od_read(0x60FD, 0x00) & 0x10000) != 0x10000)//read DI 1, until it gets high, knowing Arduino has sent lowerbyte of period
	{
		yield();// the program cannot wait for a long time without yiled() or delay()
	}
	lowerbytePeriod = od_read(0x60FD, 0x00);// read the DI 2-5 as upperbyte of period
	lowerbytePeriod = lowerbytePeriod >> 17;
	od_write(0x60FE, 0x01, 0x10000);// write 1 through DO 1, telling Arduino that I have received lowerbyte of period
		//debug

	/*while((od_read(0x60FD,0x00) & 0x10000) != 0x00)//read DI 1, until it gets low, knowing Arduino has sent upperbyte of period
	{
		yield();// the program cannot wait for a long time without yiled() or delay()
	}
	od_write(0x60FE,0x01,0x00);
	upperbytePeriod = od_read(0x60FD,0x00);// read the DI 2-5 as lowerbyte of period
	upperbytePeriod = upperbytePeriod >> 17;
	od_write(0x3321,0x02,upperbytePeriod);
	od_write(0x60FE,0x01,0x20000);// write 1 through DO 2, telling Arduino that I have received upperbyte of period
	period = (upperbytePeriod<<4);
	period = lowerbytePeriod + period;*/



	//od_write(0x6071,0x00,period);
	//test

	// now, Arduino will clear all DI of controller, so we can process on amplitude.

	//prepair for period
	/*
	offset=od_read(0x3220, 0x02);
	//od_write(0x3321, 0x02, -2);
	od_write(0x3322, 0x02, 2550);
	od_write(0x3323, 0x02, 1023);
	while(od_read(0x3320, 0x02) == 0)
	{
		yield();
	}// wait for period
	period = od_read(0x3320, 0x02);
	od_write(0x60FE, 0x01, 0x10000);
	*/



	/*while((od_read(0x60FD,0x00) & 0x20000) != 0x20000);
	od_write(0x60FE, 0x01, 0x10000);
	offset=od_read(0x3220, 0x02);
	od_write(0x3321, 0x02, -1*offset);
	od_write(0x3322, 0x02, 2550);
	od_write(0x3323, 0x02, 1023);
	while(od_read(3320, 0x02) == 0);
	period = od_read(3320, 0x02);
	od_write(0x60FE, 0x01, 0x20000); */

}
