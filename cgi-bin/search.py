#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed May 29 10:16:58 2024

@author: tokya_pt
"""

import mysql.connector
#This program is aim to search book information
#This program use 1+n paras
#format is con_header table column1 column2 ....
#This program need data.txt
conn = mysql.connector.connect(
    host="localhost",
    user="root",
    port="3308",
    password="test",
    database="lib"
)
cursor = conn.cursor()
return_data = open("return_data",mode="w")
file = open("standard.txt",mode="r")
result = file.read()
result = result.split()

con_header = result[0] #all or special 
fathers = "" #This is the tuple will be column in table book :bname,author,company
sons = "" #Thisis the tuple
i = 2
while i < len(result):
    fathers += result[i]+" "
    i+=1
fathers = tuple(fathers.split())
data = open("data.txt",mode="r")
data_result = data.read()
data_result = data_result.split()
for z in data_result:
    sons += z+" "
sons = tuple(sons.split())


if con_header  == "all":
    command = "select * from "
    command +=result[1]+" "
    cursor.execute(command)
    return_data.write("True")
    return_data.write("\n")
    for x in cursor:
        for y in x:
            return_data.write(str(y)+" ")
        return_data.write("\n")
elif con_header == "special":
    i=0
    command = "select * from  "
    command += result[1]+" where "
    command += fathers[i] + " = " + "'"+sons[i]+"'"
    i = 1
    if len(fathers) > 1:
        while i <len(fathers):
            command += " and "
            command += fathers[i] + " = " +"'"+sons[i]+"'"
            i+=1
            print(command)
    cursor.execute(command)
    return_data.write("True")
    for x in cursor:
        for y in x:
            print(str(y))
        print("\n")
else:
    print("Error")
    return_data.write("False")
    return_data.close()
    SystemExit()
return_data.close()