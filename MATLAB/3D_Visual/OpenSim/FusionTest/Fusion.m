%% Purpose

%   !!!!! not fully working, keep away !!!!!
%   This Programm formats the Sensor-Data to an XSens output. The
%   corresponding values have to be evaluated again within the DataConversion.m before the display of this is possible.
%   Within this process, the Quaternions are calculated again, (quaternion to
%   quaterion) which end in very low and buggy final dataSets. 

% SampleTimeFine
% For the correct dispaly of data, the SampleTimeFine is to be set corresponding to the Hz the Sensor is running on.
% For more information see: https://www.xsens.com/hubfs/Downloads/Manuals/Xsens%20DOT%20User%20Manual.pdf [page 25]
% Hz to Microseconds: hz / (seconds * 1000000 )

%% Clear the Workspace variables. 
clear all; close all; clc;

%% Arduino Init
a = arduino
%% MPU init
%fs = 100;
%imu = mpu9250(a,"SampleRate",fs, 'OutputFormat',"matrix")

imu = mpu9250(a, 'OutputFormat',"matrix")

%% fuse

% GyroscopeNoise and AccelerometerNoise is determined from datasheet.
GyroscopeNoiseMPU9250 = 3.0462e-06; % GyroscopeNoise (variance value) in units of rad/s
AccelerometerNoiseMPU9250 = 0.0061; % AccelerometerNoise(variance value)in units of m/s^2
%viewer = HelperOrientationViewer('Title',{'AHRS Filter'});
FUSE = ahrsfilter('SampleRate',imu.SampleRate, 'GyroscopeNoise',GyroscopeNoiseMPU9250,'AccelerometerNoise',AccelerometerNoiseMPU9250, 'OrientationFormat', 'Rotation matrix');
stopTimer = 100;

dataList = [];
sampleTimeFine = (imu.SampleRate / (1 * 1000000 ));
count = 0;

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


    dataList = [dataList;[count , sampleTimeFine, aX ,aY  ,aZ,Mat11 , Mat21, Mat31 , Mat12 ,Mat22 , Mat32 , Mat13 , Mat23 ,Mat33 ]]

end
disp(dataList)
%%
writematrix(dataList, 'Shooting.txt', 'Delimiter', 'tab');
