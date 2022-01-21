#include<iostream>
#include<cmath>

// given position profile

using namespace std;
// define parameters
double period;
double A; //define max displacement
double num; //number of points

// position & velocity profile function clarification
double positionprofile(double t); // ***merely adjust this function to satisfy different signal specification***
double velocityprofile(double t);

int main()
{
// begin calculation
double time[num];

for (int n=0; n<num; n++)
{
	time[n]= period/ (num-1) *n;
}

double velocity[num];

for (int n=0; n<num; n++)
{
	velocity[n]=velocityprofile(time[n]); // the right-hand side function can be replaced by any user-defined function
}

return 0;
}

double positionprofile(double t)
{
	double pos;
	pos=A*sin(2*pi/period*t);
	return pos; 
}


double velocityprofile(double t)
{
	double del_t=0.001;
	double vel;
	vel=(-positionprofile(t+2*del_t)+8*positionprofile(t+del_t)-8*positionprofile(t-del_t)+positionprofile(t-2*del_t))/(12*del_t);
	return vel; 
}
