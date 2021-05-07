
Matlab:
Declare periodStatus, amplitudeStatusand dataStatus registers;
Define every settings about serial communication

preset();
while (1) {
    send data repeatedly;
}

void preset(void) {//Digital in/out are all in Arduino
    //send period
    //Send 1 to Digital input 1;// declare sending period;
    //delay(100);
    //while (!(get 1 from Digital output 1));//wait until controller is ready for period
    while (!(get 1 from Digital output 1))//wait until controller is ready for period, otherwise, repeatedly declaring
        send 1 to Digital input 1;// declare sending period
    while (!(get 1 from Digital output 2))//if controller has not get period, send repeatedly 
        Send period continuously;
    // now ,Digital output 2 is 1, meaning controller has get period.

    //send amplitude
    //send 1 to Digital input 2; // declare sending amplitude;
    //delay(100);
    // here, controller will set digital output 2 as 0 again. Next time when it is 1, it means controller has ready for amplitude
    while (!(get 1 from Digital output 2));//wait until controller is ready for amplitude. otherwise, repeatedly declaring
        send 1 to Digital input 2;
    while (!(get 1 from Digital output 3))//if controller has not get amplitude, send repeatedly
        Send amplitude continuously;
    // now, Digital output 3 is 1, meaning controller has get amplitude
    // here, controller will set digital output 3 as 0 again. Next time when it is 1, it means controller has ready for data

    //send data
    //send 1 to Digital input 3;// delcare sending data
    //delay(100);
    while (!(get 1 from Digital output 3));//wait until controller is ready for data. Otherwise, repeatedly declaring
        send 1 to Digital input 3;// delcare sending data
}



Arduino:

void main() {
    Define starting-mark, meaning the start of a dataset
        //Control Arduino to send high, interrupting itself;// let Arduino know a new set of data is comming
        preset();
        read 5 bytes first anyway;//read starting-mark
    while (1)//read data from Matlab and send it immediately. Meanwhile, keep checking for starting-mark(new data package)
    {
        if (the 5 bytes read == starting-mark)// a new data set is comming
            preset();
        // else, the first 5 bytes are all data points, we need run them one by one
        send the 1st byte to controller analog 2
    }
}

void preset(void) {// Digital in/out are all in Controller
    //read period
    while (!Digital input 1 == 1);// wait until Matlab declare sending period
    send 1 through Arduino Digital output 1;// say I am ready for reading period
    do {
        read analog value into period;
    }
    // some function to determine if read
    while (!period)//repeatedly reading period until it is not 0
    //send period
    while (!(get 1 from Digital output 1))//wait until controller is ready for period, otherwise, repeatedly declaring
        send 1 to Digital input 1;// declare sending period
    while (!(get 1 from controller Digital output 2))//if controller has not get period, send repeatedly 
        Send period continuously;
    // now ,DIgital output 2 is 1, meaning controller has get period, and controller is waiting for sending amplitude flag
    send 1 through Arduino Digital output 2;// declare that period has been reveived. finish transition. 

    // read amplitude
    // now, Matlab is also waiting declare sending amplitude and wait for a 1 to send
    while (Digital input 2 == 0);// wait until Arduino to send 1, saying it is ready to send amplitude
    send 1 through Digital output 2;// say I'm ready to receive amplitude
    do {
        read analog value into amplitude;
    } while (!amplitude)//repeatedly reading amplitude until it is not 0
    //send amplitude
    // here, controller has set digital output 2 as 0 again by itself, because the code moves to next line
    // Next time when it is 1, it means controller has ready for amplitude
    while (!(get 1 from Digital output 2));//wait until controller is ready for amplitude. otherwise, repeatedly declaring
        send 1 to Digital input 2;
    while (!(get 1 from Digital output 3))//if controller has not get amplitude, send repeatedly
        Send amplitude continuously;
    // now, Digital output 3 is 1, meaning controller has get amplitude
    // here, controller will set digital output 3 as 0 again. Next time when it is 1, it means controller has ready for data
    send 1 through Digital output 3;// declare that amplitude has been received.finish transition

    
    //send data
    send 1 to Digital input 3;// delcare sending data
    delay(100);
    while (!(get 1 from Digital output 3));//wait until controller is ready for data
}


Controller:

void main() {
    Finite state settingsand declare all necessary variables, like period, amplitudeand data;
    preset();
    while (1)//here, evrything comming is data
    {
        read analog;
        signal interpret;
        run;
        if (get 1 from Digital output 1)//if get the flag indicating a data package is comming, preset
            preset();
    }
}

void preset(void) {
    //Read period
    while (!Digital input 1 == 1);// wait until Matlab declare sending period
    send 1 through Digital output 1;// say I am ready for reading period
    do {
        read analog value into period;
    }
    while (!period)//repeatedly reading period until it is not 0
    send 1 through Digital output 2;// say I've got period, finish transition
    // after next line, the Digital output 2 will clear itself automaticly
    //read amplitude
    //while (!Digital input 2);// wait 
    //send 1 through Digital output 2;// say I've got period
    //clear Digital ouput 2;
    while (Digital input 2 == 0);// wait until Arduino to send 1, saying it is ready to send amplitude
    send 1 through Digital output 2;// say I'm ready to receive amplitude
    do {
        read analog value into amplitude;
    }
    while (!amplitude)//repeatedly reading amplitude until it is not 0
    send 1 through Digital output 3;// declare that amplitude has been received.finish transition

    //read data
    while (Digital input 3 == 0);// wait until Arduino to send 1, saying it is ready to send data
    send 1 through Digital output 3;// say I'm ready to receive data
    // data will be read outside the preset() function.
}




clear
clc
baud = 115200;
period = 2500;
signal_SP = 16 / baud * 1000;
t = 0:signal_SP:period;
mag = round(2047 * sin(2 * pi / period.*t));
mag = mag + 2047;
device = serialport('COM4', baud, "Timeout", 5);
while (1)
tic;
write(device, mag, "uint16");
t = toc;
disp(t);
end




