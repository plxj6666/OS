/*************************************************************************//**
 *****************************************************************************
 * @file   read.c
 * @brief  read()
 * @author Forrest Y. Yu
 * @date   2008
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


// LFSR结构体定义
typedef struct {
    char state;    // 4位寄存器状态
    char length;   // 寄存器长度
} LFSR;

// 初始化LFSR
void init_lfsr(LFSR* lfsr, char init_state) {
    lfsr->state = init_state & 0x0F;  // 确保只取4位
    lfsr->length = 4;
}

// 生成下一个比特
char next_bit(LFSR* lfsr) {
    // 基于本原多项式 x^4 + x + 1 实现
    char new_bit = ((lfsr->state >> 0) ^ (lfsr->state >> 3)) & 1;
    lfsr->state = ((lfsr->state << 1) | new_bit) & 0x0F;
    return new_bit;
}


/*****************************************************************************
 *                                read
 *****************************************************************************/
/**
 * Read from a file descriptor.
 * 
 * @param fd     File descriptor.
 * @param buf    Buffer to accept the bytes read.
 * @param count  How many bytes to read.
 * 
 * @return  On success, the number of bytes read are returned.
 *          On error, -1 is returned.
 *****************************************************************************/

PUBLIC int read(int fd, void *buf, int count)
{
	MESSAGE msg;
	msg.type = READ;
	msg.FD   = fd;
	msg.BUF  = buf;
	msg.CNT  = count;

	send_recv(BOTH, TASK_FS, &msg);

	return msg.CNT;
}



PUBLIC int decrypt_read(int fd, void *buf, int count)
{
	char init_state = 0x0B;
    // 初始化LFSR
    LFSR lfsr;
    init_lfsr(&lfsr, init_state);

    // 构建消息以读取加密数据
    MESSAGE msg;
    msg.type = READ;          // 假设READ是读操作的消息类型
    msg.FD   = fd;
    msg.BUF  = buf;
    msg.CNT  = count;

    // 发送消息并接收响应
    if (send_recv(BOTH, TASK_FS, &msg) != 0) {
        // 发送或接收失败
        return -1;
    }

    // 检查文件系统任务的返回值
    if (msg.CNT < 0) {
        // 文件系统任务返回错误
        return -1;
    }

    int bytes_read = msg.CNT;

    // 解密读取到的数据
    char *data = (char*)buf;
    int i;
    int bit;
    for (i = 0; i < bytes_read; i++) {
        char key_byte = 0;
        for (bit = 0; bit < 8; bit++) {
            key_byte = (key_byte << 1) | next_bit(&lfsr);
        }
        data[i] ^= key_byte;
    }

    return bytes_read;
}
