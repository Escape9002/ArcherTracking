from numpy import unicode_
import serial

ser = serial.Serial('COM4', 115200, timeout = 1)
ser.flushInput()

kopfzeile = "DataRate=10.000000\nDataType=Quaternion\nversion=3\nOpenSimVersion=4.3-2021-08-27-4bc7ad9\nendheader\ntime\tpelvis_imu\n" #\thumerus_l_imu\ttorso_imu\tulna_l_imu

sec = 60
timer = int(sec / (1/10))

print(timer * (1/10))

with open('data.sto', 'w') as data:
    data.write(kopfzeile)

    for i in range(timer):
        line = ser.readline()   # read a byte
        if line:
            string = (line.decode().replace("\n","").replace("b",""))  # convert the byte string to a unicode string and replace \n with emptyness
            #print(str(i) +"\t"+string)
            string = str(i) +"\t"+string
            print(string)
            data.write(string)