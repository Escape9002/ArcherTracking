import csv

with open("2865_Shooting.csv", "r") as f:
    reader = csv.reader(f, delimiter="\t")
    for i, line in enumerate(reader):
        print ('line[{}] = {}'.format(i, line))