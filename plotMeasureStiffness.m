clear;
close all;

%% Load data
run('/home/haquang/workspace/rtai/TSAControl/ps_human_Oid2N1.m')


N = length(data(1,:));

%% Plot

for i = 6: N
    figure()
    plot(data(:,i));
end
