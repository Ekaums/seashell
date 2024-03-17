#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process_control.h"
#include "input_parse.h"

int main(void){
    char *buffer = NULL;
    size_t size = 0;

    do{
        printf("sea> ");

        ssize_t input = getline(&buffer, &size, stdin); // Get variable sized input from user

        if (input == -1){
            printf("ERROR\n");
            return -1;
        }

        char *arg = buffer; // Intermediate string to parse tokens, don't want to modify pointer of original buffer
        arg = trim_whitespace(arg);
        process(arg);

    }while((strcmp(buffer, "exit\n")));
    
    free(buffer);
    return 0;

}
