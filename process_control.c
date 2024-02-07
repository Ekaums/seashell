#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void process(char *arg){
    //char *token = NULL;
    int rc = fork();

    // while((token = strsep(&arg, " "))){
    //     // If this is the last token, remove the newline
    //     if(token[strlen(token) - 1] == '\n') token[strlen(token) - 1] = '\0';
    // }

    if (rc < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0){
        // Child
        if(strcmp(arg, "ls\n") == 0){
            if(access("/bin/ls", X_OK) == 0){
                char *args[2];
                // Don't want to insert a string literal into array since it is immutable. 
                // The pointer would point to read only section, which means you cant change args in here.
                args[0] = strdup("/bin/ls"); 
                args[1] = NULL;
                execv(args[0], args);
                printf("Success\n");
            }
            else{   printf("Error: ls is not a valid command");   }
            exit(0);
        }
        else{
            fprintf(stderr, "Error: Invalid command\n");
            exit(0);
        }
    }
    else{
        // Parent
        int rc_wait = wait(NULL);
        // printf("Parent PID: %d of child PID: %d\n", getpid(), rc_wait);
    }

    return;

}