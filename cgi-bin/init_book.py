#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon May 27 16:05:20 2024

@author: tokya_pt
"""

import mysql.connector
#This program is aim to init the book.txt.
#Read book information from Mysql 
#and write in book.txt to support the C program
conn = mysql.connector.connect(
    host="localhost",
    user="root",
    port="3308",
    password="test",
    database="lib"
)

cursor = conn.cursor()

cursor.execute("select * from book")
flie = open("book.txt",mode='w')
for x in cursor:
    for y in x:
        y = str(y)
        y = y + " "
        flie.write(y)
        print(y)
    flie.write("\n")
    #print(x)
# 关闭游标和连接
flie.close()
cursor.close()
conn.close()
