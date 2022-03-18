%% Usage
%   This programm formats the .txt files of FusionQuat into .sto files usable
%   in OpenSim. In the final .sto file, dont forget the correct header and labels of the
%   sets, at the current state, the labels have to be in this order
%{
Dataformat of the needed .txt:
    0.01	0.335519811154871	0.705822462542093	0.397070637879537	-0.481223457686294
    0.02	0.336752341057119	0.706154846931129	0.396921565385659	-0.479996316527556
    0.03	0.338450166545752	0.706274387214777	0.396256018006081	-0.479175482389812

Dataformat of the produced .sto:
    0.00946462639110307,0.113816047674197,0.664279308593034,0.738707877522815
    0.00868743907346312,0.114345261962312,0.664280162232707,0.7386349271008
    0.00815369156982238,0.114768960931577,0.664324440311292,0.738535470323233
    0.00762302466885138,0.115041235236699,0.664353350799284,0.73847276792843
    0.00735361156096059,0.114924071283394,0.664291289536735,0.738549568331254
    0.007496266018264,0.114906493351165,0.664291663557113,0.738550532807366
    0.00778433079918756,0.114739815649035,0.664325395061053,0.738543125603245

needed Header:
    DataRate=100.000000
    DataType=Quaternion
    version=3
    OpenSimVersion=4.3-2021-08-27-4bc7ad9
    endheader
    endheader
    time	pelvis_imu	humerus_r_imu	torso_imu


%}
%% Clear the Workspace variables.
clear all; close all; clc;

%% Einlesen der Daten
pelvis = readtable('IMUData\ShootingQuat_PELVIS1.txt');
torso = readtable('IMUData\ShootingQuat_TORSO1.txt');
humerus = readtable('IMUData\ShootingQuat_BogenArm1.txt');
ulna = readtable('IMUData\ShootingQuat_BogenULNA.txt');

msg = 'read files'

time = pelvis(:,1);
msg = 'time table done'

pelvis_imu = [pelvis(:,2), pelvis(:,3), pelvis(:,4), pelvis(:,5)];
msg = 'pelvis done'

torso_imu = [torso(:,2), torso(:,3), torso(:,4), torso(:,5)];
msg = 'torso done'

humerus_imu = [humerus(:,2), humerus(:,3),humerus(:,4), humerus(:,5)];
msg = 'humerus done'

ulna_imu = [ulna(:,2), ulna(:,3),ulna(:,4), ulna(:,5) ];
msg = 'ulna done'
%% Ausgabe der Dateien
msg = 'Creating files'
writetable(time, 'STOFiles\time.csv', 'Delimiter', 'comma', 'WriteVariableNames',false);
writetable(pelvis_imu, 'STOFiles\pelivs.txt', 'Delimiter', 'comma', 'WriteVariableNames',false);
writetable(torso_imu, 'STOFiles\torso.txt', 'Delimiter', 'comma', 'WriteVariableNames',false);
writetable(humerus_imu, 'STOFiles\humerus.txt', 'Delimiter', 'comma', 'WriteVariableNames',false);
writetable(ulna_imu, 'STOFiles\ulna.txt', 'Delimiter', 'comma', 'WriteVariableNames',false);
msg = 'Finished'