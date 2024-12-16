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

typedef unsigned long ssize_t;

int main(int argc, char* argv[]) {
    // 判断是否提供了文件名和文本内容
    if (argc < 2) {
        printf("Please provide a filename.\n");
        return 1;
    }

    // 只提供文件名，创建文件
    if (argc == 2) {
        // 打开文件，若不存在则创建
        int fd = open(argv[1], O_CREAT | O_RDWR);
        if (fd == -1) {
            printf("Failed to create %s. Please check.\n", argv[1]);
            return 1;
        }
        printf("Successfully created %s.\n", argv[1]);
        close(fd);
    }
    // 提供了文件名和文本内容
    else if (argc == 3) {
        // 打开文件，若不存在则创建
        int fd = open(argv[1], O_CREAT | O_RDWR);
        if (fd == -1) {
            printf("Failed to create %s. Please check.\n", argv[1]);
            return 1;
        }

        // 获取文本内容并写入文件
        const char *content = argv[2];
        ssize_t written = write(fd, content, strlen(content));
        if (written == -1) {
            printf("Failed to write to %s.\n", argv[1]);
            close(fd);
            return 1;
        }

        printf("Successfully created %s and added content: %s\n", argv[1], content);
        close(fd);
    }
    // 输入参数过多的情况
    else {
        printf("Too many arguments. Please provide only filename and optional content.\n");
        return 1;
    }

    return 0;
}

