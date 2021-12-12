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

%imu = mpu9250(a,'I2CAddress',{0x69, 0x0C})


%% calibrate
status = readCalibrationStatus(imu)

%% capture imu data format: xyz for all sensor except for last one: zyx

imu_read = read(imu);
imu_matrix = imu_read{:,:};
imu_mean = mean(imu_matrix);

imu_table = array2table(imu_mean, 'VariableNames',{'Acc x', 'Acc y','Acc z', 'gyro x', 'gyro y','gyro z'})

%ort_x_rad = imu_mean(:,12);
%ort_y_read = imu_mean(:,11);
%ort_z_read = imu_mean(:,10);

%ort_x_deg = (ort_x_rad) * (180/3.14159);
%ort_y_deg = (ort_y_rad) * (180/3.14159);
%ort_z_deg = (ort_z_rad) * (180/3.14159);

%orientation_matrix = [ort_x_deg, ort_y_deg, ort_z_deg];

%disp(orientation_matrix);