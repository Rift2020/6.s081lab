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
    backtrace();
  int n;
  uint ticks0;

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
uint64 sys_sigalarm(void){
    int interval;
    uint64 func;
    struct proc *p = myproc();

    if(argint(0,&interval)<0|| argaddr(1,&func)<0)
        return -1;
    p->interval=interval;
    p->func=func;
    //p->func();
    return 0;
}
uint64 sys_sigreturn(void){
    struct proc *p = myproc();
    p->trapframe->epc=p->epc;
    p->trapframe->sp=p->sp;
    p->trapframe->s0=p->fp;
    p->trapframe->ra=p->ra;
    p->trapframe->a0=p->a0;
    p->trapframe->a1=p->a1;
    p->trapframe->a2=p->a2;
    p->trapframe->a3=p->a3;
    p->trapframe->a4=p->a4;
    p->trapframe->a5=p->a5;
    p->trapframe->a6=p->a6;
    p->trapframe->a7=p->a7;
    p->ready=1;
    return 0;
}
