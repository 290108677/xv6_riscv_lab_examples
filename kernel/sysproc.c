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
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  backtrace();

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
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
  return xticks;
}

uint64
sys_sigalarm(void)
{
  int tick;
  argint(0, &tick);
  void (*handler)();
  uint64 fun;
  argaddr(1, &fun);
  handler = (void (*)())fun;
  //handler();
  // printf("Get tick %d, handler %p\n", tick, fun);


  
  struct proc *p = myproc();
  if(p->flag != 0)
    return 0;

  if(tick == 0 && fun == 0){
    p->flag = 0;
    return 0;
  }
  tick += 1;
  p->ticks_b = tick;
  p->ticks = tick;
  p->handler = handler;
  p->flag = 1;

  //w_sepc(p->trapframe->epc);
  return 0;
}

uint64
sys_sigreturn(void)
{
  struct proc *p = myproc();
  // usertrapret();
  p->ticks = p->ticks_b;
  p->flag = 2;
  // printf("Get tick %d\n", p->ticks);
  return 0;
}