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

a1 = arduino('COM4')    % sense
a2 = arduino('COM6')    % not sense


%% MPU init
%imu = mpu6050(a)
imu1 = mpu9250(a1, 'OutputFormat',"matrix")
imu2 = mpu9250(a2, 'OutputFormat', "matrix")

%% fuse
% Mat1, Mat2, Mat3, Mat4 are temporary variables which are often
% overwritten, there should be no passing-on-value problem with them
stopTimer = 2000;
count = 1;

%dataList1 = [];
dataList1 =zeros(stopTimer, 3); 

%dataList2 = [];
dataList2 = zeros(stopTimer);

timeperiod = 1 / imu1.SampleRate;

% GyroscopeNoise and AccelerometerNoise is determined from datasheet.
GyroscopeNoiseMPU9250 = 3.0462e-06; % GyroscopeNoise (variance value) in units of rad/s
AccelerometerNoiseMPU9250 = 0.0061; % AccelerometerNoise(variance value)in units of m/s^2

FUSE1 = ahrsfilter('SampleRate',imu1.SampleRate, 'GyroscopeNoise',GyroscopeNoiseMPU9250,'AccelerometerNoise',AccelerometerNoiseMPU9250, 'OrientationFormat', 'quaternion');
FUSE2 = ahrsfilter('SampleRate',imu2.SampleRate, 'GyroscopeNoise',GyroscopeNoiseMPU9250,'AccelerometerNoise',AccelerometerNoiseMPU9250, 'OrientationFormat', 'quaternion');


msg = ['Taking data for: ', num2str((timeperiod * stopTimer)), ' seconds']


while(count < stopTimer)
    % get data
    imu_read_acc1 = imu1.readAcceleration;
    imu_read_gyro1 = imu1.readAngularVelocity;
    imu_read_mag1 = imu1.readMagneticField;

    imu_read_acc2 = imu2.readAcceleration;
    imu_read_gyro2 = imu2.readAngularVelocity;
    imu_read_mag2 = imu2.readMagneticField;

    %fuse and evaluate data for final file
    rotators1 = FUSE1(imu_read_acc1,imu_read_gyro1,imu_read_mag1);
    rotators2 = FUSE2(imu_read_acc2,imu_read_gyro2,imu_read_mag2);

    [Mat1, Mat2, Mat3, Mat4] = parts(rotators1);
    quats = [Mat1, Mat2, Mat3, Mat4];

    dataList1 = [dataList1;[(timeperiod*count), quats ]];

    [Mat1, Mat2, Mat3, Mat4] = parts(rotators2);
    quats = [Mat1, Mat2, Mat3, Mat4];
    
    dataList2 = [dataList2;[(timeperiod*count), quats ]];

    count = count + 1

end
msg = [dataList1, " ", dataList2]

%% Print file

writematrix(dataList1, 'IMUData\ShootingQuat_TEST1.txt', 'Delimiter', 'tab');
writematrix(dataList2, 'IMUData\ShootingQuat_TEST2.txt', 'Delimiter', 'tab');