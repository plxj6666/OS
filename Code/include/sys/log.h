#ifndef _ORANGES_LOG_H_
#define _ORANGES_LOG_H_

#define TASK_LOG    5  /* 修改为正确的任务号 */

/* 日志消息结构 */
struct log_msg {
    int level;
    int category; 
    char content[256];
};

/* 函数声明 */
PUBLIC void task_log();


// 缓冲区记录日志类型
// 进程切换日志 ，最多记录128条
#define MAX_SWITCH_LOGS 128

struct proc_switch_log {
    char from_name[16];
    int from_pid;
    char to_name[16];
    int to_pid;
};

EXTERN struct proc_switch_log switch_logs[MAX_SWITCH_LOGS];
EXTERN int switch_log_index;

// 系统调用日志，最多记录128条
#define MAX_SYSCALL_LOGS 128

struct syscall_log {
    char proc_name[16];    // 进程名
    int pid;               // 进程ID
    char syscall_name[16]; // 系统调用名
    int ret;              // 返回值
    int valid;            // 是否有效
};

EXTERN struct syscall_log syscall_logs[MAX_SYSCALL_LOGS];
EXTERN int syscall_log_index;

// 设备操作日志结构
struct device_op_log {
    char proc_name[32];    // 进程名
    int pid;               // 进程ID
    int device;           // 设备号
    int op_type;          // 操作类型(DEV_OPEN/DEV_CLOSE/DEV_READ/DEV_WRITE)
    int position;         // 读写位置
    int size;            // 读写大小
    int valid;           // 是否有效
};

#define MAX_DEVICE_LOGS 128
extern struct device_op_log device_logs[MAX_DEVICE_LOGS];
extern int device_log_index;



#endif