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
#include "string.h"

#define MAX_BUF_SIZE 1024
typedef long ssize_t;

// ELF文件的标志字节
#define ELF_MAGIC "\x7f\x45\x4c\x46"

int is_elf_file(const char *filename) {
    // 打开文件
    int fd = open(filename, O_RDWR);
    if (fd == -1) {
        return 0;  // 文件无法打开
    }

    // 检查文件的前4个字节
    char magic[4];
    ssize_t bytesRead = read(fd, magic, 4);
    close(fd);

    if (bytesRead == 4 && memcmp(magic, ELF_MAGIC, 4) == 0) {
        return 1;  // 是ELF文件
    }

    return 0;  // 不是ELF文件
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename_or_command> <new_content(optional)>\n", argv[0]);
        return 1;
    }

    const char *filenameOrCmd = argv[1];  // 文件名或命令
    const char *newContent = argc > 2 ? argv[2] : NULL;

    // 如果是带参数的可执行文件（ELF文件），执行它
    if (is_elf_file(filenameOrCmd)) {
        pid_t pid = fork();
        if (pid == -1) {
            printf("Failed to fork process.\n");
            return 1;
        } else if (pid == 0) {
            // 子进程执行命令
            execv(filenameOrCmd, argv + 1);  // 执行命令
            // 如果execv失败
            printf("Command execution failed for %s.\n", filenameOrCmd);
            return 1;
        } else {
            // 父进程等待子进程完成
            int status;
            wait(&status);
            if (status != 0) {
                printf("Command %s execution failed.\n", filenameOrCmd);
                return 1;
            }
            printf("Command %s executed successfully.\n", filenameOrCmd);
        }
    } else if (newContent != NULL) {
        // 处理文件编辑的功能
        int fd = open(filenameOrCmd, O_RDWR);
        if (fd == -1) {
            printf("The file %s does not exist or cannot be opened.\n", filenameOrCmd);
            return 1;
        }

        char buf[MAX_BUF_SIZE];
        ssize_t bytesRead = read(fd, buf, MAX_BUF_SIZE - 1);
        if (bytesRead == -1) {
            printf("Failed to read from %s.\n", filenameOrCmd);
            close(fd);
            return 1;
        }

        buf[bytesRead] = '\0';

        printf("Original content of %s:\n%s\n", filenameOrCmd, buf);

        close(fd);

        fd = open(filenameOrCmd, O_RDWR);
        if (fd == -1) {
            printf("Failed to reopen the file %s.\n", filenameOrCmd);
            return 1;
        }

        // ssize_t fileLength = bytesRead;

        char tempBuffer[MAX_BUF_SIZE];
        strcpy(tempBuffer, buf);
        strcat(tempBuffer, newContent);

        close(fd);

        fd = open(filenameOrCmd, O_RDWR);
        if (fd == -1) {
            printf("Failed to reopen the file %s.\n", filenameOrCmd);
            return 1;
        }

        ssize_t bytesWritten = write(fd, tempBuffer, strlen(tempBuffer));
        if (bytesWritten == -1) {
            printf("Failed to write to %s.\n", filenameOrCmd);
            close(fd);
            return 1;
        }

        close(fd);

        printf("Modified content of %s:\n%s\n", filenameOrCmd, tempBuffer);
    } else {
        // 处理命令执行的功能
        pid_t pid = fork();
        if (pid == -1) {
            printf("Failed to fork process.\n");
            return 1;
        } else if (pid == 0) {
            // 子进程执行命令
            execv(filenameOrCmd, argv + 1);  // 执行命令
            // 如果execv失败
            printf("Command execution failed for %s.\n", filenameOrCmd);
            return 1;
        } else {
            // 父进程等待子进程完成
            int status;
            wait(&status);
            if (status != 0) {
                printf("Command %s execution failed.\n", filenameOrCmd);
                return 1;
            }
            printf("Command %s executed successfully.\n", filenameOrCmd);
        }
    }

    return 0;
}

