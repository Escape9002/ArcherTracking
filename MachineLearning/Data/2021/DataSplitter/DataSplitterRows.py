import csv

lines_read = 0
splitted = 0
valStorage = []

posVal = '20.00'
minVal = '-20.00'
collumns= 3


with open("1061_Shooting.csv", newline='\n') as f:    # open CSV file as f
    reader = csv.reader(f, delimiter=",")             # read it, splitsign ","

    print("started")

    for lines_read, line in enumerate(reader):        #skip trough rows 
        #print ('line[{}] = {}'.format(lines_read, line))

        if lines_read > 0:                            #skip the header || will produce Error (wrong datatype ) otherwise
          #print("debug0")
          valStorage.append(line)
  
                                   #iterate trough collumns (3)
          #print("debug1")
          if len(valStorage) >5:
            for a in range(collumns): 
              if line[a]< minVal or posVal < line[a]:  # if value is higher/ lower than maximum 
                splitted = splitted + 1                 # do the split
                #print("true " + str(valStorage))

                with open("DataSplit{}".format(splitted),'w', newline='') as myFile:
                  wr= csv.writer(myFile,delimiter = ' ', quotechar = '\n' , quoting =csv.QUOTE_ALL)
                  wr.writerow(valStorage)

                valStorage= []

              else:
                a = a +1
                #print(a)
              


              #print(valStorage)

              

              


              


print("Read Lines: " + str(lines_read) + " Splitted: " + str(splitted))

# #---------------------------------------------------------------------------------------------
# #start looping through data writing it to a new file for each set

# for i in range(1,number_lines,rowsize):

#     df = pd.read_csv(in_csv,

#           header=None,

#           nrows = rowsize,#number of rows to read at each loop

#           skiprows = i)#skip rows that have been read


#     #csv to write data to a new file with indexed name. input_1.csv etc.

#     out_csv = 'input' + str(i) + '.csv'


#     df.to_csv(out_csv,

#           index=False,

#           header=False,

#           mode='a',#append data to csv file

#           chunksize=rowsize)#size of data to append for each loop
