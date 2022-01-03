%% Arduino Init
a = arduino
%% MPU init
imu = mpu6050(a)

%%
a = -pi : pi/2 : pi;                                % Define Corners of cube
ph = roll; %default: pi/4                                          % Define Angular Orientation ( Phase%)
x = [cos(a+ph); cos(a+ph)]/cos(ph);
y = [sin(a+ph); sin(a+ph)]/sin(ph);
z = [-ones(size(a)); ones(size(a))];

figure
surf(x, y, z, 'FaceColor','g')                      % Plot Cube
hold on
patch(x', y', z', 'r')                              % Make Cube Appear Solid
hold off
axis([ -1  1    -1  1    -1  1]*1.5)
grid on


acc_ort = []

i = 1;
while i < 10000
imu_read = imu.readAcceleration;

aX = imu_read(1);
aY = imu_read(2);
aZ = imu_read(3);



%euler winkel

  aX = aX / 9, 81;
  aY = aY / 9, 81;
  aZ = aZ / 9, 81;

  roll = atan(aY / sqrt(aX^2 + aZ^2)) * 180 / pi;
  pitch = atan(-1 * aX / sqrt(aY^2 + aZ^2)) * 180 / pi;

acc_ort = [acc_ort; [i,roll, pitch]]

rotate()


i = i+1;
end

acc_ort_table = array2table(acc_ort, "variableNames",{'time', 'pitch', 'roll' })

%acc_table = array2table(acc_data, 'variableNames', {'Time(0.1s)', 'acc x', 'acc y', 'acc x'})


disp(acc_ort_table)

%% simple cube
a = -pi : pi/2 : pi;                                % Define Corners
ph = pi/4; %default: pi/4                                          % Define Angular Orientation ( Phase%)
x = [cos(a+ph); cos(a+ph)]/cos(ph);
y = [sin(a+ph); sin(a+ph)]/sin(ph);
z = [-ones(size(a)); ones(size(a))];

figure
surf(x, y, z, 'FaceColor','g')                      % Plot Cube
hold on
patch(x', y', z', 'r')                              % Make Cube Appear Solid
hold off
axis([ -1  1    -1  1    -1  1]*1.5)
grid on