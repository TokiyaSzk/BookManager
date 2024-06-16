#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue May 28 14:08:36 2024

@author: tokya_pt
"""
import mysql.connector
from insert_update_remove import updatebook
from add_remove_user import updatereader
from datetime import datetime, timedelta
import sys
#This program is aim to borrow book
#This program use 6 para
#format is con_header name password con_name booknum outnum 
conn = mysql.connector.connect(
    host="localhost",
    user="root",
    port="3308",
    password="test",
    database="lib"
)

cursor = conn.cursor()
return_data = open("return_data.txt",mode="w")
def checkadmin(adname,adpass):
    command = "select adname,adpass from admin where adname = "
    command += "'" + adname +"'"
    cursor.execute(command)
    cursor_result = cursor.fetchall()
    if cursor_result == []:
        return False #not admin
    else:
        return True #is admin

def insertborrowbook(bname,rname):
    command = "select bid,bname from book where bname = '"
    command += bname+"'"
    cursor.execute(command)
    cursor_result = cursor.fetchone()
    values="" #init value & use string to add more substring
    for x in cursor_result: #add substring to values(mainstring)
        values+=str(x)+","
    command = "select rid,rname from reader where rname = '"
    command += rname+"'"
    cursor.execute(command)
    cursor_result = cursor.fetchone()
    for x in cursor_result:
        values+=str(x)+","
    current_date = datetime.now()
    future_date = current_date + timedelta(days=30)
    localtime = current_date.strftime("%Y-%m-%d")
    values +=localtime+","
    values +=future_date.strftime("%Y-%m-%d")
    values = tuple(part.strip() for part in values.split(','))
    print(values)
    command = "insert into borrowbook (bid,bname,rid,rname,borrowday,returnday) values(%s,%s,%s,%s,%s,%s)"
    cursor.execute(command,values)
    conn.commit()
    #print("insertborrowbook sucess")
file = open("standard.txt",mode="r")
result = file.read()
result =result.split()
con_header = result[0]
name = result[1]
con_pass = result[2]
con_name = result[3]
fathers = ('bnum','outnum')
sons = (result[4],result[5])
con_sons = []
if con_header == "borrow_reader":
    repass = con_pass
    command = "select  lenum,usenum from reader where rname = "
    command += "'" + name +"'"
    command += " AND repass = "
    command += "'"+repass+"'"
    cursor.execute(command)
    cursor_result = cursor.fetchone()
    if cursor_result == []:
        print("Bad")
        return_data.write("False")
        return_data.close()
        sys.exit()
    else:
        lenum = cursor_result[0]
        if int(lenum) <= 0:
            print("You can't borrow")
            return_data.write("False")
            return_data.close()
            sys.exit()
        lenum = cursor_result[0]-1
        usenum = cursor_result[1]+1
        father = ("lenum","usenum")
        son = (lenum,usenum)
        #reader only can cut 1 book
        con_sons.append(int(sons[0]))
        con_sons.append(int(sons[1]))
        con_sons[0] -= 1
        con_sons[1] += 1
        sons = tuple(con_sons)
        updatebook(con_name,fathers,sons)
        updatereader(name,father,son)
        insertborrowbook(con_name, name)
        print("借阅成功")
        return_data.write("True")
elif con_header == "borrow_admin" :
    adname = name
    adpass = con_pass
    if checkadmin(adname,adpass) == True:
        updatebook(con_name, fathers, sons)
        return_data.write("True")
    else:
        print("Bad")
        return_data.write("False")
        return_data.close()
        sys.exit()
else:
    print("Error")
    return_data.write("False")
    return_data.close()
return_data.close()
