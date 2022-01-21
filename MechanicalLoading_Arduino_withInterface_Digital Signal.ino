#include <Wire.h>           //wire library
#include <Adafruit_MCP4725.h> // MCP4725 library from adafruit
#define analogVin A0          // Analog voltage input to A0

Adafruit_MCP4725 MCP4725; 

void preset();
void sendtocontroller(unsigned char data);
unsigned int readdata(const int BUFFER_SIZE);

unsigned char period, amplitude;
unsigned char temp;
int num;
int DACvolt;
bool istrue;

void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200);
   MCP4725.begin(0x60); // Default I2C Address of MCP4725 breakout board (sparkfun) 
   pinMode(0, OUTPUT);
   pinMode(1, OUTPUT); // for notifying the controller the period/amplitude is ready to be read 
   // for sending periods/amplitudes
   pinMode(2, OUTPUT); 
   pinMode(3, OUTPUT); 
   pinMode(4, OUTPUT);
   pinMode(5, OUTPUT); // for triggering the controller to enter preset()
   pinMode(12, OUTPUT);
   pinMode(13, OUTPUT);
   
   pinMode(8, INPUT); // acknowledge for the lower byte
   pinMode(9, INPUT); // acknowledge for the upper byte
   pinMode(10, INPUT);
}

void loop() {
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  preset(); // ** remember to set voltage back to 0 before entering preset()
  while(1);

}

void preset()
{
  MCP4725.setVoltage(0, false); // reset DAC volt
  digitalWrite(1, LOW);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);

  // trigger the controller to enter preset()
  digitalWrite(5, HIGH);

  while (digitalRead(10)== LOW); ///***

  digitalWrite(5, LOW); 

  //// period
  // wait for MATLAB declaration for sending the period
  do
  {
    num = Serial.available();
   }
  while (num < 1);

  do
  {
    temp = Serial.read();
   }
  while (! (temp == 1));

  // acknowledge for being ready (send '2' to MATLAB)
  Serial.write(2);

  // read out the remaining 1
   do
  {
    temp = Serial.read();
   }
  while (temp == 1);
  // read the period
   do
   {
     num = Serial.available();
    }
   while (num < 1);
    
   period = Serial.read();

   sendtocontroller(period);

   Serial.write(4);
   
  /* //// amplitude
  //** must refresh the buffer (can sending leading zeros)

  // wait for MATLAB declaration for sending amplitude
   do
  {
    num = Serial.available();
   }
  while (num < 1);
  
  do
  {
    temp = Serial.read();
   }
  while (! (temp == 1));

  // acknowledge for being ready (send '4' to MATLAB)
  Serial.write(4);

  // read out the remaining 1
   do
  {
    temp = Serial.read();
   }
   while (temp == 1);
  // read the period
   do
   {
     num = Serial.available();
   }
   while (num < 1);
  
   amplitude = Serial.read();

   sendtocontroller(amplitude);

   Serial.write(6);

   //// Sending data
   
   //** must refresh the buffer (can sending leading zeros)
   
   // wait for MATLAB declaration for sending data
     do
    {
      num = Serial.available();
     }
    while (num < 1);
    
    do
    {
      temp = Serial.read();
     }
    while (! (temp == 1));

   // acknowledge for being ready (send '6' to MATLAB)
   Serial.write(6);
   
  
   // set pin 1 back to LOW
   digitalWrite(1, LOW); */ 

}

void sendtocontroller(unsigned char data)
{
  unsigned char upperbyte, lowerbyte, temp;
  upperbyte = data >> 4;
  lowerbyte = data - (upperbyte << 4);

  for (int i=4; i>0; i--)
  {
    temp = lowerbyte >> (i-1);
    if (temp == 1)
    {
      digitalWrite(i+1, HIGH);
    }
    else
    {
      digitalWrite(i+1, LOW);
    }
    lowerbyte = lowerbyte - (temp << (i-1));
  }

  digitalWrite(1,HIGH);

 // digitalWrite(12, HIGH);
  
  while(digitalRead(8) == LOW);

  
    
  for (int i=4; i>0; i--)
  {
    temp = upperbyte >> (i-1);
    if (temp == 1)
    {
      digitalWrite(i+1, HIGH);
    }
    else
    {
      digitalWrite(i+1, LOW);
    }
    upperbyte = upperbyte - (temp << (i-1));
  }

   digitalWrite(13, HIGH);
  
   digitalWrite(1,LOW);
   
   while(digitalRead(9)== LOW);
//
//  digitalWrite(13, HIGH);

  for (int i=4; i>0; i--)
  {
    digitalWrite(i+1, LOW);
  } 
}


unsigned int readdata(const int BUFFER_SIZE)
{
  unsigned char data[BUFFER_SIZE];
  unsigned int value;
  while (Serial.available() < BUFFER_SIZE);
  
  for (int i=0;i<BUFFER_SIZE;i++)
  {
    while(!(Serial.available() > 0));
    data[i] = Serial.read();
  }
  value=(data[1]<<8)+data[0];

  return value;
}
