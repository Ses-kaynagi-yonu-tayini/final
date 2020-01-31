clc; clear all;
close all;
delete(instrfind({'Port'},{'COM3'}));

s = serial('COM3','BaudRate',115200,'DataBits',8);

fclose(s);
fopen(s);

while true
    i = 1;

    z = zeros(512,6);
    while true
        % verinin toplanacaðý yer
        temp = fscanf(s);

        C = strsplit(temp,' ');
        [L,M] =size(C);

        if M == 6

            for j = 1:6
                   z(i,j) = str2double(C(j));
            end
            i = i + 1;
            if i == 513
                break;
            end
        end    
    end

    BUFFER = 512;

    Fx = zeros(BUFFER/2,6);

    [Fx(:,1),X] =  FFT(z(:,1));
    [Fx(:,2),X2] = FFT(z(:,2));
    [Fx(:,3),X3] = FFT(z(:,3));
    [Fx(:,4),X4] = FFT(z(:,4));
    [Fx(:,5),X5] = FFT(z(:,5));
    [Fx(:,6),X6] = FFT(z(:,6));

    res = findPeaks3(Fx);
    fprintf("%.1f\n",res);
    fprintf(s,int2str(res));

end
fclose(s);

clear s