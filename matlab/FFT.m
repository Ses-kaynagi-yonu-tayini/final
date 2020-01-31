function [Fx,X] = FFT(Data)
    N = length(Data);
    Ts = 10^-3;
    W = 0:2*pi/N:2*pi*(1-1/N);
    X = W / Ts;
    Fx = fft(Data);
    X = X(1:N/2);
    Fx = abs(Fx(1:N/2));
end