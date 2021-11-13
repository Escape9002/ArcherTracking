import csv
import math

#Value Bereich: 0-7 (5 kommt 1468 mal vor) [Verhältnis: 103450/1468 {1/70}]
#Innerhalb von 30 Punkten muss der Wert mind +5g machen für split bei schuss

valStorage = []



#--------------------------------------------------------------------------------READ DATA AND MARK SPLITTS WITH "/n"
lines_read = 0

valueNew = 0.00
valueOld = []

with open("1061_Shooting.csv", newline='\n') as f:    # open CSV file as f
    reader = csv.reader(f, delimiter=",")             # read it, splitsign ","

    print("STARTED")

    for lines_read, line in enumerate(reader):        #skip trough rows 

        if(lines_read > 0):
            valueOld.append(float(line[0]))
            valueNew = float(line[0])

            if((len(valueOld)>=30) and (valueNew > (valueOld[0]+5))):

                valStorage.append("/n")
                valueOld.clear()
            else:
                valStorage.append(line[0])

print(valStorage)                
#------------------------------------------------------------------------------SPLITT DATA
splitted = 0
partValStorage = []
FinalStorage = []

for x in valStorage:
    if "/n" in x:
        FinalStorage.append(partValStorage)
        partValStorage.clear()
        splitted = splitted+1
    else:
        partValStorage.append(x)

# for y in FinalStorage:
#     if "/n" in y:

print(FinalStorage.count("/n"))


print("Splitted Times: {}".format(splitted))

print(FinalStorage)
#------------------------------------------------------------------------------OUTPUT DATA

# output_file =open('Shot.csv', 'w')

# for point in FinalStorage:
#     output_file.write(str(point) + "\n")

# output_file.close()
id = 0
for point in FinalStorage:
    id = id+1
    with open("DataSplit{}".format(id),'w', newline='') as myFile:
        wr= csv.writer(myFile,delimiter = ' ', quotechar = '\n' , quoting =csv.QUOTE_ALL)
        wr.writerow(point)

