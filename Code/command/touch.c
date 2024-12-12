#include "stdio.h"

int main(int argc, char* argv[]) {
    //只输入touch的情况
    if (argc < 2) {
        printf("Please provide a filename.\n");
        return 1;
    }

if (~open(argv[1], O_CREAT)) {
 	printf("Successfully created %s.\n", argv[1]);
 } else {
 	printf("Failed to create %s, maybe because this file has been existed already. Please check.\n", argv[1]);
 }

    return 0;
}

