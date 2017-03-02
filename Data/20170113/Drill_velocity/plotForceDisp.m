clear all;
close all;

%% Load

K = [1.0 2.0 3.0 4.0 5.0 6.0];

path = pwd;
path = strcat(path,'/');

N = length(K) + 1;

rots = 3.0;
sat = 1.69;
gamma = 0.86;
start_map = 1.58;
irange = [0.3;0.66];
domain = [0.1;0.72];
map = cubehelix(N,start_map,rots,sat,gamma,irange,domain);
FigureSetup(1,15,7);
%set(0,'DefaultAxesColorOrder',brewermap(N,'Set1'))
set(0,'DefaultAxesColorOrder',map)
set(0, 'DefaultAxesFontSize',12);
set(0, 'defaultAxesTickLabelInterpreter','latex');

bg = {'w','k'};
 
legend_str = cell(length(K),1);


for i = 1 : length(K) 

    file = strcat(path,num2str(K(i),'%1.1f'),'/','position.m')

    run(file)

     index = find(data(:,4)==0,1,'last') + 10
    NSamples = length(data(:,4));
    xm =  data(index:NSamples,5);
    Fm = -data(index:NSamples,3);
    Fs = -data(index:NSamples,4);
   
    %% Plot figure
    plot(xm,Fm,'LineWidth',2.0);
        
    legend_str{i} = num2str(K(i),'%1.2f');

   % pause;
    hold all
end


LegendMatrix(legend_str',[2.1 5.2 5.10 1.2],12,5);