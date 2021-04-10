#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
	char buf[128];
        int count = 0, childcount = 0;
	char *argv_cp[argc + 1];
	printf("RUN:");
	for(int i = 1; i < argc; i++)
	{
		argv_cp[i - 1] = argv[i];
		printf("%s", argv[i]);
	}
	printf("\n");
	argv_cp[argc - 1] = buf;
	argv_cp[argc] = 0;
	//char buf[128], count = 0;
	while(1)
	{
		char flag = read(0, buf + count, 1);
		if(buf[count] == ' ' || flag == 0)
		{
			buf[count] = 0;
			childcount ++;
			if(fork() == 0)
			{
				exec(argv_cp[0], argv_cp);
				exit(0);
			}
			if(flag == 0)
				break;
		}
		count++;
	}
	int stat;
	while(childcount--)
		wait(&stat);
	exit(0);
}
