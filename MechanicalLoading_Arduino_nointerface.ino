/*
 * Arduino Digital to Analog Coverter MCP4725 Interface code
 * Library from Adafruit-MCP4725
 * see text at www.theorycircuit.com
 */
#include <Wire.h>           //wire library
#include <Adafruit_MCP4725.h> // MCP4725 library from adafruit
#define analogVin A0          // Analog voltage input to A0

Adafruit_MCP4725 MCP4725; 
const int BUFFER_SIZE = 2;
char buf[BUFFER_SIZE];
int DAC_volt;
unsigned char data[2];

void setup(void) {
  Serial.begin(115200);
  MCP4725.begin(0x60); // Default I2C Address of MCP4725 breakout board (sparkfun)  
  pinMode(9, OUTPUT); 
}

void loop(void) {
buf[1]=0;
buf[0]=0;
if (Serial.available() >1) {
    // read the incoming bytes:
    //int rlen = Serial.readBytes(buf, BUFFER_SIZE);

    for (int i=0;i<2;i++)
    {
      while(!(Serial.available() > 0));
      data[i] = Serial.read();
    }
    DAC_volt=(data[1]<<8)+data[0];
    
    MCP4725.setVoltage(DAC_volt, false);
}
}
