# import pandas as pd
# 2
# 3
# #csv file name to be read in 
# 4
# in_csv = '2865_Shooting.csv'
# 5
# 6
# #get the number of lines of the csv file to be read
# 7
# number_lines = sum(1 for row in (open(in_csv)))
# 8
# 9
# #size of rows of data to write to the csv, 
# 10
# #you can change the row size according to your need
# 11
# rowsize = 500
# 12
# 13
# #start looping through data writing it to a new file for each set
# 14
# for i in range(1,number_lines,rowsize):
# 15
#     df = pd.read_csv(in_csv,
# 16
#           header=None,
# 17
#           nrows = rowsize,#number of rows to read at each loop
# 18
#           skiprows = i)#skip rows that have been read
# 19
# 20
#     #csv to write data to a new file with indexed name. input_1.csv etc.
# 21
#     out_csv = 'input' + str(i) + '.csv'
# 22
# 23
#     df.to_csv(out_csv,
# 24
#           index=False,
# 25
#           header=False,
# 26
#           mode='a',#append data to csv file
# 27
#           chunksize=rowsize)#size of data to append for each loop


import csv

with open("2865_Shooting.csv", "r") as f:
    reader = csv.reader(f, delimiter="\t")
    for i, line in enumerate(reader):
        print ('line[{}] = {}'.format(i, line))