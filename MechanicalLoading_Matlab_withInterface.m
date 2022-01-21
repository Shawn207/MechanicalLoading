clear
clc
%%
%prep for signal generation
period = 100;
amplitude = 150;
baud= 115200;
signal_SP=16/baud*1000;
t=0:signal_SP:period;
mag=round(2047*sin(2*pi/period.*t));
mag=mag+2047;
%%
%main function

%start preset: transmit period and amplitude to prepare for the
%communication
device=serialport('COM8',baud,"Timeout",2);
[periodFlag,amplitudeFlag,dataFlag]= preset(device,period, amplitude);
    %delcare variables 
while(1)
    %tic;
    write(device,mag,"uint16");
    %t=toc;
    %disp(t);
end


%% def of preset
function [periodFlag,amplitudeFlag,dataFlag]= preset(device,period,amplitude)
    %delcare variables       
    periodFlag = 0;
    amplitudeFlag = 0;
    dataFlag = 0;
% we find out that Matlab won't read anything write by itself
% in another word, it only read num written by Arduino, which is perfect

    % send period
    periodFlag = read(device,1,"uint8");
    %pause(6);%IMPORTANT: 3 seconds' wait is needed for Arduino to read 2, but this number is based on experiment. it may vary
    while(length(periodFlag)==0 ||periodFlag ~= 2)%wait until Arduino is ready for period, otherwise, repeatedly declaring
        write(device,1,"uint8");%declare sending period
        periodFlag = read(device,1,"uint8");% read one more int to test if it is 2
        display(periodFlag);
    end
    display("handshake succeed");
    % send three 0s to remind Arduino period is comming
%     write(device,0,"uint8");
%     write(device,0,"uint8");
%     write(device,0,"uint8");
    
    while(periodFlag ~= 4)%if controller has not get period, send repeatedly 
        write(device,period,"uint8");%send period 
        periodFlag = read(device,1,"uint8");
    end
    display("get!")
%     display("pass")
%     flush(device);
%     for i=3:100
%         write(device,i,"uint8");
%         data = read(device,1,"uint8");
%         display(data)
%     end
    
%     %% test only
%     start = [115,116,97,114,116];
%     %start = [115,116,115,116];
%     while(1)
%         for i=1:5                                                                                                                                                                                                                  
%             %write(device,start(i),"uint16");
%             data = read(device,1,"uint16");
%            display(data);
%         end
%     end
end


