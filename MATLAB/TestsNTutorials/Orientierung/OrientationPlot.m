%% Arduino Init
a = arduino
%% MPU init
%imu = mpu6050(a)

trueSR = 100;
imu = mpu9250(a,"SampleRate",trueSR)


%% Sensor Fusion Plot Init

Fs  = trueSR %imu.SampleRate; %Hz of running sensor
decim = 2;
%fuse = imufilter('ReferenceFrame','NED');
fuse = imufilter('SampleRate',Fs,'DecimationFactor',decim);

accel=[];
gyro=[];

Fusion = 1;

%% Euler Plot Init
yawZ = 0;
aXERROR = 0;
aYERROR = 0;
aZERROR = 0;

gXERROR =0;
gYERROR = 0;
gZERROR = 0;

tp = theaterPlot('XLimit',[-2 2],'YLimit',[-2 2],'ZLimit',[-2 2]);
op = orientationPlotter(tp,'DisplayName','Fused Data',...
    'LocalAxesLength',2)

Fusion = 0;


%% Main Loop
i = 1;
count = 100;
rounds = 200;

start = ["Calib: ", count, "Rounds: ",rounds, "Mode: ", Fusion]

while i <= (count + rounds)
    imu_read_acc = imu.readAcceleration; % get the sensor values
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

    if i <= count && Fusion == 0
        aXERROR = aXERROR + aX;     %calib sensors
        aYERROR = aYERROR + aY;
        aZERROR = aZERROR + aZ;

        gXERROR = gXERROR + gX;
        gXERROR = gXERROR + gY;
        gXERROR = gXERROR + gZ;

        calib = ["Calibbration Sample ", i , " of ", count]
        disp(calib)

    elseif i == (count+1) && Fusion == 0
        aXERROR = aXERROR / count;
        aYERROR = aYERROR / count;
        aZERROR = aZERROR / count;

        gXERROR = gXERROR / count;
        gXERROR = gXERROR/ count ;
        gXERROR = gXERROR / count;

        debug = ["ERRORS:",aXERROR,aYERROR,aZERROR,gXERROR,gYERROR,gZERROR] 
        disp(debug) %display Error values
        

    elseif i > count

        if Fusion == 0
            %------------------------------------eulerPlot
            t = trueSR / 1000;
            yawZ = yawZ + (gZ * 180/pi)*t;

            eulerPlot(op, aX,aY,aZ, yawZ);

        elseif Fusion == 1
            %-----------------------------------SensorFusionPlotCapture
            accel = [accel;[aX,aY,aZ]];
            gyro = [gyro;[gX,gY,gZ]];
        end
    end
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


function eulerPlot = eulerPlot(opO,aXO,aYO,aZO, gZO)

aXO = aXO / 9, 81;
aYO = aYO / 9, 81;
aZO = aZO / 9, 81;

rollX = atan(aYO / sqrt(aXO^2 + aZO^2)) * 180 / pi;
pitchY = atan(-1 * aXO / sqrt(aYO^2 + aZO^2)) * 180 / pi;
yawZO = gZO;
%yawZO = arcsin(aZO);

plotOrientation(opO, rollX, pitchY, yawZO);
drawnow

%return yawZO;

end

