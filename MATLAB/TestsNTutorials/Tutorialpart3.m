%% sources
%https://www.youtube.com/watch?v=GfhJTbFS1go&list=TLPQMTExMjIwMjFUCOHtJI9Gjw&index=3

%%draw the graph
tiledlayout(3,1);
nexttile;

%//////////////////////////////////////////////////////////////////////////acc 
acc_x_line = animatedline;
axis ([1 100 -3 3]);

acc_x_line.LineStyle = '-';
acc_x_line.LineWidth = 2;
acc_x_line.Color = '[0.99 0.01 0.01]';
acc_x_line_title = title ('Accelerometer', 'FontSize', 11);
acc_x_line_xlabel = xlabel('Time (s)');
acc_x_line_xlabel.FontSize = 9;
acc_x_line_ylabel = ylabel('acc g');
acc_x_line_ylabel.FontSize = 9;
set(gca, 'Color', '[0.45 0.45 0.5]');
grid on;
hold on;

acc_y_line = animatedline;
axis ([1 100 -3 3]);

acc_y_line.LineStyle = '-';
acc_y_line.LineWidth = 2;
acc_y_line.Color = '[0.99 0.35 0.01]';
acc_y_line_title = title ('Accelerometer', 'FontSize', 11);
acc_y_line_xlabel = xlabel('Time (s)');
acc_y_line_xlabel.FontSize = 9;
acc_y_line_ylabel = ylabel('acc g');
acc_y_line_ylabel.FontSize = 9;
set(gca, 'Color', '[0.45 0.45 0.5]');
grid on;
hold on;

acc_z_line = animatedline;
axis ([1 100 -3 3]);

acc_z_line.LineStyle = '-';
acc_z_line.LineWidth = 2;
acc_z_line.Color = '[0.99 0.70 0.01]';
acc_z_line_title = title ('Accelerometer', 'FontSize', 11);
acc_z_line_xlabel = xlabel('Time (s)');
acc_z_line_xlabel.FontSize = 9;
acc_z_line_ylabel = ylabel('acc g');
acc_z_line_ylabel.FontSize = 9;
set(gca, 'Color', '[0.45 0.45 0.5]');
grid on;
hold on;

acc_legend = legend([acc_x_line, acc_y_line, acc_z_line], {'x axis','y axis','z axis'});
acc_legend.FontSize = 12;
acc_legend.Location = 'northeastoutside';
acc_legend.NumColumns = 1;
acc_legend.TextColor = 'w';
acc_legend.Title.String = 'accl data';
acc_legend.Title.Color = 'w';
acc_legend.Title.FontSize = 11;
hold off;

nexttile;

%//////////////////////////////////////////////////////////////////////////gyro
gyro_x_line = animatedline;
axis ([1 100 -10 10]);

gyro_x_line.LineStyle = '-';
gyro_x_line.LineWidth = 2;
gyro_x_line.Color = '[0.01 0.01 0.99]';
gyro_x_line_title = title ('gyroscope', 'FontSize', 11);
gyro_x_line_xlabel = xlabel('Time (s)');
gyro_x_line_xlabel.FontSize = 9;
gyro_x_line_ylabel = ylabel('angl vel rad/s');
gyro_x_line_ylabel.FontSize = 9;
set(gca, 'Color', '[0.45 0.45 0.5]');
grid on;
hold on;

gyro_y_line = animatedline;
axis ([1 100 -10 10]);

gyro_y_line.LineStyle = '-';
gyro_y_line.LineWidth = 2;
gyro_y_line.Color = '[0.01 0.31 0.99]';
gyro_y_line_title = title ('gyroscope', 'FontSize', 11);
gyro_y_line_xlabel = xlabel('Time (s)');
gyro_y_line_xlabel.FontSize = 9;
gyro_y_line_ylabel = ylabel('angl vel rad/s');
gyro_y_line_ylabel.FontSize = 9;
set(gca, 'Color', '[0.45 0.45 0.5]');
grid on;
hold on;

gyro_z_line = animatedline;
axis ([1 100 -10 10]);

gyro_z_line.LineStyle = '-';
gyro_z_line.LineWidth = 2;
gyro_z_line.Color = '[0.01 0.70 0.99]';
gyro_z_line_title = title ('gyroscope', 'FontSize', 11);
gyro_z_line_xlabel = xlabel('Time (s)');
gyro_z_line_xlabel.FontSize = 9;
gyro_z_line_ylabel = ylabel('angl vel rad/s');
gyro_z_line_ylabel.FontSize = 9;
set(gca, 'Color', '[0.45 0.45 0.5]');
grid on;
hold on;

gyro_legend = legend([gyro_x_line, gyro_y_line, gyro_z_line], {'x axis','y axis','z axis'});
gyro_legend.FontSize = 12;
gyro_legend.Location = 'northeastoutside';
gyro_legend.NumColumns = 1;
gyro_legend.TextColor = 'w';
gyro_legend.Title.String = 'gyro data';
gyro_legend.Title.Color = 'w';
gyro_legend.Title.FontSize = 11;
hold off;


% loop
 imu_count = 0;
 imu_data = [];

 while imu_count < 100
     imu_count = imu_count +1;

     %home base
     imu_read = read(imu);
     imu_matrix = imu_read{:,:};
     imu_mean = mean(imu_matrix);

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


     imu_data = [imu_data; [imu_count, acc_x_g, acc_y_g, acc_z_g, gyro_x_deg, gyro_y_deg, gyro_z_deg]];

     show_me_imu = [acc_x_g, acc_y_g, acc_z_g, gyro_x_deg, gyro_y_deg, gyro_z_deg];

     disp(show_me_imu);

     if imu_count == 100
         disp('this is the end anikin');

     end


addpoints(acc_x_line, imu_count, acc_x_g);
addpoints(acc_y_line, imu_count, acc_y_g);
addpoints(acc_z_line, imu_count, acc_z_g);

addpoints(gyro_x_line, imu_count, gyro_x_rad);
addpoints(gyro_y_line, imu_count, gyro_y_rad);
addpoints(gyro_z_line, imu_count, gyro_z_rad);

drawnow;

 end

 imu_data;
 imu_table = array2table(imu_data, 'VariableNames',{'Time (0.1s)',' acc x ','acc y', 'acc z',  'gyro x',' gyro y', 'gyro z'})

%% find max g z axis

imu_data;
[max_z_acc, index] = max(imu_data(:,3))

 

