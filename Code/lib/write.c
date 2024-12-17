/*************************************************************************//**
 *****************************************************************************
 * @file   write.c
 * @brief  write()
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
#include <string.h>

// LFSR结构体定义
typedef struct {
    char state;    // 4位寄存器状态
    char length;   // 寄存器长度
} LFSR;



/*****************************************************************************
 *                                write
 *****************************************************************************/
/**
 * Write to a file descriptor.
 * 
 * @param fd     File descriptor.
 * @param buf    Buffer including the bytes to write.
 * @param count  How many bytes to write.
 * 
 * @return  On success, the number of bytes written are returned.
 *          On error, -1 is returned.
 *****************************************************************************/
PUBLIC int write(int fd, const void *buf, int count)
{
	MESSAGE msg;
	msg.type = WRITE;
	msg.FD   = fd;
	msg.BUF  = (void*)buf;
	msg.CNT  = count;

	send_recv(BOTH, TASK_FS, &msg);

	return msg.CNT;
}


PUBLIC int encrypt_write(int fd, const void *buf, int count )
{
    // 初始化LFSR
    LFSR lfsr;
	char init_state = 0x0B;
    init_lfsr(&lfsr, init_state);

    // 分配临时缓冲区存储加密后的数据
    char *encrypted_buf;
    
    int i;
    int bit ;
    // 对数据进行LFSR加密
    const char *input = (const char*)buf;
    for ( i = 0; i < count; i++) {
        char key_byte = 0;
        for ( bit = 0; bit < 8; bit++) {
            key_byte = (key_byte << 1) | next_bit(&lfsr);
        }
        encrypted_buf[i] = input[i] ^ key_byte;
    }

    // 构建消息
    MESSAGE msg;
    msg.type = WRITE;          // 假设WRITE是写操作的消息类型
    msg.FD   = fd;
    msg.BUF  = (void*)encrypted_buf;
    msg.CNT  = count;

    send_recv(BOTH, TASK_FS, &msg);


    return msg.CNT;
}