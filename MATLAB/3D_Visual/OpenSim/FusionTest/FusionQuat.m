%% Purpose
%   The data of the MPU9250 are catched, fused and formatted to be used
%   with the dataConversion.m of this folder.
%   1 or 2 Sensors can be looked at.
%{
Dataformat of the produced .txt:
    0.01	0.335519811154871	0.705822462542093	0.397070637879537	-0.481223457686294
    0.02	0.336752341057119	0.706154846931129	0.396921565385659	-0.479996316527556
    0.03	0.338450166545752	0.706274387214777	0.396256018006081	-0.479175482389812
%}
%% Clear the Workspace variables.
clear all; close all; clc;


%% Arduino Init

a = arduino()


%% MPU init
%imu = mpu6050(a)
imu = mpu9250(a, 'OutputFormat',"matrix")

%% fuse
% Mat1, Mat2, Mat3, Mat4 are temporary variables which are often
% overwritten, there should be no passing-on-value problem with them
stopTimer = 2000;
count = 1;

dataList = [];

timeperiod = 1 / imu.SampleRate;

% GyroscopeNoise and AccelerometerNoise is determined from datasheet.
GyroscopeNoiseMPU9250 = 3.0462e-06; % GyroscopeNoise (variance value) in units of rad/s
AccelerometerNoiseMPU9250 = 0.0061; % AccelerometerNoise(variance value)in units of m/s^2
FUSE = ahrsfilter('SampleRate',imu.SampleRate, 'GyroscopeNoise',GyroscopeNoiseMPU9250,'AccelerometerNoise',AccelerometerNoiseMPU9250, 'OrientationFormat', 'quaternion');


msg = ['Taking data for: ', num2str((timeperiod * stopTimer)), ' seconds']


while(count < stopTimer)
    % get data
    imu_read_acc = imu.readAcceleration;
    imu_read_gyro = imu.readAngularVelocity;
    imu_read_mag = imu.readMagneticField;

    %aX = imu_read_acc(1);
    %aY = imu_read_acc(2);
    %aZ = imu_read_acc(3);

    %gX = imu_read_gyro(1);
    %gY = imu_read_gyro(2);
    %gZ = imu_read_gyro(3);

    %magX = imu_read_mag(1);
    %magY = imu_read_mag(2);
    %magZ = imu_read_mag(3);

    %fuse and evaluate data for final file
    rotators = FUSE(imu_read_acc,imu_read_gyro,imu_read_mag);

    [Mat1, Mat2, Mat3, Mat4] = parts(rotators);

    quats = [Mat1, Mat2, Mat3, Mat4];

    dataList = [dataList;[(timeperiod*count), quats ]];

    count = count + 1

end

disp(dataList)
%% Print file

writematrix(dataList, 'IMUData\ShootingQuat_facingForward.txt', 'Delimiter', 'tab');
