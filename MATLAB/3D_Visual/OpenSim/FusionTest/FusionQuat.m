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

single = true;

%% Arduino Init
if single == true
    a = arduino
elseif single == true
    a1 = arduino('COM8')
    a2 = arduino('COM10')
end

%% MPU init
%imu = mpu6050(a)
if single == true
    imu = mpu9250(a, 'OutputFormat',"matrix")
elseif single == false
    imu1 = mpu9250(a1,'OutputFormat','matrix')
    imu2 = mpu9250(a2,'OutputFormat','matrix')
end
%% fuse
% Mat1, Mat2, Mat3, Mat4 are temporary variables which are often
% overwritten, there should be no passing-on-value problem with them
stopTimer = 2000;
count = 1;

if single == true
    dataList = [];

    timeperiod = 1 / imu.SampleRate;

    % GyroscopeNoise and AccelerometerNoise is determined from datasheet.
    GyroscopeNoiseMPU9250 = 3.0462e-06; % GyroscopeNoise (variance value) in units of rad/s
    AccelerometerNoiseMPU9250 = 0.0061; % AccelerometerNoise(variance value)in units of m/s^2
    FUSE = ahrsfilter('SampleRate',imu.SampleRate, 'GyroscopeNoise',GyroscopeNoiseMPU9250,'AccelerometerNoise',AccelerometerNoiseMPU9250, 'OrientationFormat', 'quaternion');

elseif single == false
    dataList1 = [];
    dataList2 = [];
    % GyroscopeNoise and AccelerometerNoise is determined from datasheet.
    GyroscopeNoiseMPU9250 = 3.0462e-06; % GyroscopeNoise (variance value) in units of rad/s
    AccelerometerNoiseMPU9250 = 0.0061; % AccelerometerNoise(variance value)in units of m/s^2
    FUSE1 = ahrsfilter('SampleRate',imu1.SampleRate, 'GyroscopeNoise',GyroscopeNoiseMPU9250,'AccelerometerNoise',AccelerometerNoiseMPU9250, 'OrientationFormat', 'quaternion');

    % GyroscopeNoise and AccelerometerNoise is determined from datasheet.
    GyroscopeNoiseMPU9250 = 3.0462e-06; % GyroscopeNoise (variance value) in units of rad/s
    AccelerometerNoiseMPU9250 = 0.0061; % AccelerometerNoise(variance value)in units of m/s^2
    FUSE1 = ahrsfilter('SampleRate',imu2.SampleRate, 'GyroscopeNoise',GyroscopeNoiseMPU9250,'AccelerometerNoise',AccelerometerNoiseMPU9250, 'OrientationFormat', 'quaternion');

    timeperiod1 = 1/imu1.SampleRate;
    timeperiod2 = 1/ imu2.SampleRate;
    timeperiod = (timeperiod1+timeperiod2) /2;
end


msg = ['Taking data for: ', num2str((timeperiod * stopTimer)), ' seconds']


while((count < stopTimer) && (single == true))
    % get data
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

    %fuse and evaluate data for final file
    rotators = FUSE(imu_read_acc,imu_read_gyro,imu_read_mag);

    [Mat1, Mat2, Mat3, Mat4] = parts(rotators);

    quats = [Mat1, Mat2, Mat3, Mat4];

    dataList = [dataList;[(timeperiod*count), quats ]];

    count = count + 1;

end

while((count < stopTimer) && (single == false))
    %---------------------------------------------------------------------------------
    % get data of sensor 1
    imu_read_acc1 = imu1.readAcceleration;
    imu_read_gyro1 = imu1.readAngularVelocity;
    imu_read_mag1 = imu1.readMagneticField;

    aX1 = imu_read_acc1(1);
    aY1 = imu_read_acc1(2);
    aZ1 = imu_read_acc1(3);

    gX1 = imu_read_gyro1(1);
    gY1 = imu_read_gyro1(2);
    gZ1 = imu_read_gyro1(3);

    magX1 = imu_read_mag1(1);
    magY1 = imu_read_mag1(2);
    magZ1 = imu_read_mag1(3);

    %fuse and evaluate data for final file
    rotators1 = FUSE1(imu_read_acc1,imu_read_gyro1,imu_read_mag1);

    [Mat1, Mat2, Mat3, Mat4] = parts(rotators1);

    quats1 = [Mat1, Mat2, Mat3, Mat4];

    dataList1 = [dataList1;[(timeperiod*count), quats1 ]];
    %---------------------------------------------------------------------------------
    % get data of sensor 2
    imu_read_acc2 = imu2.readAcceleration;
    imu_read_gyro2 = imu2.readAngularVelocity;
    imu_read_mag2 = imu2.readMagneticField;

    aX2 = imu_read_acc2(1);
    aY2 = imu_read_acc2(2);
    aZ2 = imu_read_acc2(3);

    gX2 = imu_read_gyro2(1);
    gY2 = imu_read_gyro2(2);
    gZ2 = imu_read_gyro2(3);

    magX2 = imu_read_mag2(1);
    magY2 = imu_read_mag2(2);
    magZ2 = imu_read_mag2(3);

    %fuse and evaluate data for final file
    rotators2 = FUSE2(imu_read_acc2,imu_read_gyro2,imu_read_mag2);

    [Mat1, Mat2, Mat3, Mat4] = parts(rotators1);

    quats2 = [Mat1, Mat2, Mat3, Mat4];

    dataList2 = [dataList2;[(timeperiod*count), quats2 ]];
    %---------------------------------------------------------------------------------

    count = count + 1;

end

if single ==  true

    disp(dataList)

elseif single == false
    lists = [dataList1, dataList2];
    disp(list)
end
%% Print file
if single == true
    writematrix(dataList, 'IMUData\ShootingQuat_TEST.txt', 'Delimiter', 'tab');
elseif single == false
    writematrix(dataList1, 'IMUData\ShootingQuatCOM8.txt', 'Delimiter', 'tab');
    writematrix(dataList2, 'IMUData\ShootingQuatCOM10.txt', 'Delimiter', 'tab');
end