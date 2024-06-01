#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    FILE *file;
    char line[1024]; // 假设每行数据不会超过1024个字符
    char *token;
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
        token = strtok(NULL, " ");
        printf("Name: %s\n", token);
        token = strtok(NULL, " ");
        printf("Author: %s\n", token);
        token = strtok(NULL, " ");
        printf("Publisher: %s\n", token);
        token = strtok(NULL, " ");
        printf("Quantity: %s\n", token);
        token = strtok(NULL, " ");
        printf("Location: %s\n", token);
        token = strtok(NULL, " ");
        printf("Date: %s\n", token);
        token = strtok(NULL, " ");
        printf("Status: %s\n", token);
        printf("\n");
    }

    // 关闭文件
    fclose(file);

    return EXIT_SUCCESS;
}
