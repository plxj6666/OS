#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Usage: rm <filename>\n");
        return -1;
    }

    
    MESSAGE msg;
    msg.type = UNLINK;
    msg.PATHNAME = argv[1];
    msg.NAME_LEN = strlen(argv[1]);
    
    int result = send_recv(BOTH, TASK_FS, &msg);
    
    if (result != 0) {
        printf("System error when removing file %s\n", argv[1]);
        return -1;
    }
    
    if (msg.RETVAL != 0) {
        printf("File %s does not exist or cannot be removed\n", argv[1]);
        return -1;
    }

    printf("File %s removed.\n", argv[1]);
    return 0;
}


