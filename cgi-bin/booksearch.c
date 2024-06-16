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
void printList(StaticLinkedList *list) {
    for (int i = 0; i < list->size; i++) {
        printf("书籍编号: %s\n", list->nodes[i].bid);
        printf("书名: %s\n", list->nodes[i].bname);
        printf("作者: %s\n", list->nodes[i].author);
        printf("出版社: %s\n", list->nodes[i].company);
        printf("数量: %s\n", list->nodes[i].bnum);
        printf("位置: %s\n", list->nodes[i].locate);
        printf("入库日期: %s\n", list->nodes[i].inday);
        printf("借出次数: %s\n", list->nodes[i].outnum);
        printf("<h2></h2>");
    }
}
void printbook(StaticLinkedList *list,int i){
    printf("书籍编号: %s\n", list->nodes[i].bid);
    printf("书名: %s\n", list->nodes[i].bname);
    printf("作者: %s\n", list->nodes[i].author);
    printf("出版社: %s\n", list->nodes[i].company);
    printf("数量: %s\n", list->nodes[i].bnum);
    printf("位置: %s\n", list->nodes[i].locate);
    printf("入库日期: %s\n", list->nodes[i].inday);
    printf("借出次数: %s\n", list->nodes[i].outnum);
    printf("<h2></h2>");
}
void node_find(StaticLinkedList *list, char search_son[],char con_search[]) 
{
    if (strcmp(con_search,"bname")==0)
    {
       for (int i = 0; i < list->size; i++) {
            if (strcmp(list->nodes[i].bname, search_son)  == 0) {
                printbook(list,i);
            }
        }
    }
    if (strcmp(con_search,"bid")==0)
    {
        for (int i = 0; i < list->size; i++) {
            if (strcmp(list->nodes[i].bid, search_son)  == 0) {
                printbook(list,i);
            }
        }
    }
    if (strcmp(con_search,"author"))
    {
        for (int i = 0; i < list->size; i++) {
            if (strcmp(list->nodes[i].author, search_son)  == 0) {
                printbook(list,i);
            }
        }
    }
    if (strcmp(con_search,"company")==0)
    {
       for (int i = 0; i < list->size; i++) {
            if (strcmp(list->nodes[i].company, search_son)  == 0) {
                printbook(list,i);
            }
        }
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
    char  index[256],con_header[256],data[256];
    get_param(decoded_buffer, "identity", con_header);
    get_param(decoded_buffer, "index", index);
    get_param(decoded_buffer, "data", data);
    
    printf("Content-Type: text/html; charset=utf-8\n\n");
    printf("<html><body>\n");
    printf("<a href='/borrow.html' style='color: red;'>借阅</a>");
    printf("<h2></h2>");
    if (strcmp(con_header,"all")==0)
    {
        
        printList(&list);
        
    }
    else{
        if (strcmp(index,"author")==0)
        {
           node_find(&list,data,"author");
        }
        if (strcmp(index,"bid")==0)
        {
            node_find(&list,data,"bid");
        }
        if (strcmp(index,"company")==0)
        {
            node_find(&list,data,"company");
        }
        if (strcmp(index,"bname")==0)
        {
            node_find(&list,data,"bname");
        }
    }
    printf("</body></html>\n");
    
    
   
    
}
