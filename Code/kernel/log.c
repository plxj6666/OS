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
        disable_int();  // 关中断保护临界区
        int current_level = log_level;
        int current_categories = log_categories;
        enable_int();
        
        send_recv(RECEIVE, ANY, &msg);
        
        if (msg.type == LOG_MESSAGE) {
            struct log_msg* p = (struct log_msg*)va2la(msg.source, msg.BUF);
            int result = 0;
            
            // 使用之前保存的值进行检查
            if (p->level <= current_level && 
                (current_categories & p->category)) {
                result = disklog(p->content);
            }
            
            // 立即回复消息
            msg.RETVAL = result;
            send_recv(SEND, msg.source, &msg);
        }
    }
}
