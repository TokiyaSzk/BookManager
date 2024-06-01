//
//  main.c
//  postdata
//
//  Created by Tokya_mac on 2024/6/1.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    printf("Content-Type: text/html\n\n");

    // 提取各个参数
    char name[256], age[256], email[256];
    get_param(decoded_buffer, "name", name);
    get_param(decoded_buffer, "age", age);
    get_param(decoded_buffer, "email", email);

    // 打印响应HTML
    printf("<html><body>\n");
    printf("<h1>Hello, %s!</h1>\n", name[0] ? name : "World");
    printf("<p>Age: %s</p>\n", age[0] ? age : "Unknown");
    printf("<p>Email: %s</p>\n", email[0] ? email : "Unknown");
    printf("</body></html>\n");
    FILE *standard ;
    standard = fopen("/Users/tokya_pt/Desktop/temp/www/cgi-bin/standard.txt", "w");
    fprintf(standard,"%s %s %s", name,age,email);
    fclose(standard);
    return 0;
}
