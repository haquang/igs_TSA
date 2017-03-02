close all;
clear;
run('/home/haquang/workspace/rtai/TSAControl/position.m')

%% Load data
run('/home/haquang/workspace/rtai/TSAControl/position.m')

start = 1;
Nsamples =length(data(:,1));

xm = data(start:Nsamples,2);
Fm = -data(start:Nsamples,4);
Fs = -data(start:Nsamples,3);

idx = find(xm > 1,1);

m = max(xm);

idx2 = find(xm >= m,1,'first');

x1 = xm(start:idx);
x2 = xm(idx:idx2);
x3 =  xm(idx2:Nsamples);

F1 = -data(start:idx,4);
F2 = -data(idx:idx2,4);
F3 = -data(idx2:Nsamples,4);

P = polyfit(x1,F1,1);
Fm1_fit = P(1) * x1 + P(2);

P = polyfit(x2,F2,1);
Fm2_fit = P(1) * x2 + P(2);

P = polyfit(x3,F3,1);
Fm3_fit = P(1) * x3 + P(2);
%% Plot
figure('Units','centimeters','Position', [0, 0, 12, 9]);
set(0, 'defaultAxesTickLabelInterpreter','latex');
set(0, 'defaultLegendInterpreter','latex');
set(0, 'DefaultAxesFontSize',12);

scatter(x1,F1,0.1,'r');
hold on;
plot(x1,Fm1_fit,'r','LineWidth',3,'Color',nicecolor('Mrrk'));
hold on;

scatter(x2,F2,0.1,'b');
hold on;
plot(x2,Fm2_fit,'b','LineWidth',3,'Color',nicecolor('Mbbk'));
hold on;

%scatter(x3,F3,0.1,'k');
%hold on;
%plot(x3,Fm3_fit,'b','LineWidth',3,'Color',nicecolor('Myyk'));

grid on

