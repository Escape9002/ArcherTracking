%%sources
%https://www.youtube.com/watch?v=L3YueN2FgUM&list=TLPQMTExMjIwMjFUCOHtJI9Gjw&index=2

%% dif sensor from IMU it self
% initialise like in part 1

%status = readCalibrationStatus(imu)

% home base
imu_read = read(imu);
imu_matrix = imu_read{:,:}; % last three columns (orientation:) z y x | rest is xyz
imu_mean = mean(imu_matrix);

% accel
acc_x_ms = imu_mean{:,1};
acc_y_ms = imu_mean{:,2};
acc_z_ms = imu_mean{:,3};

acc_x_g = acc_x_ms / 9.81;
acc_y_g = acc_y_ms / 9.81;
acc_z_g = acc_z_ms / 9.81;

% gyro
gyro_x_rad = imu_mean{:,4};
gyro_y_rad = imu_mean{:,5};
gyro_z_rad = imu_mean{:,6};

gyro_x_deg = (gyro_x_rad) * (180*3.14159);
gyro_y_deg = (gyro_y_rad) * (180*3.14159);
gyro_z_deg = (gyro_z_rad) * (180*3.14159);

% mag
mag_x = imu_mean{:,7};
mag_y = imu_mean{:,8};
mag_z = imu_mean{:,9};

%orientation (possibly not available on MPU9250
ort_x_rad = imu_mean{:,12};
ort_y_rad = imu_mean{:,11};
ort_z_rad = imu_mean{:,10};

ort_x_def = (ort_x_rad) * (180*3.14159);
ort_y_def = (ort_y_rad) * (180*3.14159);
ort_z_def = (ort_z_rad) * (180*3.14159);

%% live stream acc

acc_count = 0;
acc_data = [];

while acc_count < 200
    acc_count = acc_count + 1;

    % home base
    imu_read = read(imu);
    imu_matrix = imu_read{:,:};
    imu_mean = mean(imu_matrix);

    % acc data
    acc_x_ms = imu_mean{:,1};
    acc_y_ms = imu_mean{:,2};
    acc_z_ms = imu_mean{:,3};
    
    % capture data, silently

    acc_data = [acc_data; [acc_count, acc_x_ms, acc_y_ms, acc_z_ms]];

    % what will i see in the command window
    show_me_acceleration = [acc_x_ms, acc_y_ms, acc_z_ms];
    disp(show_me_acceleration);

    if acc_count == 200
        displ('this is the end of accl session');
    end
end

acc_data;
acc_table = array2table(acc_data, 'variableNames', {'Time(0.1s)', 'acc x', 'acc y', 'acc x'})





%% live stream all data for 20 sec
imu_count = 0;
imu_data = [];

while imu_count < 200
    imu_count = imu_count+1;

    %imu base
    imu_read = read(imu);
    imu_matrix = imu_read{:,:};
    imu_mean = mean(imu_matrix);

    % accel
    acc_x_ms = imu_mean{:,1};
    acc_y_ms = imu_mean{:,2};
    acc_z_ms = imu_mean{:,3};

    %acc_x_g = acc_x_ms / 9.81;
    %acc_y_g = acc_y_ms / 9.81;
    %acc_z_g = acc_z_ms / 9.81;

    % gyro
    gyro_x_rad = imu_mean{:,4};
    gyro_y_rad = imu_mean{:,5};
    gyro_z_rad = imu_mean{:,6};

    %gyro_x_deg = (gyro_x_rad) * (180*3.14159);
    %gyro_y_deg = (gyro_y_rad) * (180*3.14159);
    %gyro_z_deg = (gyro_z_rad) * (180*3.14159);

    % mag
    mag_x = imu_mean{:,7};
    mag_y = imu_mean{:,8};
    mag_z = imu_mean{:,9};

    %orientation (possibly not available on MPU9250
    ort_x_rad = imu_mean{:,12};
    ort_y_rad = imu_mean{:,11};
    ort_z_rad = imu_mean{:,10};

    %ort_x_def = (ort_x_rad) * (180*3.14159);
    %ort_y_def = (ort_y_rad) * (180*3.14159);
    %ort_z_def = (ort_z_rad) * (180*3.14159);
    
    %data cap
    imu_data = [imu_matrix; [imu_count, acc_x_ms, acc_y_ms, acc_z_ms, gyro_x_rad, gyro_y_rad, gyro_z_rad, mag_x, mag_y,mag_z,ort_x_rad,ort_y_rad,ort_z_rad]];
    
    % see waht in the command i will
    show_me_imu = [acc_x_ms, acc_y_ms,acc_y_ms, gyro_x_rad, gyro_y_rad, gyro_z_rad, mag_x, mag_y,mag_z, ort_x_rad, ort_y_rad,ort_z_rad];
    disp(show_me_imu);

    if imu_count == 200
        disp('endenndndndn')
    end

end

imd_data;
imu_table = array2table(imu_data, 'VariableNames', ...
    {'Time (0.1s)', ...
    ' acc x ','acc y', 'acc z', ...
    'gyro x',' gyro y', 'gyro z', ...
    ' mag x', 'mag y',' mag z', ...
    'ort x', 'ort y', 'ort z'})









