#include "stdio.h"

#define MAX_BUF_SIZE 1024


int main(int argc, char *argv[]) {
    // 判断是否提供了文件名
    if (argc < 2) {
        printf("Please provide a filename.\n");
        return 1;
    }

    // 尝试打开文件，使用 O_RDWR（读写模式）
    int fd = open(argv[1], O_RDWR);  // 打开文件，读写模式
    if (fd == -1) {
        // 如果无法打开文件，输出错误信息
        printf("Failed to open %s. The file might not exist or you do not have permission.\n", argv[1]);
        return 1;
    }

    // 文件成功打开，读取内容
    char buf[MAX_BUF_SIZE];
    int bytesRead = decrypt_read(fd, buf, MAX_BUF_SIZE);  // 使用 read 系统调用读取文件内容
    if (bytesRead == -1) {
        // 如果读取失败，输出错误信息
        printf("Failed to read from %s.\n", argv[1]);
        close(fd);
        return 1;
    }

    // 输出读取到的内容
    printf("Content of %s:\n", argv[1]);
    write(1, buf, bytesRead);  // 输出读取的内容到标准输出

    // 关闭文件描述符
    close(fd);


    return 0;
}

