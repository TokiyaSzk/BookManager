//
//  main.c
//  postdata
//
//  Created by Tokya_mac on 2024/6/1.
//
//This program is to get post login data

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

// 定义缓冲区大小
#define MAX_BUFFER_SIZE 1024

// URL解码函数
void url_decode(char *src, char *dest) {
    char ch;
    while ((ch = *src) != '\0') {
        if (ch == '+') {
            *dest = ' ';
        } else if (ch == '%' && isxdigit((int) *(src + 1)) && isxdigit((int) *(src + 2))) {
            char hex[3] = {*(src + 1), *(src + 2), '\0'};
            *dest = (char) strtol(hex, NULL, 16);
            src += 2;
        } else {
            *dest = ch;
        }
        src++;
        dest++;
    }
    *dest = '\0';
}

// 提取参数函数
void get_param(const char *data, const char *key, char *value) {
    const char *start = strstr(data, key);
    if (start) {
        start += strlen(key) + 1; // 跳过"key="部分
        const char *end = strchr(start, '&');
        if (!end) {
            end = start + strlen(start);
        }
        strncpy(value, start, end - start);
        value[end - start] = '\0';
    } else {
        value[0] = '\0';
    }
}

int main(void) {
    // 获取环境变量 CONTENT_LENGTH，该变量表示POST数据的长度
    char *content_length_str = getenv("CONTENT_LENGTH");
    int content_length = content_length_str ? atoi(content_length_str) : 0;

    // 为POST数据分配缓冲区
    char input_buffer[MAX_BUFFER_SIZE];
    char decoded_buffer[MAX_BUFFER_SIZE];

    // 读取POST数据
    if (content_length > 0 && content_length < MAX_BUFFER_SIZE) {
        fread(input_buffer, 1, content_length, stdin);
        input_buffer[content_length] = '\0'; // 确保以空字符结尾
    } else {
        input_buffer[0] = '\0'; // 没有数据时以空字符开始
    }

    // URL解码POST数据
    url_decode(input_buffer, decoded_buffer);

    // 打印HTTP响应头
//    printf("Content-Type: text/html\n\n");

    // 提取各个参数
    char name[256], password[256], identity[256];
    get_param(decoded_buffer, "name", name);
    get_param(decoded_buffer, "password", password);
    get_param(decoded_buffer, "identity", identity);


    printf("Status: 303 See Other\r\n");
    printf("Location: http://localhost/welcome.html\r\n");
    printf("Content-Type: text/html\r\n\r\n");
//    printf("<html><body>\n");
//    printf("<h1>Hello, %s!</h1>\n", name);
//    printf("<p>password: %s</p>\n", password);
//    printf("<p>conheader: %s</p>\n",identity);
//    printf("</body></html>\n");
    FILE *standard ;
    standard = fopen("/Users/tokya_pt/Desktop/temp/www/cgi-bin/standard.txt", "w");
    fprintf(standard,"%s %s %s", identity,name,password);
    fclose(standard);
    system("/usr/bin/python3 login.py");
    sleep(1);
    FILE *return_data;
    return_data = fopen("/Users/tokya_pt/Desktop/temp/www/cgi-bin/standard.txt", "r");
    char line[1024];
    char *token =NULL;
    char result[10];
    while (fgets(line, sizeof(line), return_data) != NULL) {
           // 使用strtok分割字符串，这里使用空格作为分隔符
           token = strtok(line, " ");
           if (token == NULL) continue; // 如果没有token，跳过这行
        strcpy(result, token);
          
       }
    if (strcmp(result, "True")==0) {
        printf("Status: 303 See Other\r\n");
        printf("Location: http://localhost/welcome.html\r\n");
        printf("Content-Type: text/html\r\n\r\n");
    }
    else{
        printf("Content-Type: text/html\n\n");
        printf("<html><body>\n");
        printf("<h1>login fail</h1>\n");
        printf("</body></html>\n");
    }
    return 0;
}
