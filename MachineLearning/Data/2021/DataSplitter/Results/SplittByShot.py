import csv
import math

#Value Bereich: shot is found when acceleration goes faster than a certain limit (5) from then to now (==30 datapoints )
#-----------------------------------------------------------------------------GLOBAL VARIABLES
valStorage = []
id = 0
FinalStorage = []

splitted = 0

runtime = 0
DataList = []
#-------------------------------------------------------------------------------IMPORT ALL FILES
from os import listdir

def find_csv_filenames( path_to_dir, suffix=".csv" ):
    filenames = listdir(path_to_dir)
    return [ filename for filename in filenames if filename.endswith( suffix ) ]

#--------------------------------------------------------------------------------READ DATA AND MARK
# WHEN PointNow is bigger than point -30 SPLITTS WITH "\n" 
# OUTPUT: [0.00,0.00,0.00]
def ReadAndMark(runtimex):
    lines_read = 0

    valueNew = 0.00
    valueOld = []

    with open("{}".format(DataList[runtimex]), newline='\n') as f:    # open CSV file as f
        reader = csv.reader(f, delimiter=",")             # read it, splitsign ","

        for lines_read, line in enumerate(reader):        #skip trough rows 

            if(lines_read > 0):
                valueOld.append(float(line[0]))
                valueNew = float(line[0])

                if((len(valueOld)>=30) and (valueNew > (valueOld[0]+5))):

                    valStorage.append("\n")
                    valueOld.clear()
                else:
                    valStorage.append("{},{},{}".format(line[0], line[1],line[2]))             
#------------------------------------------------------------------------------SPLITT DATA 
# WHEN \n is found SPLITTED TO NEW ARRAY
# OUTPUT: [[0.00,0.00,0.00 \n ...] [0.00,0.00,0.00 \n ...]...]
def splittData(valStoragex, splittedx):
    partValStorage = []
   
    for x in valStoragex:
        if "\n" in x:
            FinalStorage.append(partValStorage)
            partValStorage.clear()
            splittedx = splittedx+1
        else:
            partValStorage.append(x)
    return splittedx
    

#------------------------------------------------------------------------------OUTPUT DATA 
# OUTPUT AS CSV FILE
# OUTPUT: each file has data with an empty row between it

# output_file =open('Shot.csv', 'w')

# for point in FinalStorage:
#     output_file.write(str(point) + "\n")

# output_file.close()
def outputData(id,runtimex, FinalStoragex):
    for point in FinalStoragex:
        with open("DataSplit_{}_{}".format(id,DataList[runtimex]),'w', newline='') as myFile:
            wr= csv.writer(myFile,delimiter = ' ', quotechar = '\n' , quoting =csv.QUOTE_ALL)
            wr.writerow(point)
#-----------------------------------------------------------------------------DEBUG
# Must be called as last funktion, as values are partly created while programm is running
def DEBUG(truex):
    if(truex == 1):
        for x in DataList:
            print (x)
        print("Splitted {} Times".format(splitted))
        print(FinalStorage)
        print(valStorage)   
#-----------------------------------------------------------------------------STATUS
def status(DataLists,runtimex,splittedx):
    if(runtimex <1):
        print("ALL SETS / WORKING SET || % || Splitts per Set")
    print("{}/{} || {}% || {}".format(len(DataLists), runtimex+1, ((runtimex+1) / len(DataLists))*100, splittedx))
#-----------------------------------------------------------------------------MAIN()
print()
path = input("Please enter the full path of where the Working files are. The ResultFiles will be released into the folder where the programm is running!")

filenames = find_csv_filenames(path)
for name in filenames:
    DataList.append(name)

for x in DataList:
    status(DataList, runtime, splitted)
    ReadAndMark(runtime)
    splitted = splittData(valStorage,splitted)
    outputData(id,runtime,FinalStorage)
    #DEBUG(1)
    runtime = runtime+1
    id = id+1
    