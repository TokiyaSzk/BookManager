#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed May 29 10:16:58 2024

@author: tokya_pt
"""

import mysql.connector
#This program is aim to search book information
#This program use 1+n paras
#format is name
#This program need data.txt
conn = mysql.connector.connect(
    host="localhost",
    user="root",
    port="3308",
    password="test",
    database="lib"
)
cursor = conn.cursor()
return_data = open("data.txt",mode="w")
file = open("standard.txt",mode="r")
result = file.read()
result = result.split()

name = result[1]

command = "select rid,rname,lenum,usenum from reader where rname = '"+name+"'"
cursor.execute(command)
for x in cursor:
    for y in x:
        print(y)
        return_data.write(str(y)+" ")
return_data.write("\n")
command = "select bid,rid,bname,borrowday,returnday from borrowbook where rname = '"+name+"'"
cursor.execute(command)
for x in cursor:
    for y in x:
        print(y)
        return_data.write(str(y)+" ")
    return_data.write("\n")
return_data.close()
