#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<stdlib.h>
#include<syslog.h>
#include<errno.h>
#include<sys/stat.h>
#include<string.h>

void test_function_fork() {
    int k = fork();
    if(k == 0) {
        printf("child process\n");

    } else {
        printf("Parent process\n");

    }
}

int main() {
    char *strArr[] = {"echo","echo",NULL};
    int t;
    test_function_fork();
    printf("Hello there\n");
    return 0;
}