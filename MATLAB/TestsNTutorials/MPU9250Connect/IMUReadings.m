a = arduino('COM14', 'Uno', 'Libraries', 'I2C');
fs = 100;
imu = mpu9250(a,'SampleRate',fs,'OutputFormat','matrix'); 
GyroscopeNoiseMPU9250 = 3.0462e-06;
AccelerometerNoiseMPU9250 = 0.0061;
viewer = HelperOrientationViewer('Title',{'Visualization of Orientation'})
FUSE = kalmanfilter('SampleRate',imu.SampleRate, 'GyroscopeNoise',GyroscopeNoiseMPU9250,'AccelerometerNoise',AccelerometerNoiseMPU9250);
tic;
stoptimer = 100;
magreadings = [];
accelreadings = [];
gyroreadings = [];
while(toc<stopTimer) 
    [accel,gyro,mag] = read(imu);
    magreadings = [magreadings;mag];
    accelreadings = [accelreadings;accel];
    gyroreadings = [gyroreadings;gyro];
    fuseReadings = FUSE(accel,gyro,mag);
    for j = numel(fuseReadings)
       viewer(fuseReadings(j));
    end
    subplot(3,2,1);
    plot(accelreadings);drawnow
    title('Accelerometer readings');
    subplot(3,2,2);
    plot(gyroreadings);drawnow
    title('GyroScope readings');
    subplot(3,1,3);
    plot(magreadings);drawnow
    title('Magnetometer readings');
end
%%magx_min = min(magReadings);
%%magx_max = max(magReadings);
%%magx_correction = (magx_max+magx_min)/3;



