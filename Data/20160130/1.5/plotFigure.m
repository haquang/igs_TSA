close all;
clear;
path = pwd;
path = strcat(path,'/position.m');
run(path);

start= 1;
Nsamples = length(data(:,1));
%Nsamples = 10000;
%k_set(start:Nsamples) = 1;

idx = find(data(:,2) > 0.1,1);
idx2 = find(abs(data(:,3)) >= max(abs(data(:,3))),1);

xs = data(idx:idx2,1);
xm = data(idx:idx2,2);
Fm = -data(idx:idx2,4);
Fs = -data(idx:idx2,3);

km = Fm ./ xm;
ks = Fs ./ xm;


figure()
subplot(2,1,1)
plot(xs,'--r')
hold on
plot(xm,'b')
hold on;
grid on;
legend('design','measure');
title('Position');

subplot(2,1,2)
plot(Fs,'r');
hold on;
plot(Fm,'k');
legend('design','measure');
title('Force');
grid on


figure();
plot(xm,Fm,'k');

k = Fm./xm;
ks = Fs./xm;
figure();
plot(k,'k');
hold on
plot(ks,'r');
