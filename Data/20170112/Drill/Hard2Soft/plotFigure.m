close all;
clear;

path = pwd;
path = strcat(path,'/position.m');
run(path);

start= 1;

%Nsamples = 10000;
%k_set(start:Nsamples) = 1;
%idx = find(data(:,2) > 0.1,1);
%idx2 = find(abs(data(:,3)) >= max(abs(data(:,3))),1);

%Nsamples = length(data(:,1));
 start = find(data(:,4)==0,1,'last') + 10
Nsamples = length(data(:,4));

xs = data(start:Nsamples,1);
xtsa = data(start:Nsamples,2);

Fm = -data(start:Nsamples,3);
Fs = -data(start:Nsamples,4);
xm = data(start:Nsamples,5);


km = Fm ./xm;
ks = Fs ./xm;


figure()
subplot(2,1,1)
plot(xs,'--r')
hold on
plot(xtsa,'b')
hold on;

grid on;
legend('design','measure');
title('Position');

subplot(2,1,2)
plot(Fs,'r');
hold on;
plot(Fm,'k');
legend('design','measure');


figure();
plot(xm,Fm);


figure();
plot(km);
hold on;
plot(ks);