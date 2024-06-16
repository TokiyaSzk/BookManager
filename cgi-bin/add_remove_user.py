#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue May 28 11:31:58 2024

@author: tokya_pt
"""
#All is tested pass
#Need login check
import mysql.connector
#This program is aim to add or remove ,reader and admin
#This program use four paras in ctop.txt 
#format is con_header name password con_flag
def updatereader(rname,father,son):
    i=0
    while i <len(father):
        command = "UPDATE reader SET "
        command += father[i] + " = "
        command += "'"+str(son[i])+"'"
        command += " WHERE rname = "
        command += "'"+rname+"'"
        cursor.execute(command)
        conn.commit()
        i+=1
        print(command)
    print("updatereader sucess")
conn = mysql.connector.connect(
    host="localhost",
    user="root",
    port="3308",
    password="test",
    database="lib"
)
return_data = open("return_data.txt",mode="w")
cursor = conn.cursor()
file = open("standard.txt",mode="r")
result = file.read()
result = result.split()
con_header = result[0]#frist is user_admin or user_reader
name = result[1]#Second is name of user
con_pass = result[2]#Third is password of user
con_flag = result[3]#Forth is flag (add,remove or update)
if con_header == "user_reader" and con_flag=="add":
    #This is add reader
    command = "insert into reader (rname,repass,lenum,usenum) value (%s,%s,%s,%s)"
    value = ""
    value += name
    value += ","+con_pass
    value += ",3,0"
    value = tuple(part.strip() for part in value.split(','))
    print(value)
    cursor.execute(command,value)
    conn.commit()
    print("Reader add sucess") 
    return_data.write("True")
elif con_header == "user_admin" and con_flag=="add":
    #This is add admin
    command = "insert into admin (adname,adpass) value (%s,%s)"
    value = ""
    value += name
    value += ","+con_pass
    value = tuple(part.strip() for part in value.split(','))
    cursor.execute(command,value)
    conn.commit()
    print("Admin add sucess")
    return_data.write("True")
elif con_header == "user_reader" and con_flag=="remove":
    #This is remove reader 
    command = "DELETE FROM reader WHERE rname = "
    command += "'"+name+"'"
    cursor.execute(command)
    conn.commit()
    print("Reader remove sucess")
    return_data.write("True")
elif con_header == "user_admin" and con_flag=="remove":
    #This is remove admin
    command ="DELETE FROM admin WHERE adname = "
    command +="'"+name+"'"
    cursor.execute(command)
    conn.commit()
    print("Admin remove sucess")
    return_data.write("True")
elif con_flag == "update":
    #This is update reader
    father = ('repass',)
    son =(con_pass,)
    updatereader(name, father, son)
    print("Update password sucess")
    return_data.write("True")
else:
    print("Error")
    return_data.write("False")
    return_data.close()
return_data.close()




