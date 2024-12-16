#include "stdio.h"

int main(int argc, char *argv[]) {
    printf(
        "===============================================================================\n"
        "        name                   |                      function                      \n"
        "===============================================================================\n"
        "        ls                     |       List all files in current file path\n"
        "        help                   |       List all commands\n"
        "        ps                     |       List all process's message\n"
        "        kill   [id]            |       kill a process with this pid\n"
        "        touch  [name][str]     |       Create a file\n"
        "        rm     [file]          |       Delete a file\n");
 printf(
        "        echo   [str]           |       print a str\n"
        "        pwd                    |       print  /\/ \n"
        "        read   [file]          |       read a file content\n"
        "        edit   [file][str]     |       Edit file, add the content,if ELF,execute it\n"
        "        cls                    |       Clear the screen\n"
        "        ikun                   |       output an easter egg\n"
        "===============================================================================\n"
    );

    return 0;
}