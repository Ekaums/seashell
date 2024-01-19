#include <stdio.h>
#include <string.h>

int main(void){
    char input[256]; // Currently, only 256 chars can be stored

    do{
        printf("sea> ");

        if (fgets(input, sizeof(input), stdin) == NULL)
            return 1;

    }while(!(strcmp(input, "exit\n") == 0));

}
