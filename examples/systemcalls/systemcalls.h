#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
<<<<<<< HEAD
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<stdlib.h>
#include<syslog.h>
#include<string.h>
#include<errno.h>
=======
>>>>>>> assignments-base/assignment3-part-1

bool do_system(const char *command);

bool do_exec(int count, ...);

bool do_exec_redirect(const char *outputfile, int count, ...);
