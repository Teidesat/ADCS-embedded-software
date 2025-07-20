import sys
import math

# take the csv output from (name of the program for generating orbital data) 
# a orbital simulation, and add a time column to be compatible with matlab's format
def add_time_column(fileName):
    print("new file name: " + fileName)
    
    newFileContent = ""

    with open(fileName, "r", encoding="utf-8") as file:
        time = 0
        timelapse = 0.1
        
        for line in file:
            newLine = str(time) + "," + line
            newFileContent = newFileContent + newLine
            time = time + timelapse
            time = math.floor(time*100)/100

    with open(fileName, "w") as file:
        file.write(newFileContent)



add_time_column(sys.argv[1])