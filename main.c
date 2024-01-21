#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
    char *buffer = NULL;
    char *midbuffer = NULL;
    char *token = NULL;
    size_t size = 0;

    do{
        printf("sea> ");

        ssize_t input = getline(&buffer, &size, stdin); // Get variable sized input from user

        if (input == -1){
            printf("ERROR\n");
            return 1;
        }

        // Intermediate buffer to parse tokens, sets to NULL at the end of string
        // (don't want to modify pointer of original buffer)
        midbuffer = buffer;
        while((token = strsep(&midbuffer, " "))){
            printf("%s", token);
        }

    }while((strcmp(buffer, "exit\n")));
    
    free(buffer);
    return 0;

}
