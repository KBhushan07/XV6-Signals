#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "signal.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}


int
sys_sigkill(void)
{
  int pid , signo;
  if(argint(0, &pid) < 0 || argint(1,&signo) < 0){
    return -1;
  }
    sigkill(pid,signo);
    return 0;
}

int
sys_signal(void)
{
  int signo = 0;
  sighandler_t handler;
  char *ptr;
  if(argint(0, &signo) < 0 || argptr(1, &ptr, sizeof(sighandler_t)) < 0){
    return -1;
  }
  handler = (sighandler_t)ptr;
  signal(signo, handler);
  return 0;
}


int
sys_sigprocmask(void)
{
  int how;
  argint(0, &how);
  int which;
  argint(1, &which);
  sigprocmask(how, which);
  return 0;
}

int
sys_sigpause(void)
{
  pause();
  return 0;
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
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

int
sys_sleep(void)
{
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

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
