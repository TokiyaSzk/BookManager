//
//  main.c
//  test
//
//  Created by Tokya_mac on 2024/5/30.
//

//
//  library.c
//  library
//
//  Created by Tokya_mac on 2024/5/30.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_SIZE 100

// 静态链表节点结构体
typedef struct {
    char bid[10];
    char bname[30];
    char author[30];
    char company[20];
    int bnum;
    char locate[20];
    char inday[20];
    int outnum;
    int next;
} Node;

// 静态链表结构体
typedef struct {
    Node nodes[MAX_SIZE];
    int size;
} StaticLinkedList;

// 初始化静态链表
void initList(StaticLinkedList *list) {
    list->size = 0;
    for (int i = 0; i < MAX_SIZE - 1; i++) {
        list->nodes[i].next = i + 1;
    }
    list->nodes[MAX_SIZE - 1].next = -1; // 链表尾部节点的next设置为-1
}

// 在链表末尾添加节点
void append(StaticLinkedList *list, char bid[12],
            char bname[12],
            char author[10],
            char company[5],
            int bnum,
            char locate[11],
            char inday[12],
            int outnum) {
    if (list->size >= MAX_SIZE) {
        printf("List is full.\n");
        return;
    }
    int index = list->size;
    strcpy(list->nodes[index].bid, bid);
    strcpy(list->nodes[index].bname, bname);
    strcpy(list->nodes[index].author, author); // 修正此处，添加 author 字段的复制
    strcpy(list->nodes[index].company, company);
    list->nodes[index].bnum = bnum;
    strcpy(list->nodes[index].locate, locate);
    strcpy(list->nodes[index].inday, inday);
    list->nodes[index].outnum = outnum;
    list->nodes[index].next = -1; // 设置为链表尾部
    list->size++;
}

// 在链表中插入节点
void insert(StaticLinkedList *list, int data, int position) {
    if (position < 0 || position > list->size) {
        printf("Invalid position.\n");
        return;
    }
    if (list->size >= MAX_SIZE) {
        printf("List is full.\n");
        return;
    }
    int index = list->size;
    for (int i = list->size - 1; i >= position; i--) {
        list->nodes[i + 1] = list->nodes[i];
    }
    //list->nodes[position].data = data;
    list->nodes[position].next = list->nodes[position - 1].next;
    list->nodes[position - 1].next = position;
    list->size++;
}

// 删除链表中的节点
void node_remove(StaticLinkedList *list, int position) {
    if (position < 0 || position >= list->size) {
        printf("Invalid position.\n");
        return;
    }
    for (int i = position; i < list->size - 1; i++) {
        list->nodes[i] = list->nodes[i + 1];
    }
    list->size--;
}

// 查找链表中的节点
int node_find(StaticLinkedList *list, char search_bid[]) {
    for (int i = 0; i < list->size; i++) {
        if (strcmp(list->nodes[i].bid, search_bid)  == 0) {
            return i;
        }
    }
    return -1; // 数据未找到
}

// 打印链表
void printList(StaticLinkedList *list) {
    for (int i = 0; i < list->size; i++) {
        printf("书籍编号: %s\n", list->nodes[i].bid);
        printf("书名: %s\n", list->nodes[i].bname);
        printf("作者: %s\n", list->nodes[i].author);
        printf("出版社: %s\n", list->nodes[i].company);
        printf("数量: %d\n", list->nodes[i].bnum);
        printf("位置: %s\n", list->nodes[i].locate);
        printf("入库日期: %s\n", list->nodes[i].inday);
        printf("借出次数: %d\n", list->nodes[i].outnum);
        printf("\n");
    }
}
void printbook(StaticLinkedList *list,int i){
    printf("书籍编号: %s\n", list->nodes[i].bid);
    printf("书名: %s\n", list->nodes[i].bname);
    printf("作者: %s\n", list->nodes[i].author);
    printf("出版社: %s\n", list->nodes[i].company);
    printf("数量: %d\n", list->nodes[i].bnum);
    printf("位置: %s\n", list->nodes[i].locate);
    printf("入库日期: %s\n", list->nodes[i].inday);
    printf("借出次数: %d\n", list->nodes[i].outnum);
    printf("\n");
}
    
int main(void) {
    char bid[10];
    char bname[30];
    char author[30];
    char company[20];
    int bnum = 0;
    char locate[20];
    char inday[20];
    int outnum = 0;
    StaticLinkedList list;
    initList(&list);
    FILE *file;
    char line[1024]; // 假设每行数据不会超过1024个字符
    char *token = NULL;
    char *filename = "/Users/tokya_pt/Desktop/temp/www/cgi-bin/book.txt"; // 替换为你的文件名
    // 打开文件
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
        
    }

    // 逐行读取文件
    while (fgets(line, sizeof(line), file) != NULL) {
        // 使用strtok分割字符串，这里使用空格作为分隔符
        token = strtok(line, " ");
        if (token == NULL) continue; // 如果没有token，跳过这行

        // 假设每行数据格式是固定的，可以按顺序读取数据
        printf("ID: %s\n", token);
        strcpy(bid, token);
        token = strtok(NULL, " ");
        printf("Name: %s\n", token);
        strcpy(bname, token);
        token = strtok(NULL, " ");
        printf("Author: %s\n", token);
        strcpy(author, token);
        token = strtok(NULL, " ");
        printf("Publisher: %s\n", token);
        strcpy(company, token);
        token = strtok(NULL, " ");
        printf("Quantity: %s\n", token);
        bnum = atoi(token);
        token = strtok(NULL, " ");
        printf("Location: %s\n", token);
        strcpy(locate, token);
        token = strtok(NULL, " ");
        printf("Date: %s\n", token);
        strcpy(inday, token);
        token = strtok(NULL, " ");
        printf("Status: %s\n", token);
        outnum = atoi(token);
        printf("\n");
        append(&list, bid, bname, author, company, bnum, locate, inday, outnum);
    }

    // 关闭文件

    fclose(file);

    printList(&list);
    //above has finished list init

    //insert(&list, 15, 1);
    //printList(&list);

    //node_remove(&list, 1);
    //printList(&list);
    char search_bid[]="10001012";
    int position = node_find(&list, search_bid);
    printbook(&list, position);
    printf("%d",position);
    char exit[10];
    FILE *end;
    
    do {
        
        
        end = fopen("/Users/tokya_pt/Desktop/temp/www/cgi-bin/control.txt", "r");
        fgets(exit, 5, end);
        sleep(5);
        fclose(end);
    } while (strcmp(exit, "exit"));
    return 0;
}
