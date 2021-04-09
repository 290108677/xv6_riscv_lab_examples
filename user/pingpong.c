#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
	int cp[2], pc[2], pid, childpid;
	char *pingpong[] = {"ping", "pong"};
	char buf[10];

	pipe(cp);
	pipe(pc);
	childpid = fork();
	pid = getpid();
	if( childpid == 0 )
	{
		write(cp[1], pingpong[0], strlen(pingpong[0]));
		read(pc[0], buf, 4);
		printf("child,receive:%s\r\n", buf);
		printf("This is child,pid:%d\r\n", pid);
		exit(0);
	}
	else
	{
		int stat;
		//wait(&stat);
		write(pc[1], pingpong[1], strlen(pingpong[1]));
		read(cp[0], buf, 4);
		wait(&stat);
		printf("parent,receive:%s\r\n", buf);
		//wait(&stat);
		printf("This is parent,pid:%d\r\n", pid);
	}
	exit(0);
}
