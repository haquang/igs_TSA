close all;
clear;
run('/home/haquang/workspace/rtai/TSAControl/position.m')

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
figure()
subplot(2,1,1)
plot(xs,'--r')
hold on
plot(xm,'b')
hold on;
hold on
plot(xm - xs);
grid on;
legend('design','measure');
title('Position');

subplot(2,1,2)
plot(Fs,'r');
hold on;
plot(Fm,'k');
hold on
plot(Fm - Fs);
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


%figure();
%plot(data(start:Nsamples,1))
%hold on;
%plot(data(start:Nsamples,3),'--r')
%{
run('/home/haquang/workspace/rtai/TSAControl/position_medium.m')


%figure(1);
%plot(data(:,2));

figure(2);
subplot(2,1,1)
plot(data(:,1))
hold on
plot(data(:,2),'r')
hold on
plot(-data(:,3),'k');
legend('design','actual','force');


subplot(2,1,2)
plot(data(:,2),-data(:,3),'k')
legend('force');
grid on


%figure(1);
%plot(data(:,2));

run('/home/haquang/workspace/rtai/TSAControl/position_hard.m')

%figure(1);
%plot(data(:,2));

figure(3);
subplot(2,1,1)
plot(data(:,1))
hold on
plot(data(:,2),'r')
hold on
plot(-data(:,3),'k');
legend('design','actual','force');


subplot(2,1,2)
plot(data(:,2),-data(:,3),'k')
legend('force');
grid on

%}