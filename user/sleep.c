#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
	if( argc == 2)
	{
		sleep(atoi(argv[1]));
	}	
	else
	{
		char *echolist[] = {"More than one args"};
		exec("echo", echolist);
	}
	exit(0);
}
