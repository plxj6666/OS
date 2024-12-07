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

#endif