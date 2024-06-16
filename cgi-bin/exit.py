#!/usr/bin/python3

import cgitb

file = open("return_data.txt","w")
file1 = open("data.txt","w")
file2 = open("standard.txt","w")
file.close()
file1.close()
file2.close()
# 打印标准的 HTTP 响应头


print("Status: 302 Found")
print("Location: http://localhost/exit.html")
print("Content-Type: text/html")
print()  # 打印一个空行