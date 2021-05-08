#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
void error(char* str)
{
		perror(str);
		exit(EXIT_FAILURE);
}
void main()
{
	exit(1);
}
