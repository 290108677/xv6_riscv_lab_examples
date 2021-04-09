#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
	int stat, childpid, count = 0;
	//childpid = fork();
top:
	count ++;
	if( count == 100)
		exit(0);
	childpid = fork();
	if( childpid == 0)
		goto top;
	wait(&stat);
	printf("COUNT:%d\r\n", count);
	exit(0);
}
