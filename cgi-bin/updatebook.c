#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>

// 定义缓冲区大小
#define MAX_BUFFER_SIZE 1024
#define MAX_SIZE 100

typedef struct {
    char bid[10];
    char bname[30];
    char author[30];
    char company[20];
    char bnum[6];
    char locate[20];
    char inday[20];
    char outnum[6];
    int next;
} Node;
typedef struct {
    Node nodes[MAX_SIZE];
    int size;
} StaticLinkedList;
void initList(StaticLinkedList *list) {
    list->size = 0;
    for (int i = 0; i < MAX_SIZE - 1; i++) {
        list->nodes[i].next = i + 1;
    }
    list->nodes[MAX_SIZE - 1].next = -1; // 链表尾部节点的next设置为-1
}
void append(StaticLinkedList *list, char bid[12],
            char bname[12],
            char author[10],
            char company[5],
            char bnum[6],
            char locate[11],
            char inday[12],
            char outnum[6]) {
    if (list->size >= MAX_SIZE) {
        printf("List is full.\n");
        return;
    }
    int index = list->size;
    strcpy(list->nodes[index].bid, bid);
    strcpy(list->nodes[index].bname, bname);
    strcpy(list->nodes[index].author, author); // 修正此处，添加 author 字段的复制
    strcpy(list->nodes[index].company, company);
    strcpy(list->nodes[index].bnum,bnum);
    strcpy(list->nodes[index].locate, locate);
    strcpy(list->nodes[index].inday, inday);
    strcpy(list->nodes[index].outnum,outnum);
    list->nodes[index].next = -1; // 设置为链表尾部
    list->size++;
}
int node_find(StaticLinkedList *list, char search_bid[]) {
    for (int i = 0; i < list->size; i++) {
        if (strcmp(list->nodes[i].bname, search_bid)  == 0) {
            return i;
        }
    }
    return -1; // 数据未找到
}

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
    FILE *init;
    char path1[1035];

    // 打开 Python 脚本文件
    init = popen("/usr/bin/python3 /Applications/AMPPS/www/cgi-bin/init_book.py", "r");
    if (init == NULL) {
        printf("Failed to run command\n" );
        exit(1);
    }
    char bid[10];
    char bname[30];
    char author[30];
    char company[20];
    char bnum[6];
    char locate[20];
    char inday[20];
    char outnum[6];
    StaticLinkedList list;
    initList(&list);
    FILE *file;
    char line1[1024]; // 假设每行数据不会超过1024个字符
    char *token1 = NULL;
    char *filename = "/Applications/AMPPS/www/cgi-bin/book.txt"; // 替换为你的文件名
    // 打开文件
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
        
    }

    // 逐行读取文件
    while (fgets(line1, sizeof(line1), file) != NULL) {
        // 使用strtok分割字符串，这里使用空格作为分隔符
        token1 = strtok(line1, " ");
        if (token1 == NULL) continue; // 如果没有token，跳过这行

        // 假设每行数据格式是固定的，可以按顺序读取数据
        strcpy(bid, token1);
        token1 = strtok(NULL, " ");
        strcpy(bname, token1);
        token1 = strtok(NULL, " ");
        strcpy(author, token1);
        token1 = strtok(NULL, " ");
        strcpy(company, token1);
        token1 = strtok(NULL, " ");
        strcpy(bnum,token1);
        token1 = strtok(NULL, " ");
        strcpy(locate, token1);
        token1 = strtok(NULL, " ");
        strcpy(inday, token1);
        token1 = strtok(NULL, " ");
        strcpy(outnum,token1);
        append(&list, bid, bname, author, company, bnum, locate, inday, outnum);
    }
    
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
    char con_header[256],con_bname[256], con_author[256],con_company[256],con_bnum[256],con_locate[256],con_inday[256];
    get_param(decoded_buffer, "bname",con_bname);
    get_param(decoded_buffer, "author", con_author);
    get_param(decoded_buffer, "identity", con_header);
    get_param(decoded_buffer, "company", con_company);
    get_param(decoded_buffer, "bnum", con_bnum);
    get_param(decoded_buffer, "locate", con_locate);
    get_param(decoded_buffer, "inday", con_inday);

    FILE *standard;
    FILE *data;
    data = fopen("/Applications/AMPPS/www/cgi-bin/data.txt","w");
    standard = fopen("/Applications/AMPPS/www/cgi-bin/standard.txt", "w");
    fprintf(standard,"%s %s ",con_header,con_bname);
    if (strlen(con_author)!=0)
    {
        fprintf(standard,"author ");
        fprintf(data,"%s ",con_author);
    }
    if (strlen(con_bnum)!=0)
    {
        fprintf(standard,"bnum ");
        fprintf(data,"%s ",con_bnum);
    }
    if (strlen(con_company)!=0)
    {
        fprintf(standard,"company ");
        fprintf(data,"%s ",con_company);
    }
    if (strlen(con_locate)!=0)
    {
        fprintf(standard,"locate ");
        fprintf(data,"%s ",con_locate);
    }
    if (strlen(con_inday)!=0)
    {
        fprintf(standard,"indy ");
        fprintf(data,"%s ",con_inday);
    }
    fclose(data);
  
    
    fclose(standard);
    // printf("Content-Type: text/html\n\n");
    // printf("<html><body>\n");
    // printf("<h1>login fail</h1>\n");
    // printf("</body></html>\n");
    FILE *fp;
    char path[1035];

    //打开 Python 脚本文件
    fp = popen("/usr/bin/python3 /Applications/AMPPS/www/cgi-bin/insert_update_remove.py", "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        exit(1);
    }
    char line2[MAX_BUFFER_SIZE];
    while (fgets(line2, sizeof(line2), fp) != NULL) {
        // 检查输出中是否包含指示退出的信息，比如 "sys.exit()"
        if (strstr(line2, "sys.exit()") != NULL) {
            // 如果发现退出信息，关闭管道并退出循环
            pclose(fp);
            break;
        }
    }
    
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
        printf("Location: http://localhost/sucess.html\r\n");
        printf("Content-Type: text/html\r\n\r\n");
        printf("Referer: http://localhost/\r\n\r\n");
    }
    else{
        printf("Status:303 See Other\r\n");
        printf("Location: http://localhost/fail.html\r\n");
        printf("Content-Type: text/html\r\n\r\n");
        printf("Referer: http://localhost/\r\n\r\n");
    }

}
