#!/usr/bin/python3
import mysql.connector
import json

# 连接到MySQL数据库
conn = mysql.connector.connect(
    host="localhost",
    user="root",
    port="3308",
    password="test",
    database="lib"
)

# 创建游标对象
cursor = conn.cursor()

# 执行SQL查询
cursor.execute("show tables")

# 获取查询结果
results = cursor.fetchall()

# 将结果转换为JSON格式并打印到标准输出
print(json.dumps(results))

# 关闭游标和连接
cursor.close()
conn.close()
