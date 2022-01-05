%% Arduino Init
a = arduino
%% MPU init
%imu = mpu6050(a)
imu = mpu9250(a)
%%
acc_ort = []

i = 1;
while i < 10000
imu_read = imu.readAcceleration;

aX = imu_read(1);
aY = imu_read(2);
aZ = imu_read(3);

gX = imu_read(4);
gY = imu_read(5);
gZ = imu_read(6);



%euler winkel

  aX = aX / 9, 81;
  aY = aY / 9, 81;
  aZ = aZ / 9, 81;

  roll = atan(aY / sqrt(aX^2 + aZ^2)) * 180 / pi;
  pitch = atan(-1 * aX / sqrt(aY^2 + aZ^2)) * 180 / pi;

acc_ort = [acc_ort; [i,roll, pitch]]




i = i+1;
end

acc_ort_table = array2table(acc_ort, "variableNames",{'time', 'pitch', 'roll' })

%acc_table = array2table(acc_data, 'variableNames', {'Time(0.1s)', 'acc x', 'acc y', 'acc x'})


disp(acc_ort_table)

