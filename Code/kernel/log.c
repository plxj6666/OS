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
#include "log.h"

PUBLIC void task_log()
{
    while (1) {
        MESSAGE msg;
        send_recv(RECEIVE, ANY, &msg);
        
        if (msg.type == LOG_MESSAGE) {
            struct log_msg* p = (struct log_msg*)va2la(msg.source, msg.BUF);
            
            /* 检查日志级别和类别 */
            if (p->level <= log_level && (log_categories & p->category)) {
                printf("log have been written\n");
                disklog(p->content);
            }
        }
    }
}
