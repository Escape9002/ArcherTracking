%% Arduino Init
a = arduino
%% MPU init
%imu = mpu6050(a)
imu = mpu9250(a)

%% Sensor Fusion Plot Init

Fs  = 200%imu.SampleRate; %Hz of running sensor
decim = 2;
%fuse = imufilter('ReferenceFrame','NED');
fuse = imufilter('SampleRate',Fs,'DecimationFactor',decim);

accel=[];
gyro=[];

Fusion = 1;

%% Euler Plot Init

tp = theaterPlot('XLimit',[-2 2],'YLimit',[-2 2],'ZLimit',[-2 2]);
op = orientationPlotter(tp,'DisplayName','Fused Data',...
    'LocalAxesLength',2)

Fusion = 0;

%% Main Loop
i = 1;
while i <= 200
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

    if Fusion == 0
    %------------------------------------eulerPlot
    eulerPlot(op,aX,aY,aZ)
    elseif Fusion == 1
    %-----------------------------------SensorFusionPlotCapture
    accel = [accel;[aX,aY,aZ]];
    gyro = [gyro;[gX,gY,gZ]];
    end
    %---------------------------------------

    disp(i)
    i = i+1;
end


%% Sensor Fusion Plot Function

pose = fuse(accel,gyro);

tp = theaterPlot('XLimit',[-2 2],'YLimit',[-2 2],'ZLimit',[-2 2]);
op = orientationPlotter(tp,'DisplayName','Fused Data',...
    'LocalAxesLength',2);

for i=1:numel(pose)
    plotOrientation(op, pose(i))
    drawnow
end

%% EulerPlot Function
function eulerPlot = eulerPlot(opO,aXO,aYO,aZO)

aXO = aXO / 9, 81;
aYO = aYO / 9, 81;
aZO = aZO / 9, 81;

rollX = atan(aYO / sqrt(aXO^2 + aZO^2)) * 180 / pi;
pitchX = atan(-1 * aXO / sqrt(aYO^2 + aZO^2)) * 180 / pi;

plotOrientation(opO, rollX, pitchX,0)
drawnow
end

