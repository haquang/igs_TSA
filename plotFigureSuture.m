clear;
close all;

%% Load data


run('/home/haquang/workspace/rtai/TSAControl/position.m')

start = 1;
Nsamples =length(data(:,1));

xm = data(start:Nsamples,2);
Fm = -data(start:Nsamples,4);
Fs = -data(start:Nsamples,3);


%% Plot

t = (0:length(Fm)-1);

figure('Units','centimeters','Position', [0, 0, 12, 9]);
set(0, 'defaultAxesTickLabelInterpreter','latex');
set(0, 'defaultLegendInterpreter','latex');
set(0, 'DefaultAxesFontSize',10);

range_y = 6;
range_x = Nsamples;

subplot(2,1,1);
plot(xm,'-r','LineWidth',1.5,'color',nicecolor('byk'));
ylim([0 range_y]);
xlim([0 range_x]);
title('Position','Interpreter','latex','fontsize', 10);
grid on;

range_y = 4;
subplot(2,1,2);
plot(Fs,'-r','LineWidth',1.5,'color',nicecolor('byk'));
hold on;
plot(Fm,'LineWidth',1.5,'color',nicecolor('rr'));
ylim([0 range_y]);
xlim([0 range_x]);

set(gca,'ytick',[0 range_y]);
legend('Fd','Fm','Orientation','vertical','Location','Best','Interpreter','latex');
title('Force Tracking','Interpreter','latex','fontsize', 10);
grid on

% % Save figure
set(gcf, 'PaperUnits', 'centimeters');
set(gcf, 'PaperPosition', [0 0 12 7]); 
print('SuturePlot','-dpng','-r1200');

