#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
  {
    //if(myproc()->trace_flag)
    //	printf("%d\n", -1);
    return -1;
  }
  myproc()->trace_flag = 0;
  exit(n);
  //printf("%d\n", 0);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  //if(myproc()->trace_flag)
//	  printf("%d\n", myproc()->pid);
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  int rt = fork();
  //printf("[[[[[[[[[[[[[[[[[[[[[[[[[[[[");
  //if(myproc()->trace_flag)
  //	printf("%d\n", rt);
  return rt;
}

uint64
sys_wait(void)
{
  uint64 p, rt;
  if(argaddr(0, &p) < 0)
  {
    //if(myproc()->trace_flag)
    //	printf("%d\n", -1);
    return -1;
  }
  rt = wait(p);
  //if(myproc()->trace_flag)
  //	printf("%d\n", rt);
  return rt;
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
  {
    //if(myproc()->trace_flag)
    //	printf("%d\n", -1);
    return -1;
  }
  addr = myproc()->sz;
  if(growproc(n) < 0)
  {
    //if(myproc()->trace_flag)
    //	printf("%d\n", -1);
    return -1;
  }
  //if(myproc()->trace_flag)
  //	printf("%d\n", addr);
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
  {
    //if(myproc()->trace_flag)
    //	printf("%d\n", -1);
    return -1;
  }
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      //if(myproc()->trace_flag)
      //	printf("%d\n", -1);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  //if(myproc()->trace_flag)
  //	printf("%d\n", 0);
  return 0;
}

uint64
sys_kill(void)
{
  int pid, rt;

  if(argint(0, &pid) < 0)
  {
    //if(myproc()->trace_flag)
    //	printf("%d\n", -1);
    return -1;
  }
  rt = kill(pid);
  //if(myproc()->trace_flag)
  //	printf("%d\n", rt);
  return rt;
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  //if(myproc()->trace_flag)
  //	printf("%d\n", xticks);
  return xticks;
}

uint64
sys_trace(void)
{
	struct proc *p = myproc();
	p->trace_flag = 1;
	int arg_trace_id;
	argint(0, &arg_trace_id);
	p->trace_id = arg_trace_id;
	//printf("%d", arg_trace_id);
	return 0;
}
