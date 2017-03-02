function LegendMatrix(labels,POS,fs,col)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Function 'LegendMatrix':
%
% Builds a custom legend box based on an existing legend. Recently, MATLAB
% changed the 'legend' command such that it has a minimum size which cannot
% be changed. This circumvents the existing legend command and adds several
% enhancements.
%
% Input Variables:
%
%         labels: Cell array of strings for the legend labels. This should
%                   be formatted [{'Label1'},{'Label2'},... ]
%         POS: standard 1x4 matrix for position in units of centimeters
%                   [x0 y0 w h]
%         fs: font size for the legend entries labels.
%         col: number of columns for legend entries to be split into
%
% Usage Notes:
% 'LegendMatrix' creates a new legend with the ability to change the size
% of the legend box outside of the allowable bounds by Matlab.
% Additionally, this function gives you the option of making legend entries
% with multiple columns. The size and position of the legend
% box is specified with a standard 1x4 matrix in units of centimeters. The
% position starts in the lower left corner and the width/height of the
% legend are specified. When specifying multiple columns, the colums are
% divided at equal intervals and aligned left based on the width of the
% legend box. The position command is defined in centimeters and can be
% placed anywhere inside (or outside) the figure box. The size of the lines
% and spacing is tied to the fontsize of the text specified.
%
% Note: when passing through linespec handles with markers, you
% must specify at least 'Marker' and 'MarkerEdgeColor' for this function to
% work properly.
%
% Written by Jonathan D. Ellis, PhD
% Updated September 25, 2015
% Version: 1.00
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

delete( findobj('Tag','LegendMatrixTag') )

labels = cellstr(labels);

allAxes = findall(gcf,'type','axes');

h = allAxes(1).Children;

for i = 2:length(allAxes)
    h = [h ; allAxes(i).Children];
end
h = flipud( h );


M = ceil(length(h)/col);

AX = axes;
AX.Box = 'on';
AX.Units = 'centimeters';
AX.Position = POS;
AX.XTick = [];
AX.YTick = [];
AX.Units = 'points';
AX.LineWidth = 0.25;
A = AX.Position;
AX.XLim = [0 A(3)];
AX.YLim = [0 A(4)];
AX.Tag = 'LegendMatrixTag';

W = A(3) / col;
H = A(4) / M;

Top = A(4);


j = 0;
N = 0;

for i = 1:length(h)
    if isequal(get(h(i),'LineStyle'),'--')==1
        l1 = line( [3+j*W 6+j*W] , [Top-H/2-(N)*H+1 Top-H/2-(N)*H+1] );
        l1.Color = get(h(i),'Color');
        l1.LineWidth = get(h(i),'LineWidth')*1.5;
        
        l2 = line( [8+j*W 11+j*W] , [Top-H/2-(N)*H Top-H/2-(N)*H] );
        l2.Color = get(h(i),'Color');
        l2.LineWidth = get(h(i),'LineWidth')*1.5;
    else
        l = line( [3+j*W 11+j*W] , [Top-H/2-(N)*H+1 Top-H/2-(N)*H+1] );
        l.Color = get(h(i),'Color');
        l.LineWidth = get(h(i),'LineWidth')*1.25;
        l.LineStyle = get(h(i),'LineStyle');
    end
   
    m = line( [4.5+j*W+1] , [Top-H/2-(N)*H+1] );
    m.Marker = get( h(i) , 'Marker' );

    m.MarkerFaceColor = get(h(i),'MarkerFaceColor');
    m.MarkerEdgeColor = get(h(i),'MarkerEdgeColor');
    m.LineStyle = 'none';

    t = text( 13+j*W , Top-H/2-(N)*H , labels(i) );
    t.FontSize = fs;
    t.FontName = 'Times New Roman';
    t.VerticalAlignment = 'middle';
    t.LineStyle = 'none';
    
    N = N+1;
    
    if N == M
        j = j + 1;
        N = 0;
    end
    
end