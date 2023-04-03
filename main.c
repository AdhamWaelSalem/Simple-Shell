#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <time.h>


FILE* logFile;

void child_terminated(int signum) {
    logFile = fopen("LogFile.txt","a");
    if (!logFile) {
        printf("Error in file!\n");
    } else {
        fprintf(logFile,"Child process was terminated \n");
        fclose(logFile);
    }
}

int main() {

    logFile = fopen("LogFile.txt","w");
    time_t s_t;
    time(&s_t);
    fprintf(logFile,"Shell Initiated at %s",ctime(&s_t));
    fclose(logFile);

    char command[1024];
    char* arg;
    char* args[32];

    while (1) {
        int i = 0;
        gets(command);
        arg = strtok(command, " ");
        while (arg) {
            args[i] = arg;
            arg = strtok(NULL, " ");
            i++;
        }
        args[i] =  NULL;
        if (strcmp(args[0],"exit") == 0){
            logFile = fopen("LogFile.txt","a");
            if (!logFile) {
                printf("Error in file!\n");
            } else {
                time_t e_t;
                time(&e_t);
                fprintf(logFile,"Shell Initiated at %s",ctime(&s_t));
                fclose(logFile);
            }
            exit(0);
        }
        if (strcmp(args[0],"cd") == 0){
            chdir(args[1]);
        }
        pid_t pid = fork();
        if (pid == 0) {
            int error = execvp(args[0], args);
            if (error < 0){
                printf("Error in command!!\n");
                exit(1);
            }
        }
        signal(SIGCHLD, child_terminated);
        if (strcmp(args[i-1],"&") != 0) {
            int status;
            waitpid(pid,&status,0);
        }
    }
    return 0;
}