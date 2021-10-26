import csv

lines_read = 0
splitted = 0
valStorage = []

posVal = '20.00'
minVal = '-20.00'
collumns= 3


with open(1061_Shooting.csv, newline='\n') as f:    # open CSV file as f
    reader = csv.reader(f, delimiter=",")             # read it, splitsign ","

    print("started")

    for lines_read, line in enumerate(reader):        #skip trough rows 
        #print ('line[{}] = {}'.format(lines_read, line))
       
        print(line)