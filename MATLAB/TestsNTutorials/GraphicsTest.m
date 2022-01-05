%% simple cube
a = -pi : pi/2 : pi;                                % Define Corners
ph = pi/4; %default: pi/4                                          % Define Angular Orientation ( Phase%)
x = [cos(a+ph); cos(a+ph)]/cos(ph);
y = [sin(a+ph); sin(a+ph)]/sin(ph);
z = [-ones(size(a)); ones(size(a))];

figure
surf(x, y, z, 'FaceColor','g')                      % Plot Cube
hold on
patch(x', y', z', 'r')                              % Make Cube Appear Solid
hold off
axis([ -1  1    -1  1    -1  1]*1.5)
grid on

%% Orietation Plot
ld = load('rpy_9axis.mat')

accel = ld.sensorData.Acceleration;
gyro = ld.sensorData.AngularVelocity;    
Fs  = ld.Fs; %Hz of running sensor
decim = 2; 
fuse = imufilter('SampleRate',Fs,'DecimationFactor',decim);

pose = fuse(accel,gyro);

tp = theaterPlot('XLimit',[-2 2],'YLimit',[-2 2],'ZLimit',[-2 2]);
op = orientationPlotter(tp,'DisplayName','Fused Data',...
    'LocalAxesLength',2);

for i=1:numel(pose)
    plotOrientation(op, pose(i))
    drawnow
end