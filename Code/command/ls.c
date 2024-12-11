#include "stdio.h"

int main(int argc, char* argv[])
{
	int fd = -1;
	char bufr[1024];

	fd = open(location, O_RDWR);

	if (fd == -1)
	{
		printf("Error opening file\n");
		return;
	}

	read(fd, bufr, 1024);
	printf("%s\n", bufr);
	close(fd);
}
