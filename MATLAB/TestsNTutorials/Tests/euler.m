%% Arduino Init
a = arduino
%% MPU init
%imu = mpu6050(a)
imu = mpu9250(a)
%%
acc_ort = []
%testComment
i = 1;
while i < 200
imu_read = imu.readAcceleration;

aX = read(1);
aY = read(2);
aZ = read(3);

gX = read(4);
gY = read(5);
gZ = read(6);



%{
%euler winkel
  aX = aX / 9, 81;
  aY = aY / 9, 81;
  aZ = aZ / 9, 81;

  roll = atan(aY / sqrt(aX^2 + aZ^2)) * 180 / pi;
  pitch = atan(-1 * aX / sqrt(aY^2 + aZ^2)) * 180 / pi;

acc_ort = [acc_ort; [i,roll, pitch]]
%}

% 
%Orientation plot
accel = [accel; [aX,aY,aZ]]; %SensorData - accel
gyro = [gyro; [gX,gY,gZ]]; %SensorData - Gyro

i = i+1;
end

acc_ort_table = array2table(acc_ort, "variableNames",{'time', 'pitch', 'roll' })

%acc_table = array2table(acc_data, 'variableNames', {'Time(0.1s)', 'acc x', 'acc y', 'acc x'})

%%Orientation Plot from stored Data
disp(acc_ort_table)


Fs = 50; %SampleRate in Hz
decim = 1; %https://www.mathworks.com/help/fusion/ref/imufilter-system-object.html - me dont understand fully
fuse = imufilter('SampleRate', Fs, 'DecimationFactor', decim); %create the above mentioned IMUFilter

pose = fuse(accel, gyro); %Sensor Fusion for orientation

tp = theaterPlot('XLimit',[-2 2],'YLimit',[-2 2],'ZLimit',[-2 2]);
op = orientationPlotter(tp,'DisplayName','Fused Data',...
    'LocalAxesLength',2);

for i=1:numel(pose)
    plotOrientation(op, pose(i))
    drawnow
end