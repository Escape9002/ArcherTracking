%% Arduino Init
a = arduino
%% MPU init
%imu = mpu6050(a)
imu = mpu9250(a)

%% orientation Plot
Fs  = 200%imu.SampleRate; %Hz of running sensor
decim = 2; 
%fuse = imufilter('ReferenceFrame','NED');
fuse = imufilter('SampleRate',Fs,'DecimationFactor',decim);

%% Euler plot
acc_ort = []

tp = theaterPlot('XLimit',[-2 2],'YLimit',[-2 2],'ZLimit',[-2 2]);
op = orientationPlotter(tp,'DisplayName','Fused Data',...
    'LocalAxesLength',2)

%%
accel=[];
gyro=[];
mag = [];

i = 1;
while i <= 1600
imu_read_acc = imu.readAcceleration;
imu_read_gyro = imu.readAngularVelocity;
imu_read_mag = imu.readMagneticField;

aX = imu_read_acc(1);
aY = imu_read_acc(2);
aZ = imu_read_acc(3);

gX = imu_read_gyro(1);
gY = imu_read_gyro(2);
gZ = imu_read_gyro(3);

magX = imu_read_mag(1);
magY = imu_read_mag(2);
magZ = imu_read_mag(3);

%Orientation Plot
accel = [accel;[aX,aY,aZ]];
gyro = [gyro;[gX,gY,gZ]]; 
mag = [mag;[magX,magY,magZ]]; 


%euler winkel

  aX = aX / 9, 81;
  aY = aY / 9, 81;
  aZ = aZ / 9, 81;

  roll = atan(aY / sqrt(aX^2 + aZ^2)) * 180 / pi;
  pitch = atan(-1 * aX / sqrt(aY^2 + aZ^2)) * 180 / pi;

acc_ort = [acc_ort; [i,roll, pitch]]

  plotOrientation(op, roll, pitch,0)
    drawnow




disp(i)
i = i+1;
end

%acc_ort_table = array2table(acc_ort, "variableNames",{'time', 'pitch', 'roll' })

%acc_table = array2table(acc_data, 'variableNames', {'Time(0.1s)', 'acc x', 'acc y', 'acc x'})

%OrientationPlot
%disp(acc_ort_table)

%% Run Plot
pose = fuse(accel,gyro);

tp = theaterPlot('XLimit',[-2 2],'YLimit',[-2 2],'ZLimit',[-2 2]);
op = orientationPlotter(tp,'DisplayName','Fused Data',...
    'LocalAxesLength',2);

for i=1:numel(pose)
    plotOrientation(op, pose(i))
    drawnow
end

%% run Euler Plot
tp = theaterPlot('XLimit',[-2 2],'YLimit',[-2 2],'ZLimit',[-2 2]);
op = orientationPlotter(tp,'DisplayName','Fused Data',...
    'LocalAxesLength',2);

for i=1:numel(eulerList)
    plotOrientation(op, eulerList(1))
    drawnow
end
