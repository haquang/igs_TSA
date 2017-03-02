clear;
close all;

%% Load data
run('/home/haquang/workspace/rtai/TSAControl/position_soft.m')
start = 8000;
Nsamples =18000;

xm1 =  data(start:Nsamples,2);
Fm1 = -data(start:Nsamples,4);
Fs1 = -data(start:Nsamples,3);

P = polyfit(xm1,Fm1,1);
Fm1_fit = P(1) * xm1 + P(2);


run('/home/haquang/workspace/rtai/TSAControl/position_medium.m')

start = 5000;
Nsamples =15000;


xm2 = data(start:Nsamples,2);
Fm2 = -data(start:Nsamples,4);
Fs2 = -data(start:Nsamples,3);

P = polyfit(xm2,Fm2,1);
Fm2_fit = P(1) * xm2 + P(2);

run('/home/haquang/workspace/rtai/TSAControl/position_hard.m')

start = 3000;
Nsamples =13000;


xm3 = data(start:Nsamples,2);
Fm3 = -data(start:Nsamples,4);
Fs3 = -data(start:Nsamples,3);


P = polyfit(xm3,Fm3,1);
Fm3_fit = P(1) * xm3 + P(2);

%% Plot

t = (0:0.0001:(length(Fm1)-1)/10000);

figure('Units','centimeters','Position', [0, 0, 20, 4]);
set(0, 'defaultAxesTickLabelInterpreter','latex');
set(0, 'defaultLegendInterpreter','latex');
set(0, 'DefaultAxesFontSize',15);

range_y = 5;
range_x = 1;

hAxis(1) = subplot(1,3,1);
plot(t,Fs1,'-r','LineWidth',2.5,'color',nicecolor('byk'));
hold on;
plot(t,Fm1,'LineWidth',2.5,'color',nicecolor('rr'));
ylim([0 range_y]);
xlim([0 range_x]);
set(gca,'ytick',[0 range_y]);
ylabel('Force (N)');
title('Soft','Interpreter','latex','fontsize', 15);
grid on



subplot(1,3,2);
plot(t,Fs2,'-r','LineWidth',2.5,'color',nicecolor('byk'));
hold on;
plot(t,Fm2,'LineWidth',2.5,'color',nicecolor('rr'));
ylim([0 range_y]);
xlim([0 range_x]);
set(gca,'ytick',[0 range_y]);
xlabel('Time (s)');

title('Medium','Interpreter','latex','fontsize', 15);
grid on


subplot(1,3,3);
plot(t,Fs3,'-r','LineWidth',2.5,'color',nicecolor('byk'));
hold on;
plot(t,Fm3,'LineWidth',2.5,'color',nicecolor('rr'));
ylim([0 range_y]);
xlim([0 range_x]);
set(gca,'ytick',[0 range_y]);
h = legend('Fd','Fm','Orientation','horizonal','Location','Best','Interpreter','latex');
pos = get(h,'position');
rect = [pos(1), pos(2)-0.41, pos(3:4)];
set(h, 'Position', rect);

title('Hard','Interpreter','latex','fontsize', 15);
grid on



% % Save figure
set(gcf, 'PaperUnits', 'centimeters');
set(gcf, 'PaperPosition', [0 0 20 4]); 
print('ForcePlot','-dpng','-r500');


figure('Units','centimeters','Position', [0, 0, 12, 6]);
set(0, 'defaultAxesTickLabelInterpreter','latex');
set(0, 'defaultLegendInterpreter','latex');
set(0, 'DefaultAxesFontSize',15);

range_y = 5.0;

scatter(xm1,Fm1,0.25,'r');
hold on;
plot(xm1,Fm1_fit,'r','LineWidth',3,'Color',nicecolor('Mrrk'));
hold on;
txt = '\rightarrow Soft';
text(max(xm1),max(Fm1_fit),txt,'fontsize', 15)

scatter(xm2,Fm2,0.25,'b');
hold on;
plot(xm2,Fm2_fit,'b','LineWidth',3,'Color',nicecolor('Mbbk'));
hold on;
txt = '\rightarrow Medium';
text(max(xm2),max(Fm2_fit),txt,'fontsize', 15)

scatter(xm3,Fm3,0.25,'k');
hold on;
plot(xm3,Fm3_fit,'k','LineWidth',3,'Color',nicecolor('Mk'));
hold on;
txt = '\rightarrow Hard';
text(max(xm3),max(Fm3_fit),txt,'fontsize', 15)

set(gca,'ytick',[0 range_y]);
title('Stiffness','Interpreter','latex','fontsize', 15);
grid minor
ylim([0 range_y]);

xlabel('Displacement (mm)');
ylabel('Force (N)');

set(gcf, 'PaperUnits', 'centimeters');
set(gcf, 'PaperPosition', [0 0 12 6]); 
print('StiffnessPlot','-dpng','-r1200');

