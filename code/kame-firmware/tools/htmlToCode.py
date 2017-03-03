#!/usr/bin/python3
# imports
import sys
import re

def conversion(fileName, varName):
    with open(fileName, 'r') as htmlFile:
        with open(fileName.split('.')[0]+'.h', 'w') as hFile:
            page = re.sub(r"\t+", "", htmlFile.read())
            page = re.sub(r"\n", "", page)
            page = re.sub(r"\"", "\\\"", page)
            hFile.write('#define '+fileName.split('.')[0]+'Message String message = "'+page+'";')


if __name__ == '__main__':
    conversion('root.html', 'message')
