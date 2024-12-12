/*****************************************************************************
*                                shabby_shell
*****************************************************************************/
/**
* A very powerful shell.
*
* @param tty_name  TTY file name.
// *****************************************************************************/
void shabby_shell(const char * tty_name)
{    
    int fd_stdin = open(tty_name, O_RDWR);
    assert(fd_stdin == 0);
    int fd_stdout = open(tty_name, O_RDWR);
    assert(fd_stdout == 1);

    char rdbuf[128];
    char filepath[MAX_FILENAME_LEN];
    
    colorful();
    clear();
    welcome();

    initFS();

    while (1) {
        write(1, "$ ", 2);  // 提示符
        int r = read(0, rdbuf, 70);  // 读取用户输入
        rdbuf[r] = 0;

        int argc = 0;
        char * argv[PROC_ORIGIN_STACK];
        char * p = rdbuf;
        char * s;
        int word = 0;
        char ch;
        do {
            ch = *p;
            if (*p != ' ' && *p != 0 && !word) {
                s = p;
                word = 1;
            }
            if ((*p == ' ' || *p == 0) && word) {
                word = 0;
                argv[argc++] = s;
                *p = 0;
            }
            p++;
        } while(ch);
        argv[argc] = 0;
        
        int argv1[PROC_ORIGIN_STACK];
        int argv2[PROC_ORIGIN_STACK];
        int argc1 = 0;
        int argc2 = 0;
        int mul_task = 0;
        int i;
        for (i = 0; i < argc; i++) {
            if (!mul_task) {
                argv1[i] = argv[i];
                argc1++;
                if(strcmp(argv[i], "&") == 0) {
                    argc1--;
                    mul_task = 1;
                }
            } else {
                argc2++;
                argv2[i - 1 - argc1] = argv[i];
            }
        }
        
        argv1[argc1] = 0;
        argv2[argc2] = 0;
        
        
        int fd1 = open(argv1[0], O_RDWR);
        int fd2 = 1;
        
        if (mul_task) {
            fd2 = open(argv2[0], O_RDWR);
        }

        if (fd1 == -1 || fd2 == -1) {
            if (rdbuf[0]) {
                write(1, "{", 1);
                write(1, rdbuf, r);
                write(1, "}\n", 2);
            }
        } else {
            close(fd1);
            if (mul_task) {
                close(fd2);
            }

            int pid1 = fork();
            int pid2 = -1;        
            if(pid1 != 0 && mul_task) {
                pid2 = fork();
            }
    
            if(pid1 != 0 && pid2 !=0) {
                // 父进程等待子进程完成
                int s1;
                wait(&s1);
                if (mul_task) {
                    wait(&s1);  // 等待第二个子进程
                }
            } else if (pid1 == 0) {
                // 子进程1
                if(mul_task) {
                    printf("[pid: %d]command: %s\n", getpid(), argv1[0]);
                }
                execv(argv1[0], argv1);
            } else if (pid2 != -1) {
                // 子进程2
                printf("[pid: %d]command: %s\n", getpid(), argv2[0]);
                execv(argv2[0], argv2);
            }
        }
    }
    
    close(1);
    close(0);
}

