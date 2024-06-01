//
//  init.c
//  library
//
//  Created by Tokya_mac on 2024/5/30.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main( ){
    char command[50];
    
    strcpy(command,"/usr/bin/python3 init_book.py");
    system(command);
    return 0;
}
