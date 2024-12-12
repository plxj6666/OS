/*************************************************************************//**
 *****************************************************************************
 * @file   syslog.c
 * @brief  
 * @author Forrest Y. Yu
 * @date   Thu Nov 20 17:02:42 2008
 *****************************************************************************
 *****************************************************************************/

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
#include "stdarg.h"
/*****************************************************************************
 *                                syslog
 *****************************************************************************/
/**
 * Write log directly to the disk by sending message to FS.
 * 
 * @param fmt The format string.
 * 
 * @return How many chars have been printed.
 *****************************************************************************/
PUBLIC int syslog(int level, int category, const char *fmt, ...)
{
    if (!system_ready) return 0;
    
    char buf[256];
    va_list arg;
    va_start(arg, fmt);
    vsprintf(buf, fmt, arg);
    va_end(arg);

    disable_int();
    int current_level = log_level;
    int current_categories = log_categories;
    enable_int();

    // 如果不满足日志级别和类别要求,直接返回
    if (level > current_level || 
        !(current_categories & category)) {
        return 0;
    }

    // 构造消息
    MESSAGE msg;
    struct log_msg log;
    
    log.level = level;
    log.category = category;
    strcpy(log.content, buf);
    msg.type = LOG_MESSAGE;
    msg.BUF = &log;
    
    // 发送消息到日志任务
    send_recv(BOTH, TASK_LOG, &msg);
    
    return msg.RETVAL;
}

PUBLIC void enable_log_level(int level)
{
    disable_int();
    log_level = level;
    enable_int();
}

PUBLIC void disable_log_level(int level)
{
    disable_int();
    if (log_level == level) {
        log_level = 0;  // 或者设置为更低级别
    }
    enable_int();
}

PUBLIC void enable_log_category(int category)
{
    disable_int();
    log_categories |= category;
    enable_int();
}

PUBLIC void disable_log_category(int category)
{
    disable_int();
    log_categories &= ~category;
    enable_int();
}

/* 日志级别转字符串 */
PUBLIC const char* get_log_level_str(int level)
{
    switch(level) {
        case LOG_LEVEL_ERROR: return "ERROR";
        case LOG_LEVEL_WARN:  return "WARN";
        case LOG_LEVEL_INFO:  return "INFO";
        case LOG_LEVEL_DEBUG: return "DEBUG";
        case LOG_LEVEL_TRACE: return "TRACE";
        default:              return "UNKNOWN";
    }
}

/* 日志类别转字符串 */
PUBLIC const char* get_log_category_str(int category)
{
    switch(category) {
        case LOG_CAT_ERROR:   return "ERROR";
        case LOG_CAT_SYSTEM:  return "SYSTEM";
        case LOG_CAT_PROCESS: return "PROCESS";
        case LOG_CAT_MEMORY:  return "MEMORY";
        case LOG_CAT_FS:      return "FS";
        case LOG_CAT_DEVICE:  return "DEVICE";
        default:              return "UNKNOWN";
    }
}

