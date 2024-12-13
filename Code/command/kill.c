#include "type.h"
#include "stdio.h"
#include "string.h"
#include "sys/const.h"
#include "sys/protect.h"
#include "sys/fs.h"
#include "sys/proc.h"
#include "sys/tty.h"
#include "sys/console.h"
#include "sys/global.h"
#include "sys/proto.h"


int my_atoi(const char* str) {
    int result = 0;
    int sign = 1;

    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }

    return sign * result;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: kill <pid>\n");
        return -1;
    }
    int pid = my_atoi(argv[1]);


    MESSAGE msg;
    msg.PID = pid;
    msg.type = KILL_PROC; 
    send_recv(BOTH, TASK_SYS, &msg);
    printf("Process %d killed.\n", pid);
    return 0;
}
