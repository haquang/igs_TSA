close all;
clear;
path = pwd;
path = strcat(path,'/position.m');

run(path);
start= 1;
Nsamples = length(data(:,1));
%Nsamples = 10000;
%k_set(start:Nsamples) = 1;

figure()
subplot(2,1,1)
plot(data(start:Nsamples,1),'--r')
hold on
plot(data(start:Nsamples,2),'b')
hold on;
grid on;
legend('design','measure');
title('Position');

subplot(2,1,2)
plot(data(start:Nsamples,3),'r');
hold on;
plot(data(start:Nsamples,4),'k');
legend('design','measure');
title('Force');
grid on


figure();
plot(data(start:Nsamples,2),-data(start:Nsamples,4),'k');


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