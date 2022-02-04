%{
Dataformat:
time	pelvis_imu
0.01	0.335519811154871	0.705822462542093	0.397070637879537	-0.481223457686294
0.02	0.336752341057119	0.706154846931129	0.396921565385659	-0.479996316527556
0.03	0.338450166545752	0.706274387214777	0.396256018006081	-0.479175482389812
%}

%% Einlesen der Daten
pelvis = readtable('ShootingQuatPelvis.txt');
torso = readtable('ShootingQuatPelvis.txt');
humerus = readtable('ShootingQuatPelvis.txt');

time = pelvis(:,1)

pelvis_imu = [pelvis(:,2), pelvis(:,3), pelvis(:,4)]

torso_imu = [torso(:,2), torso(:,3), torso(:,4)]

humerus_imu = [humerus(:,2), humerus(:,3),humerus(:,4)]
%%
finalData = table(time, pelvis_imu, torso_imu, humerus_imu)


