#include "stdio.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Please provide a filename.\n");
        return 1;
    }

    struct stat buffer;
    // 使用 stat 函数检查文件是否存在
    if (stat(argv[1], &buffer) == -1) {
        printf("The file %s does not exist.\n", argv[1]);
        return 1; 
    }

    // 尝试删除文件
    if (unlink(argv[1]) == -1) {
        printf("Failed to remove file %s, please check your authority or try again later.\n", argv[1]);
        return -1;
    }

    // 删除成功，输出成功信息
    printf("Successfully removed file %s\n", argv[1]);
    return 0;
}



