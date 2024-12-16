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

int main(int argc, char* argv[])
{
    MESSAGE msg;
    
    // 打开根目录
    msg.type = OPEN;
    msg.PATHNAME = "/";
    msg.FLAGS = O_RDWR;   // 以读写方式打开
    msg.NAME_LEN = 1;     // 路径长度为1 ('/')
    
    // 打开根目录
    int result = send_recv(BOTH, TASK_FS, &msg);
    if (result != 0) {
        printf("Failed to open root directory.\n");
        return -1;
    }
    
    int fd = msg.FD;  // 获取文件描述符
    
    // 读取目录内容
    char buf[SECTOR_SIZE];
    msg.type = READ;
    msg.FD = fd;
    msg.BUF = buf;
    msg.CNT = sizeof(buf);
    msg.POSITION = 0;  // 从头开始读
    
    result = send_recv(BOTH, TASK_FS, &msg);
    if (result != 0 || msg.CNT <= 0) {
        printf("Failed to read directory entries.\n");
        return -1;
    }
    
    // 遍历并打印文件名
    struct dir_entry* pde = (struct dir_entry*)buf;
    int nr_dir_entries = msg.CNT / DIR_ENTRY_SIZE;
    
    for (int i = 0; i < nr_dir_entries; i++, pde++) {
        if (pde->inode_nr != 0) {  // 有效的目录项
            printf("%s\n", pde->name);
        }
    }
    
    // 关闭文件
    msg.type = CLOSE;
    msg.FD = fd;
    send_recv(BOTH, TASK_FS, &msg);
    
    return 0;
}
