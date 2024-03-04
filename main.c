#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process_control.h"

int main(void){
    char *buffer = NULL;
    char *arg = NULL;
    size_t size = 0;

    do{
        printf("sea> ");

        ssize_t input = getline(&buffer, &size, stdin); // Get variable sized input from user

        if (input == -1){
            printf("ERROR\n");
            return -1;
        }

        // Intermediate buffer to parse tokens, sets to NULL at the end of string
        // Don't want to modify pointer of original buffer
        arg = buffer;
        if(arg[strlen(arg) - 1] == '\n') arg[strlen(arg) - 1] = '\0';   // Remove newline
        // TODO: Formatting input (stripping whitespace)
        process(arg);

        // Function to check if using path command

    }while((strcmp(buffer, "exit\n")));
    
    free(buffer);
    return 0;

}
