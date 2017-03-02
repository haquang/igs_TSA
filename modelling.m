close all;
clc;
clear;

%% Initialize

% position
x = 0;
y = 0;

% Springs
k1 = 5;
k2 = 10;
d = 0.1;
M = 0.01;

% External Force
Fext = 0;
dT = 0.001;
T = 2.5;
acc = 0;
vx = 0;
vy = 0;
i = 1;
L = 2;

% Virtual Mass Spring Damper
xm = 0;
vm = 0;
Km = 10.0;
Dm = 1;
Mm = 0.01;
Kp = 10;

% For external force
t1 = 0.5;
t2 = 1.5;
t3 = 1.51;
F = 5;
a1 = (F/t1);
a2 = F/(t2-t3);
b2 = F - a2 * t2;

%DAQ
posx = NaN(T/dT,1);
posy = NaN(T/dT,1);
force = NaN(T/dT,1);
%% Simulation
for t = 0 : dT : T
    %% Ramp function for user force
     if (t < t1)
        Fext = a1 * t;
    else if (t < t2 )
            Fext = F;
        else if ( t < t3)
                Fext = a2 * t + b2;
                %Fext = 0;
            else 
                Fext = 0;
            end
        end
    end
    
    %% Admittance controller
    acc = (Fext - Km * xm  - Dm * vm) / Mm;
    vm = vm + dT * acc;
    xm = xm + dT * acc;
    
    Ftsa = Kp * (y - xm);
    
    %% Physical mass spring
    acc = (Fext - (k1+k2) * x  - d * vx) / M;
    vx = vx + dT * acc;
    x = x + dT * vx;
    
    acc = (Fext - (k1+k2) * y  - d * vy - Ftsa) / M;
    vy = vy + dT * acc;
    y = y + dT * vy;
    
    
    %% Recording
    posx(i) = x;
    posy(i) = y;
    
    force(i) = Fext;
    i = i+1;
end


%% Plot
figure(1);
subplot(2,1,1);
plot(posx);
hold on;
plot(posy,'r');
hold on;
plot(posy-posx,'k');
hold on;
plot(force/30,'--b');
grid on;

subplot(2,1,2);
plot(force);
grid on;
