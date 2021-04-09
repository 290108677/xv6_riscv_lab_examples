#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
	int pc[35][2];
	int childpid;
	char num[35] = {0};
	int max_num = 35;
	int count = 0;
	//printf("%d", sizeof(num));
top:
	//pipe(cn);
	pipe( pc[count] );
	//printf( "COUNT:%d\r\n", count );
	if( count )
		read( pc[count - 1][0], num, sizeof(num) );
	int firstnum, flag = 1;
	for( int i = 1; i < max_num; i++ )
	{
		if( num[i] == 0 && flag)
		{
			flag = 0;
			firstnum = i + 1;
			printf("Prime %d\r\n", firstnum);
			num[i] = 2;
		}
		else if( num[i] == 0 && !flag)
		{
			if( (i + 1) % firstnum == 0 )
			{
				num[i] = 1;
			}
		}
	}
	if( flag )
		exit(0);
	write( pc[count++][1], num, sizeof(num) );
	if( count >= max_num -1 )
		exit(0);
	childpid = fork();
	if( childpid == 0 )
		goto top;
	int stat;
	wait(&stat);
	exit(0);
}
