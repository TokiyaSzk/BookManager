#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>

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

    // 提取各个参数
    char name[256], password[256], identity[256];
    get_param(decoded_buffer, "name", name);
    get_param(decoded_buffer, "password", password);
    get_param(decoded_buffer, "identity", identity);
    FILE *standard ;
    standard = fopen("/Applications/AMPPS/www/cgi-bin/standard.txt", "w");
    fprintf(standard,"%s %s %s", identity,name,password);
    fclose(standard);
    // printf("Content-Type: text/html\n\n");
    // printf("<html><body>\n");
    // printf("<h1>login fail</h1>\n");
    // printf("</body></html>\n");
    FILE *fp;
    char path[1035];

    // 打开 Python 脚本文件
    fp = popen("/usr/bin/python3 /Applications/AMPPS/www/cgi-bin/login.py", "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        exit(1);
    }
    
    sleep(1);
    FILE *return_data;
    return_data = fopen("/Applications/AMPPS/www/cgi-bin/return_data.txt", "r");
    char line[1024];
    char *token =NULL;
    char result[10];
    while (fgets(line, sizeof(line), return_data) != NULL) {
           // 使用strtok分割字符串，这里使用空格作为分隔符
           token = strtok(line, " ");
           if (token == NULL) continue; // 如果没有token，跳过这行
        strcpy(result, token);
          
       }
    fclose(return_data);
    if (strcmp(result, "True")==0) {
        printf("Status:303 See Other\r\n");
        if (strcmp(identity,"login_admin")==0)
        {
           printf("Location: http://localhost/admin.html\r\n");
        }
        else{
            FILE *fp1;
            char path[1035];

            // 打开 Python 脚本文件
            fp1 = popen("/usr/bin/python3 /Applications/AMPPS/www/cgi-bin/search.py", "r");
            if (fp1 == NULL) {
                printf("Failed to run command\n" );
                exit(1);
            }
            //print data of reader with borrowlist
           printf("Location: http://localhost/cgi-bin/reader.cgi\r\n");
        }
        printf("Content-Type: text/html\r\n\r\n");
        printf("Referer: http://localhost/\r\n\r\n");
    }
    else{
        printf("Content-Type: text/html\n\n");
        printf("<html><body>\n");
        printf("<h1>Please retry</h1>\n");
        printf("<h1>%s</h1>\n",result);
        printf("</body></html>\n");
    }

}
