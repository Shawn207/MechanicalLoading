#include <Wire.h>           //wire library
#include <Adafruit_MCP4725.h> // MCP4725 library from adafruit
#define analogVin A0          // Analog voltage input to A0

Adafruit_MCP4725 MCP4725; 

void preset();
unsigned int readdata(const int BUFFER_SIZE);

unsigned char period, amplitude;
unsigned char temp;
int num;
int DACvolt;

void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200);
   MCP4725.begin(0x60); // Default I2C Address of MCP4725 breakout board (sparkfun) 
   pinMode(9, OUTPUT);
   
   pinMode(1, OUTPUT); // for triggering preset() in the controller
   pinMode(2, OUTPUT); // for declaring sending the period
   pinMode(3, INPUT); 
   pinMode(4, OUTPUT);
   pinMode(5, INPUT); 
}

void loop() {
  digitalWrite(4, LOW);
  preset(); // ** remember to set voltage back to 0 before entering preset()
  while(1);

}

void preset()
{
  MCP4725.setVoltage(0, false); // reset DAC volt
  digitalWrite(2, LOW);
  digitalWrite(4, LOW);

  // trigger the controller to enter preset()
  digitalWrite(1, HIGH);

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
  
   period = Serial.read();

   DACvolt=(int)period*4096.0/255;
   
   // declare for sending period to motor controller
   digitalWrite(2, HIGH); 
   
   MCP4725.setVoltage(DACvolt, false); // send period
   
   while (digitalRead(3) == LOW); // wait until the period is received
   
   MCP4725.setVoltage(0, false);
   digitalWrite(2, LOW);

   Serial.write(4); // acknowledge for receiving the period

   //// amplitude
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
  
   amplitude = Serial.read();

   DACvolt=(int)amplitude*4096.0/255;
   
   // declare for sending amplitude to motor controller
   digitalWrite(4, HIGH); 

   MCP4725.setVoltage(DACvolt, false); // send the amplitude
   
   while (digitalRead(5) == LOW); // wait until the amplitude is received
   
   MCP4725.setVoltage(0, false);
   digitalWrite(4, LOW);

   Serial.write(5); // acknowledge for receiving the amplitude

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
   digitalWrite(1, LOW); 

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
