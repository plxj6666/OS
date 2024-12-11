#include "stdio.h"
#define FRAME_COUNT 20 

int main(int argc, char *argv[]) {
printf("===============================================================================\n");
	printf("        name                   |                      function                      \n");
	printf("===============================================================================\n");
	printf("        ls                     |       List all files in current file path\n");
	printf("        help                   |       List all commands\n");
	printf("        ps                     |       List all process's message\n");
	printf("        kill   [id]            |       kill a process with this pid\n");
	printf("        mkpro  [name]          |       folk and start a new process\n");
	printf("        touch  [name]          |       Create a directory\n");
	printf("        rm     [file]          |       Delete a file\n");
	printf("        delete [file]          |       Delete a directory\n");
	printf("        edit   [file][str]     |       Edit file, cover the content\n");
	printf("===============================================================================\n");

    return 0;
}

