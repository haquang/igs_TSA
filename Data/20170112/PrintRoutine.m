function PrintRoutine( DPI ,filename)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Function ‘PrintRoutine’ is used to print the current figure file to both
% a high resolution bitmap-based file in Portable Network Graphic (*.png)
% format and a vector-based file in Encapuslated Postscript (*.eps) format.
% Typically, bitmap files are used for WYSIWYG software for word processing
% and presentations (i.e., Microsoft Word and Powerpoint). Vector files are
% commonly used with LaTeX processing. This script prints the current
% figure to both a *.png and a *.eps file whose name is specified when the
% function is called. The output file is sized according to the paper size
% of the figure box and its resolution is either 600 DPI (default) or specified
% via the single input.
% 
% Function Inputs
% DPI: desired figure resolution in ‘dots per inch’
% 
% Defaults
% If unspecified, the output resolution of the files are set to 600
% DPI
%
% This file is intended to be used with the ‘FigureSetup’ function.
% FigureSetup specifies the size of a figure box in MATLAB and is used to
% scale figures such that they can be imported into files without resizing.
% This ensures the scaling of the figure remains constant. If this function
% is used, it automatically resizes both the figure box and the paper to
% match the desired input size. Thus, figures printed using this file do
% not need to be cropped (for *.png files) or do not need external bounding
% boxes added (for *.eps files).
% 
% Written by: Jonathan D. Ellis, PhD
% Last Updated: August 28, 2015
% Version: 1.00
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 
% Establish the number of input arguments
M = nargin;
 
% Switch to 600 DPI default output resolution if no input is specified
switch M
case {0}
DPI = 600;
end
 
% Verifies whether you want to generate prints for the current figure. This
% is useful and needed for embedding this equation is nested functions
% where multiple figures might be printed. 
% 
% Answer with either ‘y’ or ‘n’
arg2 = input('Do you wish to print PNG & EPS file? (y/n) ','s');
arg2 = lower(arg2);
 
% If ‘y’ is selected, then the figure will be printed to file.
switch arg2
case {'y'}
% Input the file name. No extension is needed in the file name as
% it is appended by the ‘print’ command.
print_file = filename;
print_file_png = [print_file,'.png'];
print_file_eps = [print_file,'.eps'];
print_file_tiff = [print_file,'.tiff'];
print(print_file_png,['-r',num2str(DPI)],'-dpng','-loose','-noui');
print(print_file_eps,['-r',num2str(DPI)],'-depsc','-tiff','-loose','-noui');
% print(print_file_tiff,[‘-r’,num2str(DPI)],’-dtiff’,’-loose’,’-noui’)
case {n}
display(' ')
display('Figure Not Printed')
display(' ')
otherwise
display('Selected a wrong option. Figure Not Printed')
display(' ')
end
 
end