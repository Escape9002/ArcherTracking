Here you can find some Matlab tests featuring the MPU9250 and OpenSim, the programm I now use to visualize the motion data.
A working visualization programm can be found [here](https://github.com/Escape9002/ArcherTracking/tree/main/MATLAB/OpenSim).



## How To:

- OpenSim Version used: 4.3

0. Upload [MPU9250_DMP_Serial](https://github.com/Escape9002/ArcherTracking/tree/main/MATLAB/OpenSim/MPU9250_DMP_Serial/MPU9250_DMP_Serial.ino) to your Arduino!
1. Plug in your Arduino and write the correct port into [SingleArdSeRead](https://github.com/Escape9002/ArcherTracking/tree/main/MATLAB/OpenSim/SingleArdSeRead.py), line 4. Then configure the header to feature the correct data for OpenSim's .sto files! Running the programm creates the file "data.sto".
2. To get a working/pleasing (aka, not making the skeleton motion sick :) )  Visualisation you need at least 2 sensors, thus run [sensorData_Connector](https://github.com/Escape9002/ArcherTracking/tree/main/MATLAB/OpenSim/sensorData_Connector.py). Configure the header! The Output-File can be named by your pleasing.
3. Open OpenSim, import the model (e.g. [Rajagopal_2015.osim](https://github.com/Escape9002/ArcherTracking/tree/main/MATLAB/OpenSim/model/Rajagopal_2015.osim)).
4. Choose the "IMU Placer" from "Tools", import your .sto file, adjust your orientations (wearing the sensor pointing in the direction of view, I used x:-90). Click "Run"
5. Choose the "IMU Inversive Kinematic" button from "Tools", import the same .sto file, adjust the orientations the same way as in 4, adjust the "Weights" (if you dont know how, dont change anything, it's still needed as the programm will return an error if you don't...). Choose a file in which you write your results ("results.mot") and click "Run"!
6. Enjoy (and speed up, around 4.00 speed up shows it about realtime for me)