#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue May 28 10:41:53 2024

@author: tokya_pt
"""
#All is tested pass
import mysql.connector
#This program is aim to insert , update or remove  book imformation in mysql
#This program use 8 paras
#This program need data.txt
#format is con_header bname author company bnum locate inday outnum
#If con_header is update ,you need to open the new data.txt to read data
#and colum which behind in bname will be column's name 
#This program don't allow reader to use
#Every use this program will be admin
conn = mysql.connector.connect(
    host="localhost",
    user="root",
    port="3308",
    password="test",
    database="lib"
)

cursor = conn.cursor()

def updatebook(bname,fathers,sons):
    i = 0
    while i < len(fathers):#loop to update columns
        command = "UPDATE book SET "
        command+= fathers[i] +" = "
        command+= "'"+str(sons[i])+"'" 
        command+=" WHERE bname = "
        command+="'"+bname +"'"
        print(command)
        cursor.execute(command)
        conn.commit()
        i+=1
    print("Update book sucess")
    
return_data = open("return_data.txt",mode="w")
file = open("standard.txt",mode="r")
result = file.read()
result = result.split()
con_header = result[0]
bname = result[1]
if con_header == "book_insert":
        
    #This is insert
    command = "insert into book (bname,author,company,bnum,locate,indy,outnum) values(%s,%s,%s,%s,%s,%s,%s)"
    value = ""
    temp = 1
    while temp < len(result):
        value += result[temp] +","
        temp +=1
    #value = "苏东坡传,林语堂,苏教,3,2-A-100-1,2024-1-1"
    value += "0"
    value = tuple(part.strip() for part in value.split(','))
    print(value)
    cursor.execute(command,value)
    conn.commit()
    print("Insert book sucess")
    return_data.write("True")
elif con_header == "book_update":
    fathers = ""
    i = 2
    while i < len(result):
        fathers += result[i] +" "
        i+=1
    fathers = tuple(fathers.split())
    data = open("data.txt",mode="r")
    data_result = data.read()
    sons = tuple(data_result.split())
    updatebook(bname, fathers, sons)
    print("Update book sucess")
    return_data.write("True")
elif con_header == "book_remove":
    #This is remove
    command = "DELETE FROM book WHERE bname = "
    command += "'"+bname+"'"
    cursor.execute(command)
    conn.commit()
    print("Delete book sucess")
    return_data.write("True")
else:
    print("Error")
    return_data.write("False")
    return_data.close()
return_data.close()


