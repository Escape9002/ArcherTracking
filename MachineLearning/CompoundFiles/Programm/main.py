import os
from os import listdir

def find_csv_filenames( path_to_dir, suffix=".txt" ):
    filenames = listdir(path_to_dir)
    return [ filename for filename in filenames if filename.endswith( suffix ) ]

path = input("Please enter the full path of where the Working files are. The ResultFiles will be released into the folder where the programm is running!")
# C:\Users\makerspace\Documents\_NUTZER\RehAntonio\JuFo\ArcherTracking\MachineLearning\CompoundFiles\Programm
print("Running......................................................................next File")

readFiles = []
filenames = find_csv_filenames(path)
for name in filenames:
    readFiles.append(name)

#print(readFiles)

outputFile = open('outputFile.txt', 'w')

for file in readFiles:
    x = 0
    with open(file) as inputFile:
        lines = inputFile.readlines()
        #print(lines)

    for line in lines:
        if not (x==0):
            outputFile.write(line)
        else:
            outputFile.write("aX,aY,aZ \n")
        x = x +1
    outputFile.write('\n')
    print("Running......................................................................last File length:  {}".format(x)) 





