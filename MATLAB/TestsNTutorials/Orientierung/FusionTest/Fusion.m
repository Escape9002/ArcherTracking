%% Arduino Init
a = arduino
%% MPU init
%imu = mpu6050(a)

fs = 100;
imu = mpu9250(a,"SampleRate",fs, 'OutputFormat',"matrix")



%% Sensor Fusion Plot Function
%% Calib
%displayMessage(['Fusion algorithms use magnetometer readings which need to compensate for magnetic distortions.' ...
%    'The given code snippet can be used to find the correction values for compensating Hard Iron Distortions. When the code '...
%    'is executing, rotate the sensor around x axis from 0 to 360 degree. For other axes, modify the code accordingly and rotate the ' ...
%    'sensor along that axis'],'Compensating Hard Iron Distortions');
tic;
stopTimer = 100;
magReadings=[];
while(toc<stopTimer)
    % Rotate the sensor around x axis from 0 to 360 degree.
    % Take 2-3 rotations to improve accuracy.
    % For other axes, rotate around that axis.
    [accel,gyro,mag] = read(imu);
    magReadings = [magReadings;mag];
end

% For y axis, use magReadings (:,2) and for z axis use magReadings(:,3)
magx_min = min(magReadings(:,1));
magx_max = max(magReadings(:,1));
magx_correction = (magx_max+magx_min)/2;


%% fuse

% GyroscopeNoise and AccelerometerNoise is determined from datasheet.
GyroscopeNoiseMPU9250 = 3.0462e-06; % GyroscopeNoise (variance value) in units of rad/s
AccelerometerNoiseMPU9250 = 0.0061; % AccelerometerNoise(variance value)in units of m/s^2
%viewer = HelperOrientationViewer('Title',{'AHRS Filter'});
FUSE = ahrsfilter('SampleRate',imu.SampleRate, 'GyroscopeNoise',GyroscopeNoiseMPU9250,'AccelerometerNoise',AccelerometerNoiseMPU9250, 'OrientationFormat', 'Rotation matrix');
stopTimer = 50;

dataList = [];
sampleTimeFine = 9.141516;
count = 0;


years = 0;
month= 0;
days= 0;
sec= 0;
utc= 0;
utcyear= 0;
utcmonth= 0;
utcd= 0;
utch= 0;
utcmin= 0;
utcsec= 0;
utcval= 0;

% visualize
% Use ahrsfilter to estimate orientation and update the viewer as the
% sensor moves for time specified by stopTimer
%displayMessage(['This section uses AHRS filter to determine orientation of the sensor by collecting live sensor data from the \slmpu9250 \rm' ...
%    'system object. Move the sensor to visualize orientation of the sensor in the figure window. Keep the sensor stationery before you' ...
%    'click OK'],...
%    'Estimate Orientation using AHRS filter and MPU-9250')
tic;
while(toc < stopTimer)

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

    % [accel,gyro,mag] = readSensorDataMPU9250(imu);
    rotators = FUSE(imu_read_acc,imu_read_gyro,imu_read_mag);

    Mat11 = rotators([1],[1]);
    Mat12 = rotators([1],[2]);
    Mat13 = rotators([1],[3]);

    Mat21 = rotators([2],[1]);
    Mat22 = rotators([2],[2]);
    Mat23 = rotators([2],[3]);

    Mat31 = rotators([3],[1]);
    Mat32 = rotators([3],[2]);
    Mat33 = rotators([3],[3]);

    count = count + 1;


    dataList = [dataList;[count , sampleTimeFine ,years,month,days,sec,utc,utcyear,utcmonth,utcd,utch,utcmin,utcsec,utcval, aX ,aY  ,aZ,Mat11 , Mat21, Mat31 , Mat12 ,Mat22 , Mat32 , Mat13 , Mat23 ,Mat33 ]]




end
disp(dataList)
%%
writematrix(dataList, 'Test.csv', 'Delimiter', 'tab');