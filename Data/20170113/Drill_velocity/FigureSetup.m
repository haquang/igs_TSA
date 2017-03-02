function FigureSetup(FigNum,x_cm,y_cm)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Function 'FigureSetup':
%
% Establishes a Figure custom sized using units of centimeters for the
% width and height. This function is useful for generating figure boxes of
% custom size for publications. Note: figures may look distorted on the
% screen depending on your monitor and screen resolution. This function is
% intended for publications where the exact size of the figure is needed.
% This is meant to be used with the fuction 'PrintRoutine' to print the
% figure to a PNG file and an eps file.
%
% Typical Two-Column journal articles have a width of 8 cm to 9 cm. The
% height of the figure can vary based on the desired information.
% 
% Input Variables:
% 
%         FigNum: a figure created with this figure number.
%         x_cm: width of the figure in units of centimeters. 
%         y_cm: height of the figure in units of centimeters.
% 
% System Defaults: If no figure number (FigNum) is specified, then  a new
% figure will be created. If no X and Y dimensions are specified, then the
% figure size will default to x_cm = 8.3 and y_cm = 4.5 to create a figure
% that is 8.3 cm by 4.5 cm high.
% 
% Written by Jonathan D. Ellis, PhD
% Updated August 26, 2015
% Version: 1.00
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


M = nargin;

% Determine the number of input arguments
switch M
    case {0}
        figure;
        x_cm = 8.3;
        y_cm = 4.5;
    case {1}
        figure(FigNum)
        x_cm = 8.3;
        y_cm = 4.5;
    case {2}
        figure;
        X = FigNum;
        Y = x_cm;
        x_cm = X;
        y_cm = Y;
    otherwise
        figure(FigNum)
end

% initially position the figure in the center of the screen
set(gcf,'Units','normalized','Position',[.33 .33 .33 .33])

% Set Figure units and orientation
set(gcf,'PaperUnits','Centimeters',...
    'Units','Centimeters',...
    'PaperOrientation','Portrait',...
    'PaperPositionMode','auto');

% Get the correctly scaled position vector (1x4) in units of centimeters.
% The figure should be placed at the 1/3rd - 1/3rd edge of the screen from
% the lower left corner
[A] = get(gcf,'Position');

% Keep the same starting location for the figure box but now resize it to
% the input sizes
set(gcf,'Position',[A(1) A(2) x_cm y_cm]);

% When printing the figure using the print command, it will print the
% figure to the default paper size (lettersize or A4). This part of the
% code resizes the paper of the figure such that it is the sam size as the
% figure. Now, when you print the figure to a file using the 'print'
% command, it will be automatically cropped. For *.eps figures, this will
% create a bounding box at the edge of the figure.
PAPER = get(gcf,'Position');
set(gcf,'PaperSize',[PAPER(3) PAPER(4)])

end