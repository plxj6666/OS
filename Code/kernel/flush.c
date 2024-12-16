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
    
    // 获取当前日志设置
    disable_int();
    int current_level = log_level;
    int current_categories = log_categories;
    enable_int();
    
    // 如果日志系统未启用，清空所有日志缓冲区并返回
    if (current_level == 0 || current_categories == 0) {
        // 清空所有缓冲区
        for (i = 0; i < MAX_SYSCALL_LOGS; i++) {
            syscall_logs[i].valid = 0;
        }
        for (i = 0; i < MAX_DEVICE_LOGS; i++) {
            device_logs[i].valid = 0;
        }
        for (i = 0; i < MAX_SWITCH_LOGS; i++) {
            switch_logs[i].from_pid = 0;
        }
        return;
    }
    
    // 一次只处理少量日志，避免消息堆积
    int max_logs_per_flush = 5;
    int processed = 0;
    
    // 处理进程切换日志
    if (processed < max_logs_per_flush && 
        current_level >= LOG_LEVEL_INFO && 
        (current_categories & LOG_CAT_PROCESS)) {
        for (i = 0; i < MAX_SWITCH_LOGS && processed < max_logs_per_flush; i++) {
            if (switch_logs[i].from_pid != 0) {  // 有效记录
                sprintf(buf, "Process switch: %s(PID:%d) -> %s(PID:%d)\n",
                        switch_logs[i].from_name, switch_logs[i].from_pid,
                        switch_logs[i].to_name, switch_logs[i].to_pid);
                syslog(LOG_LEVEL_INFO, LOG_CAT_PROCESS, buf);
                // 清空记录
                switch_logs[i].from_pid = 0;
                processed++;
            }
        }
    }
    
    // 处理系统调用日志
    if (processed < max_logs_per_flush && 
        current_level >= LOG_LEVEL_INFO && 
        (current_categories & LOG_CAT_SYSTEM)) {
        for (i = 0; i < MAX_SYSCALL_LOGS && processed < max_logs_per_flush; i++) {
            if (syscall_logs[i].valid) {
                if (syscall_logs[i].ret == 0) {
                    sprintf(buf, "System call from Process %s(PID:%d): %s\n",
                            syscall_logs[i].proc_name, syscall_logs[i].pid, 
                            syscall_logs[i].syscall_name);
                } else {
                    sprintf(buf, "System call failed from Process %s(PID:%d): %s, ret=%d\n",
                            syscall_logs[i].proc_name, syscall_logs[i].pid,
                            syscall_logs[i].syscall_name, syscall_logs[i].ret);
                }
                syslog(LOG_LEVEL_INFO, LOG_CAT_SYSTEM, buf);
                syscall_logs[i].valid = 0;
                processed++;
            }
        }
    }
    
    // 处理设备操作日志
    if (processed < max_logs_per_flush && 
        current_level >= LOG_LEVEL_INFO && 
        (current_categories & LOG_CAT_DEVICE)) {
        for (i = 0; i < MAX_DEVICE_LOGS && processed < max_logs_per_flush; i++) {
            if (device_logs[i].valid) {
                char* op_str;
                switch(device_logs[i].op_type) {
                    case DEV_OPEN:  op_str = "opened"; break;
                    case DEV_CLOSE: op_str = "closed"; break;
                    case DEV_READ:  op_str = "reading from"; break;
                    case DEV_WRITE: op_str = "writing to"; break;
                    case DEV_IOCTL:
                        sprintf(buf, "Process %s(PID:%d) IOCTL on HD partition %d, request: %d\n",
                                device_logs[i].proc_name, device_logs[i].pid, 
                                device_logs[i].device, device_logs[i].position);
                        break;
                    default: op_str = "unknown operation on"; break;
                }
                
                if (device_logs[i].op_type != DEV_IOCTL) {
                    sprintf(buf, "Process %s(PID:%d) %s HD partition %d\n",
                            device_logs[i].proc_name, device_logs[i].pid, 
                            op_str, device_logs[i].device);
                }
                
                syslog(LOG_LEVEL_INFO, LOG_CAT_DEVICE, buf);
                device_logs[i].valid = 0;
                processed++;
            }
        }
    }
}

PUBLIC void task_flush()
{
    while (!system_ready) {
    }
    while (1) {
        flush_logs();
        milli_delay(2000);    // 增加延迟到2000ms
    }
}