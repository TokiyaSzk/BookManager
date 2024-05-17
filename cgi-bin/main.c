//
//  main.c
//  test
//
//  Created by Tokya_mac on 2024/5/16.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to read POST data
void read_post_data() {
    char *lenstr;
    char *post_data;
    long len;

    // Get the content length
    lenstr = getenv("CONTENT_LENGTH");
    if (lenstr != NULL) {
        len = strtol(lenstr, NULL, 10);

        // Allocate memory to hold the POST data
        post_data = (char *)malloc(len + 1);
        if (post_data == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            return;
        }

        // Read the POST data
        fread(post_data, 1, len, stdin);
        post_data[len] = '\0';  // Null-terminate the data

        // Print the POST data
        printf("Content-type: text/plain\n\n");
        printf("POST data: %s\n", post_data);

        // Free the allocated memory
        free(post_data);
    } else {
        printf("Content-type: text/plain\n\n");
        printf("No POST data received\n");
    }
}

int main(void) {
    // Print the HTTP headers
    printf("Content-type: text/plain\n\n");

    // Read and print the POST data
    read_post_data();

    return 0;
}
