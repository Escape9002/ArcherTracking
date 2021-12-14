%% sources
%https://www.youtube.com/watch?v=0Oz1YxQ42X0&list=TLPQMTExMjIwMjFUCOHtJI9Gjw&index=1

%example with arduino sense : openExample('arduinoio/MeasureLSM9DS1FromI2CDeviceOnArduinoHardwareExample')


%% init arduino
a = arduino


%% init mpu 
% Objective 2 Carful, you need the sensor fusion package + update the arduino with it
%imu = lsm9ds1(a)
imu = mpu6050(a)
%imu = mpu9250(a,'SampleRate',50,'SamplesPerRead',5,'ReadMode','Latest') 
%addr = scanI2CBus(a,0)

%imu = mpu9250(a,'I2CAddress',{0x68, 0x0C})
%imu = mpu9250(a)

%% calibrate
status = readCalibrationStatus(imu)

%% capture imu data format: xyz for all sensor except for last one: zyx

imu_read = read(imu);
imu_matrix = imu_read{:,:};
imu_mean = mean(imu_matrix);

imu_table = array2table(imu_mean, 'VariableNames',{'Acc x', 'Acc y','Acc z', 'gyro x', 'gyro y','gyro z'})

% accel
acc_x_ms = imu_mean(:,1);
acc_y_ms = imu_mean(:,2);
acc_z_ms = imu_mean(:,3);

acc_x_g = acc_x_ms / 9.81;
acc_y_g = acc_y_ms / 9.81;
acc_z_g = acc_z_ms / 9.81;

% gyro
gyro_x_rad = imu_mean(:,4);
gyro_y_rad = imu_mean(:,5);
gyro_z_rad = imu_mean(:,6);

gyro_x_deg = (gyro_x_rad) * (180*3.14159);
gyro_y_deg = (gyro_y_rad) * (180*3.14159);
gyro_z_deg = (gyro_z_rad) * (180*3.14159);

x = acc_x_ms;
y = acc_y_ms;
z = acc_y_ms;

figure
hold on
plot3(x,y,z);
xlabel('x');
ylabel('y');
zlabel('z');
grid on
axis('equal')

hold off

%orientation_matrix = [ort_x_deg, ort_y_deg, ort_z_deg];

%disp(orientation_matrix);