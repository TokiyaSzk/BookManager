#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu May 30 10:37:09 2024

@author: tokya_pt
"""
import mysql.connector

conn = mysql.connector.connect(
    host="localhost",
    user="root",
    port="3308",
    password="test",
    database="lib"
)
cursor = conn.cursor()

file = open("standard.txt",mode="r")
return_data = open("return_data.txt",mode="w")
result = file.read()
result = result.split()
con_header = result[0]
name = result[1]
con_pass = result[2]
if con_header == "login_reader":
    command = "select * from reader where rname = '"+name+"' and repass = '"+con_pass+"'"
    cursor.execute(command)
    cursor_result = cursor.fetchall()
    if cursor_result == []:
        print("Error")
        return_data.write("False")
    else:
        print("Login sucess")
        return_data.write("True")
elif con_header =="login_admin":
    command = "select * from admin where adname = '"+name+"' and adpass = '"+con_pass+"'"
    cursor.execute(command)
    cursor_result = cursor.fetchall()
    if cursor_result == []:
        print("Error")
        return_data.write("False")
    else:
        print("Login sucess")
        return_data.write("True")
return_data.close()