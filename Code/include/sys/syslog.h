#ifndef _ORANGES_SYSLOG_H_
#define _ORANGES_SYSLOG_H_

/* 日志级别和类别的字符串转换函数 */
PUBLIC const char* get_log_level_str(int level);
PUBLIC const char* get_log_category_str(int category);

/* syslog函数声明 */
PUBLIC int syslog(int level, int category, const char *fmt, ...);

/* 日志控制函数 */
PUBLIC void set_log_level(int level);
PUBLIC void set_log_categories(int categories);
PUBLIC void enable_log_category(int category);
PUBLIC void disable_log_category(int category);

#endif /* _ORANGES_SYSLOG_H_ */