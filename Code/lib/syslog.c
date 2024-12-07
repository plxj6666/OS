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
    struct log_msg msg;
    msg.level = level;
    msg.category = category;
    
    va_list arg = (va_list)((char*)(&fmt) + 4);
    vsprintf(msg.content, fmt, arg);
    
    MESSAGE m;
    m.type = LOG_MESSAGE;
    m.BUF = &msg;
    
    return send_recv(BOTH, TASK_LOG, &m);
}

PUBLIC void set_log_level(int level)
{
    log_level = level;
}

PUBLIC void set_log_categories(int categories) 
{
    log_categories = categories;
}

PUBLIC void enable_log_category(int category)
{
    log_categories |= category;
}

PUBLIC void disable_log_category(int category)
{
    log_categories &= ~category;
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

