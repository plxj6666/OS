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
#include "syslog.h"

PUBLIC void flush_logs()
{
    char buf[256];
    int i;
    
    // 刷新进程切换日志
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
        else {
            break;
        }
    }
    
    // 刷新系统调用日志
    for (i = 0; i < MAX_SYSCALL_LOGS; i++) {
        struct syscall_log* log = &syscall_logs[i];
        if (log->valid) {  // 有效记录
            if (log->ret == 0) {
                sprintf(buf, "System call from Process %s(PID:%d): %s\n",
                        log->proc_name, log->pid, log->syscall_name);
            } else {
                sprintf(buf, "System call failed from Process %s(PID:%d): %s, ret=%d\n",
                        log->proc_name, log->pid, log->syscall_name, log->ret);
            }
            syslog(LOG_LEVEL_DEBUG, LOG_CAT_SYSTEM, buf);
            
            // 清空记录
            log->valid = 0;
        }
        else {
            break;
        }
    }
}

PUBLIC void task_flush()
{
    while (!system_ready) {
    }
    while (1) {
        flush_logs();  // 刷新日志
        printl("flush_logs\n");
        milli_delay(200);    // 每200ms刷新一次
    }
}