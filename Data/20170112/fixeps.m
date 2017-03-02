function fixeps(inname,outname,fixmode)
if nargin==2
    fixmode = 'LJ';
end
fi = fopen(inname,'r');
fo = fopen(outname,'w');
tline = fgets(fi);
while ischar(tline)
    if (strcmp(tline,['10.0 ML' 10])) % Replace 10.0 miterlimit
        switch (fixmode)
            case 'LJ'
                fwrite(fo,['1 LJ' 10]); % With round linejoin
            case 'ML'
                fwrite(fo,['2.5 ML' 10]); % With smaller miterlimit
        end
    else
        fwrite(fo,tline);
    end
    tline = fgets(fi);
end
fclose(fo);
fclose(fi);