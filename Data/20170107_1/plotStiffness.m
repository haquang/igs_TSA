close all;
clear;

%% Initialize
K = [0.01 0.1 0.2 0.4 0.8 1.0 1.5 2.0 2.5 3.0];

%% Loading 
path = pwd;
path = strcat(path,'/');
Nsamples = 21000;

N = length(K);

%FigureSetup(1,15,9);
%set(0,'DefaultAxesColorOrder',brewermap(N,'Set1'))

bg = {'w','k'};
 
legend_str = cell(length(K),1);

for i = 1 : length(K)
    if i == 1
        file = strcat(path,num2str(K(i),'%1.2f'),'/','position.m')        
    else
        file = strcat(path,num2str(K(i),'%1.1f'),'/','position.m')
    end
   
    run(file)

    xm =  data(:,2);
    Fm = -data(:,4);
    Fs = -data(:,3);
    
    %% Plot figure
    plot(xm,Fm,'LineWidth',2.0);
    legend_str{i} = num2str(K(i),'%1.2f');
%    pause;
    hold all
end


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

LegendMatrix(legend_str',[4.1 7.5 7.10 1.2],12,5);
legend boxoff 

%PrintRoutine(1200,'StiffnessPlot');

%set(gcf, 'PaperUnits', 'centimeters');
%set(gcf, 'PaperPosition', [0 0 18 7]); 
%print('StiffnessPlot','-dpng','-r800');