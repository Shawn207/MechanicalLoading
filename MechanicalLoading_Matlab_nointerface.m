clear
clc
baud=115200;
period=1000;
signal_SP=16/baud*1000;
t=0:signal_SP:period;
mag=round(2047*sin(2*pi/period.*t));
mag=mag+2047;
device=serialport('COM4',baud,"Timeout",10);
while(1)
    tic;
    write(device,mag,"uint16");
    t=toc;
    disp(t);
end





