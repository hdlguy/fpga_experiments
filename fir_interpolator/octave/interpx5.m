%
clear;

D = 5;
Nfir = 34;
Wc = 1/D;

b = fir1(Nfir, Wc)*D;

%plot(b, '*-r');

format long;

for i=1:Nfir+1
    printf("%4.10f, ", b(i));
endfor
printf("\n");

