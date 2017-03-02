close all;
clear;
run('/home/haquang/workspace/rtai/TSAControl/position.m')

%%

x_enc = 0.0003.*data(:,1);
x_m = data(:,2);


%% plot

figure()
plot(x_m,'r');
hold on
plot(x_enc);