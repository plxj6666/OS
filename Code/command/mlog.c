#include "type.h"
#include "const.h"
#include "protect.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "proto.h"
#include "stdio.h"
#include "syslog.h"

void print_menu() {
    printf("\nLog Management Menu:\n");
    printf("1. Enable log level\n");
    printf("2. Disable log level\n");
    printf("3. Enable log category\n");
    printf("4. Disable log category\n");
    printf("5. Exit\n");
    printf("\nPlease enter your choice (1-5): ");
}

void print_levels() {
    printf("\nAvailable Log Levels:\n");
    printf("1. ERROR (Level 1)\n");
    printf("2. WARN  (Level 2)\n");
    printf("3. INFO  (Level 3)\n");
    printf("4. DEBUG (Level 4)\n");
    printf("5. TRACE (Level 5)\n");
    printf("\nPlease enter level number (1-5): ");
}

void print_categories() {
    printf("\nAvailable Log Categories:\n");
    printf("1. ERROR   - Error logs\n");
    printf("2. SYSTEM  - System related logs\n");
    printf("3. PROCESS - Process related logs\n");
    printf("4. MEMORY  - Memory related logs\n");
    printf("5. FS      - File system logs\n");
    printf("6. DEVICE  - Device related logs\n");
    printf("\nPlease enter category number (1-6): ");
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
            // 退出前禁用所有日志类别和级别
            int current_level = 5;  // 从最高级别开始禁用
            while (current_level > 0) {
                manage_log(2, current_level);  // 禁用每个级别
                current_level--;
            }
            
            // 禁用所有类别
            for (int cat = 1; cat <= 6; cat++) {
                manage_log(4, cat);  // 禁用每个类别
            }
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
