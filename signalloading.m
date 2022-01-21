function signalloading(t,mag)
    if (length(t)~=length(mag))
        fprintf('Error!!! Vector sizes aren''t consistent!\n');
    else
        fileID = fopen('signalloading.cpp','w');
        fprintf(fileID,'map U16 StatusWord as input 0x6041:00\n');
        fprintf(fileID,'#include "wrapper.h"\n\n');
        fprintf(fileID,'void user()\n{\n');
        num=length(t);
        fprintf(fileID,'int samplingnum = %d;\n',num);
        fprintf(fileID,'int timevector[%d];\n',num); 
        for i=1:length(t)
            fprintf(fileID,'timevector[%d]=%d;\n',i-1,t(i));
        end
        
        fprintf(fileID,'int sampling_period = timevector[1]-timevector[0];\n');

        fprintf(fileID,'int mag[%d];\n',num); 
        for i=1:length(mag)
            fprintf(fileID,'mag[%d]=%d;\n',i-1,mag(i));
        end
        fprintf(fileID,'od_write(0x6060, 0x00, 3); // Set mode "Profile velocity"\n'); 
        fprintf(fileID,'od_write(0x60FF, 0x00, 0); // Set the target velocity\n\n');
        fprintf(fileID,'od_write(0x6040, 0x00, 0x6); // Request state "Ready to switch on"\n');
        fprintf(fileID,'while ((In.StatusWord & 0xEF) != 0x21) {\n');
        fprintf(fileID,'yield(); // Wait for the next cycle (1ms)\n');
        fprintf(fileID,'}\n\n');
        fprintf(fileID,'od_write(0x6040, 0x00, 0x7); // Request the state "Switched on"\n');
        fprintf(fileID,'while ((In.StatusWord & 0xEF) != 0x23) {\n');
		fprintf(fileID,'yield();\n');
        fprintf(fileID,'}\n\n');
        fprintf(fileID,'od_write(0x6040, 0x00, 0xF); // Request the state "Operation enabled"\n');
    	fprintf(fileID,'while ((In.StatusWord & 0xEF) != 0x27) {\n');
		fprintf(fileID,'yield();\n');
        fprintf(fileID,'}\n\n');

        fprintf(fileID,'sleep(100);\n');

        fprintf(fileID,'while (1) //main loop\n');
        fprintf(fileID,'{\n');
        fprintf(fileID,'for (int i = 0; i < samplingnum; i++)\n');
        fprintf(fileID,'{\n');
		fprintf(fileID,'// Set the target velocity\n');
        fprintf(fileID,'od_write(0x60FF, 0x00, mag[i]);\n');

		fprintf(fileID,'// Let the motor run for a while\n');
		fprintf(fileID,'sleep(sampling_period);\n');
		fprintf(fileID,'}\n');
        fprintf(fileID,'}\n');
        fprintf(fileID,'}\n');
        fclose(fileID); 
    end
end