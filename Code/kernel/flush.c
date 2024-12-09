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

PUBLIC void task_flush()
{
    while (!system_ready) {
    }
    while (1) {
        flush_switch_logs();  // 刷新日志
        printl("flush_switch_logs\n");
        milli_delay(200);    // 每200ms刷新一次
    }
}

PUBLIC void flush_switch_logs()
{
    char buf[256];
    int i;
    
    // 添加调试信息
    printl("Current process: %s(PID:%d)\n", p_proc_ready->name, proc2pid(p_proc_ready));
    printl("TASK_LOG: %d\n", TASK_LOG);
    
    for (i = 0; i < MAX_SWITCH_LOGS; i++) {
        struct proc_switch_log* log = &switch_logs[i];
        if (log->from_pid != 0) {  // 有效记录
            sprintf(buf, "Process switch: %s(PID:%d) -> %s(PID:%d)\n",
                    log->from_name, log->from_pid,
                    log->to_name, log->to_pid);
            syslog(LOG_LEVEL_DEBUG, LOG_CAT_PROCESS, buf);
            
            // 清空记录
            log->from_pid = 0;
        }
    }
}