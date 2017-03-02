%% Init
close all;
clear;
%% 
L = 40.75;
enc = [2000;4000;6000;8000;10000;12000;14000;15000;18000];
x = [40.32;39.87;39.04;37.74;35.74;33.78;31.99;30.94;28.07];

for i = 1: length(x)
    x(i) = L - x(i);
end

% 

P = polyfit(enc,x,2)
K = x\enc;
x_fit = P(1) * enc.^2 + P(2) * enc + P(3);

ENCODER_RESOLUTION = 1024;
r0 = 0.25;
for i = 1: length(enc)
   theta = enc(i)/ ENCODER_RESOLUTION * 2 * pi;
   X = sqrt(L*L - theta * theta * r0 * r0);
   r = r0 *sqrt(L/X);   
   x_tsa(i) =  L - sqrt(L*L - theta * theta * r * r);
end


% Plot
figure();
scatter(enc,x);
hold on;
plot(enc,x_fit);
hold on
plot(enc,x_tsa);