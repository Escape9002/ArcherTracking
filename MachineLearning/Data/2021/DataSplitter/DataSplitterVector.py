import csv
import math

lines_read = 0
splitted = 0
valStorage = []

posVal = '20.00'
minVal = '-20.00'
collumns= 3

#shit = open("Spliftted1")

with open("1061_Shooting.csv", newline='\n') as f:    # open CSV file as f
    reader = csv.reader(f, delimiter=",")             # read it, splitsign ","

    print("STARTED")

    for lines_read, line in enumerate(reader):        #skip trough rows 
        #print ('line[{}] = {}'.format(lines_read, line))
        if(lines_read > 0):
            value = float(line[0]) + float(line[1]) + float(line[2])

            #print(math.sqrt(value))
            if(abs(value) > 20.00):
                splitted = splitted+1
                #value = abs(round(value,2))
                value = round(value,1)
                print(value)
                valStorage.append(value)
            
    
#shit.write(str(valStorage))

print("Splitted Times: ")
print(splitted)


output_file =open('months.csv', 'w')

for month in valStorage:
    output_file.write(str(month) + "\n")

output_file.close()

