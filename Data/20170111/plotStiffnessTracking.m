close all;
clear;

%% Initialize
K = [0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9];

%% Loading 
path = pwd;
path = strcat(path,'/');
Nsamples = 21000;

N = length(K);

%FigureSetup(1,15,9);
%set(0,'DefaultAxesColorOrder',brewermap(N,'Set1'))

bg = {'w','k'};
 
legend_str = cell(length(K),1);
figure();

for i = 1 : length(K) 

    file = strcat(path,num2str(K(i),'%1.1f'),'/','position.m')

    run(file)

    index = find(data(:,4)==0,1,'last') + 10
    N = length(data(:,4));
    xm =  data(index:N,5);
    Fm = -data(index:N,3);
    Fs = -data(index:N,4);
    
    Km = Fm ./xm;
    Ks = Fs ./xm;
    
    subplot(3,3,i);
    plot(Km,'LineWidth',2.0);
    hold on;
    plot(Ks,'--','LineWidth',2.0);
    ylim([0 1.2]);
    %% Plot figure
end


%% Plot figure

% Plot real stiffness
%file = strcat(path,'/real/position.m');    
%run(file)
%xm = data(3000:7000,9) - data(6414,9);

%Fm = -data(3000:7000,12);
%plot(xm,Fm,'LineWidth',1.5,'DisplayName','real');

   
%xlim([0 12]);
%ylim([-0.5 9.0]);

%columnlegend(5,legend_str,'location','NorthWest');

xlabel('Displacement (mm)');
ylabel('Force (N)');
grid off;
box off;

LegendMatrix(legend_str',[4.1 7.5 7.10 1.2],12,3);
legend boxoff 

%PrintRoutine(1200,'StiffnessPlot');

%set(gcf, 'PaperUnits', 'centimeters');
%set(gcf, 'PaperPosition', [0 0 18 7]); 
%print('StiffnessPlot','-dpng','-r800');