theta =-[-2;-3;-4;-5;-6;-7;-8;-9;-10;-11;-12];
force =-[-0.5;-0.7;-0.98;-1.08;-1.1619;-1.965;-2.86;-3.341;-4.362;-4.613;-6.129] ;

P = polyfit(theta,force,2)

force_fit = P(1) * theta.^2 + P(2) * theta + P(3);

figure();
scatter(theta,force);
hold on;
plot(theta,force_fit);
