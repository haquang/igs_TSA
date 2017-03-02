%close all;
clear;
run('/home/haquang/workspace/rtai/TSAControl/position.m')

start= 1;
Nsamples = length(data(:,1));
%Nsamples = 10000;
%k_set(start:Nsamples) = 1;

idx = find(data(:,2) > 0.3,1);
idx2 = find(abs(data(:,3)) >= max(abs(data(:,3))),1);

xs = data(idx:idx2,1);
xm = data(idx:idx2,2);
Fm = -data(idx:idx2,4);
Fs = -data(idx:idx2,3);

km = Fm ./ xm;
ks = Fs ./ xm;

%% Plot
figure();
plot(km);
hold on
plot(ks);

figure();

plot(xm,'r');
hold on
plot(xs);

figure();

plot(Fm,'r');
hold on
plot(Fs);