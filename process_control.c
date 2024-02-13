#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

void process(char *arg){

    if(!strcmp(arg, "exit\n")) return; // Exit command

    int rc = fork();

    if (rc < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if(rc == 0){
        // Child

        // TODO: Initalize args function to abstract this

        if(arg[strlen(arg) - 1] == '\n') arg[strlen(arg) - 1] = '\0';   // Remove newline
        size_t index = 0;
        size_t capacity = 2; // Starts at 2 to have arg + NULL

        char **args = (char**)malloc(sizeof(char *) * capacity); // Cannot use char *args[]. This defines a static-sized array which cannot be malloced
        if(!args) assert(args);

        char *token;

        // Parse arguments, adding them to args array
        while(token = strsep(&arg, " ")){
            // Ensure first command is valid
            if(index == 0){
                args[index] = strdup("/bin/"); 
                strcat(args[index], token);

                if(access(args[0], X_OK) != 0){ 
                    printf("Invalid Command\n");
                    exit(-1);
                }
                index += 1;
                continue;
            }
            // Double size of array if more args
            if (index+1 >= capacity){
                capacity *= 2; 
                args = realloc(args, capacity * sizeof(char *));
                if(!args) assert(args);
            }
            args[index++] = token;
        }

        args[index] = NULL; // Add NULL before calling execv
        execv(args[0], args);

    }
    else{
        // Parent
        int rc_wait = wait(NULL);
        printf("Done\n");
    }

    return;

}