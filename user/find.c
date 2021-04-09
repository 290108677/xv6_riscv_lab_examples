#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char *path, char *filename)
{
	char buf[128];
	memmove(buf, path, strlen(path) + 1);
	int fd, fdt;
	struct stat st, stt;
	struct dirent de;
	if((fd = open(path, 0)) < 0)
	{
		printf("Can not open file : %s", path);
		return;
	}
	if((fstat(fd, &st)) < 0)
	{
		printf("Can not get file stat : %s", path);
		close(fd);
		return;
	}
	if(st.type == T_FILE)
	{
		//printf("%s %d %d %l\n", path, st.type, st.ino, st.size);
		printf("This is the file not directory");
	}
	else if(st.type == T_DIR)
	{
		while(read(fd, &de, sizeof(de)) == sizeof(de))
		{
			if(de.inum == 0)
				continue;
			buf[strlen(path)] = '/';
			memmove(buf + strlen(path) + 1, de.name, strlen(de.name) + 1);
			if((fdt = open(buf, 0)) < 0)
				printf("Testing Open %s Failed", buf);
			if(fstat(fdt, &stt) < 0)
			{
				printf("Testing Get Status %s Failed", buf);
				close(fdt);
			}
			if(stt.type == T_FILE)
			{
				if(!strcmp(de.name, filename))
				{
					printf("F ");
					printf("%s\n", buf);
				}
			}
			else if(stt.type == T_DIR)
			{
				//printf("D ");
				//printf("%s\n", buf);
				if(strcmp(&buf[strlen(buf) - 2], "/.") && strcmp(&buf[strlen(buf) - 3], "/.."))
					find(buf, filename);
			}
			else
			{
				//printf("N ");
				//printf("%s\n", buf);
			}
			close(fdt);
		}
	}
	close(fd);
	return;

}
	
int
main(int argc, char *argv[])
{
	if(argc > 2)
	{
		for(int i = 2; i < argc; i++)
		{
			find(argv[1], argv[i]);
		}
	}
	else
	{
		printf("find [dir] [name1] [name2]\r\n");
	}
	//find(".");
	exit(0);	
}
