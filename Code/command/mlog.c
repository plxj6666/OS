#include "type.h"
#include "const.h"
#include "protect.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "proto.h"
#include "stdio.h"
#include "syslog.h"

void print_to_console(const char* str) {
    // 获取当前控制台
    int fd = 1;  // stdout
    write(fd, str, strlen(str));
}

void print_menu() {
    print_to_console("\nLog Management Menu:\n");
    print_to_console("1. Enable log level\n");
    print_to_console("2. Disable log level\n");
    print_to_console("3. Enable log category\n");
    print_to_console("4. Disable log category\n");
    print_to_console("5. Exit\n");
    print_to_console("\nPlease enter your choice (1-5): ");
}

void print_levels() {
    print_to_console("\nAvailable Log Levels:\n");
    print_to_console("1. ERROR (Level 1)\n");
    print_to_console("2. WARN  (Level 2)\n");
    print_to_console("3. INFO  (Level 3)\n");
    print_to_console("4. DEBUG (Level 4)\n");
    print_to_console("5. TRACE (Level 5)\n");
    print_to_console("\nPlease enter level number (1-5): ");
}

void print_categories() {
    print_to_console("\nAvailable Log Categories:\n");
    print_to_console("1. ERROR   - Error logs\n");
    print_to_console("2. SYSTEM  - System related logs\n");
    print_to_console("3. PROCESS - Process related logs\n");
    print_to_console("4. MEMORY  - Memory related logs\n");
    print_to_console("5. FS      - File system logs\n");
    print_to_console("6. DEVICE  - Device related logs\n");
    print_to_console("\nPlease enter category number (1-6): ");
}

int main(int argc, char* argv[])
{
    char buf[256];
    int choice;
    
    while(1) {
        print_menu();
        int r = read(0, buf, 256);
        if(r <= 0) continue;
        choice = buf[0] - '0';
        
        if(choice == 5) {
            // 直接调用功能号5来禁用所有日志并清空缓冲区
            manage_log(5, 0);  // param参数在这里不重要
            break;
        }
        
        switch(choice) {
            case 1: // Enable level
                print_levels();
                r = read(0, buf, 256);
                if(r > 0) {
                    int level = buf[0] - '0';
                    if(level >= 1 && level <= 5) {
                        manage_log(1, level);
                        printf("Log level %d enabled\n", level);
                    }
                }
                break;
                
            case 2: // Disable level
                print_levels();
                r = read(0, buf, 256);
                if(r > 0) {
                    int level = buf[0] - '0';
                    if(level >= 1 && level <= 5) {
                        manage_log(2, level);
                        printf("Log level %d disabled\n", level);
                    }
                }
                break;
                
            case 3: // Enable category
                print_categories();
                r = read(0, buf, 256);
                if(r > 0) {
                    int cat = buf[0] - '0';
                    if(cat >= 1 && cat <= 6) {
                        manage_log(3, cat);
                        printf("Log category %d enabled\n", cat);
                    }
                }
                break;
                
            case 4: // Disable category
                print_categories();
                r = read(0, buf, 256);
                if(r > 0) {
                    int cat = buf[0] - '0';
                    if(cat >= 1 && cat <= 6) {
                        manage_log(4, cat);
                        printf("Log category %d disabled\n", cat);
                    }
                }
                break;
        }
    }
    
    return 0;
}
