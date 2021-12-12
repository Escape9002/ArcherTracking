%%https://www.youtube.com/watch?v=OUwfE_-tcfo

clear
clc
close all

%x = [1 2 5];
%y = [2 3 4];
%z = [1 3 0];
t = linspace(0, 6*pi, 30);
x = 3* cos(t);
y = 1*sin(t);
z = 0.01 * t.^2;

figure 
plot3(x,y,z);
xlabel('x');
ylabel('y');
zlabel('z');
grid on
axis('equal')

figure scatter3(x,y,z
