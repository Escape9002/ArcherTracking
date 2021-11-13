from os import listdir

def find_csv_filenames( path_to_dir, suffix=".csv" ):
    filenames = listdir(path_to_dir)
    return [ filename for filename in filenames if filename.endswith( suffix ) ]

filenames = find_csv_filenames("C:/Users/anton/Videos/JugendForscht/2020_21/BogenTracker/ArcherTracking/MachineLearning/Data/2021/DataSplitter")
for name in filenames:
  print (name)