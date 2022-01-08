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

%% Orietation Plot
ld = load('rpy_9axis.mat')

accel = ld.sensorData.Acceleration;
gyro = ld.sensorData.AngularVelocity;
Fs  = ld.Fs;
decim = 2;
fuse = imufilter('SampleRate',Fs,'DecimationFactor',decim);

pose = fuse(accel,gyro);

tp = theaterPlot('XLimit',[-2 2],'YLimit',[-2 2],'ZLimit',[-2 2]);
op = orientationPlotter(tp,'DisplayName','Fused Data',...
    'LocalAxesLength',2);

for i=1:numel(pose)
    plotOrientation(op, pose(i))
    drawnow
end

%% second orientation plot

%double acRoll, acPitch, gyRoll, gyPitch, gyYaw, trueRoll, truePitch, trueYaw;
%#define RAD_TO_DEG 57.295779513082320876798154814105
%double eulerOrt(double acX, double acY, double acZ, double gyX, double gyY, double gyZ,  double freq) {

%t = (freq / 1000); //hz is not time but frequenzy
gyX = imu.gyro_x_radps() - gXOFF;
gyY = imu.gyro_y_radps() - gYOFF;
gyZ = imu.gyro_z_radps() - gZOFF;

aX = imu.accel_x_mps2() - aXOFF;
aY = imu.accel_y_mps2() - aYOFF;
aZ = imu.accel_z_mps2() - aZOFF;

aX = aX / 9.81;
aY = aY / 9.81;
aZ = aZ / 9.81;

acRoll = atan(aY / sqrt(pow(aX, 2) + pow(aZ, 2))) * 180 / pi;
acPitch = atan(-1 * aX / sqrt(pow(aY, 2) + pow(aZ, 2))) * 180 / pi;

gyRoll = gyRoll + gyX * 0.5 *  t; % deg/s * t = deg
gyPitch = gyPitch + gyY * 0.5 * t;
gyYaw = gyYaw + gyZ * 0.5 *   t;

% Complementary filter - combine acceleromter and gyro angle values
trueRoll = 0.5 * gyRoll + 0.5 * acRoll;
truePitch = 0.5 * gyPitch + 0.5 * acPitch;
trueYaw = gyYaw;

%#ifdef CSVDEGREE
Serial.print(t);
Serial.print("\t");
Serial.print(trueRoll);
Serial.print("\t");
Serial.print(truePitch);
Serial.print("\t");
Serial.print(trueYaw);
Serial.print("\n");
%#endif

