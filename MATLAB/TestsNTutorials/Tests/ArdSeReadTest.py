from asyncio.windows_events import NULL
import serial

import csv
import json

ser = serial.Serial('COM4', 115200, timeout = 1)
ser.flushInput()

kopfzeile = "DataRate=10.000000\nDataType=Quaternion\nversion=3\nOpenSimVersion=4.3-2021-08-27-4bc7ad9\nendheader\ntime\tpelvis_imu" #\thumerus_l_imu\ttorso_imu\tulna_l_imu
timer = 500

print(timer * (1/10))

with open('adressen_schreiben.txt', 'w', encoding='utf-8') as data:
    data.write(kopfzeile)

    for i in range(timer):
        line = ser.readline()   # read a byte
        if line:
            string = line.decode()  # convert the byte string to a unicode string
            #num = int(string) # convert the unicode string to an int
            print(str(i) +"\t"+string)
            data.write(str(i) +"\t"+ string)

        