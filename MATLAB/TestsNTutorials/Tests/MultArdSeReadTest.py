from tokenize import String
from numpy import unicode_
import serial

ser1 = serial.Serial('COM5', 115200, timeout = 1)
ser1.flushInput()
msg1 = ""

ser2 = serial.Serial('COM3', 115200, timeout = 1)
ser2.flushInput()
msg2=""

dataString =""

kopfzeile = "DataRate=50.000000\nDataType=Quaternion\nversion=3\nOpenSimVersion=4.3-2021-08-27-4bc7ad9\nendheader\ntime\thumerus_r_imu\tulna_r_imu\n" #\ttorso_imu\tulna_l_imu

sec = 10
timer = int(sec / (1/10))
msg = []

print(timer * (1/10))

with open('data.sto', 'w') as data:
    data.write(kopfzeile)

    for i in range(timer):
        line1 = ser1.readline()   # read a byte
        line2 = ser2.readline()   # read a byte
        if line1:
            string = (line1.decode().replace("\n","").replace("b",""))  # convert the byte string to a unicode string and replace \n with emptyness
            #print(str(i) +"\t"+string)
            values = string.split(",")
            for dat in values:
                msg.append((int(dat) -100) / 100)
            msg1 = str(msg[0]) +"," + str(msg[1]) +","+ str(msg[2]) +","+ str(msg[3]) 
            msg.clear()
            #print("1:"+ msg1)
            

        if line2:
            string = (line2.decode().replace("\n","").replace("b",""))  # convert the byte string to a unicode string and replace \n with emptyness
            #print(str(i) +"\t"+string)
            values = string.split(",")
            for dat in values:
                msg.append((int(dat) -100) / 100)
            msg2 =str(msg[0]) +"," + str(msg[1]) +","+ str(msg[2]) +","+ str(msg[3])
            msg.clear()
            #print("2:" + msg2)
            
        
        dataString = str(i) + "\t" + msg1 + "\t" + msg2 + "\n"
        print(dataString)
        data.write(dataString)