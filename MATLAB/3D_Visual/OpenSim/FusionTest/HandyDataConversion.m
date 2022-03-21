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


%% Init tables
% these are data of the humerus movement
accl = readmatrix('HandyIMU\5598_Accl.txt');
msg = 'read accl fin'
gyro = readmatrix('HandyIMU\5598_Gyro.txt');
msg = 'read gyro fin'
mag = readmatrix('HandyIMU\5598_mag.txt');
msg = 'read mag fin'



%% fuse
% Mat1, Mat2, Mat3, Mat4 are temporary variables which are often
% overwritten, there should be no passing-on-value problem with them

% GyroscopeNoise and AccelerometerNoise is determined from datasheet.
GyroscopeNoiseMPU9250 = 3.0462e-06; % GyroscopeNoise (variance value) in units of rad/s
AccelerometerNoiseMPU9250 = 0.0061; % AccelerometerNoise(variance value)in units of m/s^2
sampleRate = 100; % Sample Rate while the data was taken (IMU)
FUSE = ahrsfilter('SampleRate',sampleRate, 'GyroscopeNoise',GyroscopeNoiseMPU9250,'AccelerometerNoise',AccelerometerNoiseMPU9250, 'OrientationFormat', 'quaternion');
count = 1;
timeperiod = 0.01;
dataList = [];

msg = ['Taking data for: ', num2str(height(accl)), ' sets']

%%
while(count < height(accl))
    % get data

    multiply = [-1,1,-1];
 
    imu_read_acc = accl(count,:).* multiply;
    imu_read_gyro = gyro(count,:);
    imu_read_mag = mag(count,:);

    %fuse and evaluate data for final file
    rotators = FUSE(imu_read_acc,imu_read_gyro,imu_read_mag);

    [Mat1, Mat2, Mat3, Mat4] = parts(rotators);

    quats = [Mat1, Mat2, Mat3, Mat4];

    dataList = [dataList;[(timeperiod*count), quats ]];

    count = count + 1;

end
%% Print file
writematrix(dataList, 'IMUData\ShootingQuat_ZugHumerus.txt', 'Delimiter', 'tab');
msg = ["Printed file"]
