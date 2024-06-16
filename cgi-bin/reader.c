#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    
    printf("Content-Type: text/html; charset=utf-8\n\n");
    printf("<html><body>\n");
    FILE *file;
    char line[1024]; // 假设每行数据不会超过1024个字符
    char *token = NULL;
    char *filename = "/Applications/AMPPS/www/cgi-bin/data.txt"; // 替换为你的文件名
    // 打开文件
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    
    }
    fgets(line, sizeof(line), file);
    token = strtok(line, " ");
    printf("读者id:%s ",token);
    token = strtok(NULL," ");
    printf("读者姓名:%s ",token);
    token = strtok(NULL," ");
    printf("剩余借阅数目:%s ",token);
    token = strtok(NULL," ");
    printf("已借阅数目:%s ",token);
    printf("<h1>以上是读者信息</h1>");
    // 逐行读取文件
    while (fgets(line, sizeof(line), file) != NULL) {
        // 使用strtok分割字符串，这里使用空格作为分隔符
        token = strtok(line," ");
        if (token == NULL) continue; // 如果没有token，跳过这行
        // 假设每行数据格式是固定的，可以按顺序读取数据
        printf("借阅书id:%s ",token);
        token = strtok(NULL," ");
        printf("借阅人id:%s ",token);
        token = strtok(NULL," ");
        printf("书名：%s ",token);
        token = strtok(NULL," ");
        printf("借阅日期:%s ",token);
        token = strtok(NULL," ");
        printf("归还日期：%s ",token);
        printf("<br><br>");
    }
    printf("<form action='/cgi-bin/exit.py'>\n");
    printf("<button type='submit'>退出登陆</button>\n");
    printf("</form>\n");
    printf("</body></html>\n");
}