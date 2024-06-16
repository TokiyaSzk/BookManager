#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue May 28 14:51:30 2024

@author: tokya_pt
"""

import mysql.connector
from insert_update_remove import updatebook
from add_remove_user import updatereader
from borrow import checkadmin
from datetime import datetime
import sys
#This program is aim to return book
#This program use 6 paras
#format is con_header username password bookname booknum outnum rname(admin visit)
def overdar(bname,rid):
    command = "select returnday from borrowbook where bname = '" +bname+"' and rid = "+str(rid)
    cursor.execute(command)
    cursor_result = cursor.fetchone()
    
    date_str = cursor_result[0]
    date_format = "%Y-%m-%d"   
    returnday = datetime.strptime(str(date_str), date_format)
    current_date = datetime.now()
    current_date = current_date.strftime("%Y-%m-%d")
    current_date = datetime.strptime(current_date, date_format)
    if returnday > current_date:
        return False
    else:
        return True

conn = mysql.connector.connect(
    host="localhost",
    user="root",
    port="3308",
    password="test",
    database="lib"
)
pay = True
cursor = conn.cursor()
file = open("standard.txt",mode="r")
result = file.read()
result = result.split()
return_data=open("return_data.txt",mode="w")
con_header = result[0]
name = result[1]
con_pass = result[2]
bname =  result[3]
#fathers is para to updatebook
fathers = ("bnum","outnum")
sons = (result[4],result[5])
if con_header == "return_reader":
    #This branch name is reader
    command = "select  rid,lenum,usenum from reader where rname = "
    command += "'" + name +"'"
    command += " AND repass = "
    command += "'"+con_pass+"'"
    print(command)
    cursor.execute(command)
    cursor_result = cursor.fetchone()
    if cursor_result == []:
        
        return_data.write("False")
        print("Bad")
        return_data.close()
        sys.exit()
    else:
        con_son1 =int(sons[0])+1
        con_son2 =int(sons[1])-1
        sons = (con_son1,con_son2)
        #change tuple sons data
        lenum = cursor_result[1]
        #get the reader leave numbers
        father = ("lenum","usenum")
        #father is para to updatereader
        rid = cursor_result[0]
        lenum = cursor_result[1]
        usenum = cursor_result[2]
        son = (lenum+1,usenum-1)
        #create the tuple son as para to updatereader
        #judege over return day 
        if overdar(bname, rid) == True :#已逾期
            print("支付逾期费用")
            if pay == False :#未支付
                print("是这里")
                return_data.write("False")
                return_data.close()
                sys.exit()
        updatebook(bname, fathers, sons)
        updatereader(name, father, son)
        command = "delete from borrowbook where rid = "
        command += str(rid) + " and bname = '"
        command += bname+"'"
        cursor.execute(command)
        conn.commit()
        print("Delete borrowbook sucess")
    print("Return book sucess")
    return_data.write("True")
elif con_header == "return_admin" :
    #This branch name is admin 
    #rname is reader
    rname = result[6]
    if checkadmin(name, con_pass) == True:
        command = "select  rid,lenum,usenum from reader where rname = "
        command += "'" + rname +"'"
        cursor.execute(command)
        cursor_result = cursor.fetchone()
        if cursor_result == []:
            print("Bad")
            return_data.write("False")
            return_data.close()
            sys.exit()
        else:
            con_son1 =int(sons[0])+1
            con_son2 =int(sons[1])-1
            sons = (con_son1,con_son2)
            lenum = cursor_result[1]
            father = ("lenum","usenum")
            son1 = int(lenum)+1
            son2 = int(cursor_result[2])-1
            rid = cursor_result[0]
            son = (son1,son2)
            updatebook(bname, fathers, sons)
            updatereader(rname, father, son)
            command = "delete from borrowbook where rid = "
            command += str(rid) + " and bname = '"
            command += bname+"'"
            cursor.execute(command)
            conn.commit()
            return_data.write("True")
    else:
        print("Error")
        return_data.write("False")
        sys.exit()
else:
    print("Error")
    return_data.write("False")
    return_data.close()
    sys.exit()
return_data.close()

sys.exit()
        
        