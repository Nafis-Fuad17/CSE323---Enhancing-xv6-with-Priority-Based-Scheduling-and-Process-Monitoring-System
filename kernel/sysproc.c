#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"
#include "procinfo.h"

extern struct proc proc[];

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
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
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if(t == SBRK_EAGER || n < 0) {
    if(growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if(addr + n < addr)
      return -1;
    if(addr + n > TRAPFRAME)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
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

  argint(0, &pid);
  return kkill(pid);
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
// setpriority(int pid, int priority)
// Sets the priority of process <pid> to <priority> (0-10).
// Returns 0 on success, -1 if pid not found or priority out of range.
uint64
sys_setpriority(void)
{
  int pid, priority;
  argint(0, &pid);
  argint(1, &priority);

  if(priority < 0 || priority > 10)
    return -1;

  struct proc *p;
  for(p = proc; p < &proc[NPROC]; p++){
    acquire(&p->lock);
    if(p->pid == pid){
      p->priority = priority;
      release(&p->lock);
      return 0;
    }
    release(&p->lock);
  }
  return -1;  // pid not found
}

// getprocs(struct procinfo *buf, int max)
// Fills buf with info about up to <max> active processes.
// Returns the number of entries written.
uint64
sys_getprocs(void)
{
  uint64 addr;
  int max;
  argaddr(0, &addr);
  argint(1, &max);

  struct proc *p;
  struct procinfo info;
  int count = 0;

  // State names matching the procstate enum
  static char *states[] = {
    [UNUSED]    "unused",
    [USED]      "used",
    [SLEEPING]  "sleep",
    [RUNNABLE]  "runnable",
    [RUNNING]   "running",
    [ZOMBIE]    "zombie",
  };

  for(p = proc; p < &proc[NPROC] && count < max; p++){
    acquire(&p->lock);
    if(p->state != UNUSED && p->state != USED){
      info.pid      = p->pid;
      info.ppid     = p->parent ? p->parent->pid : 0;
      info.priority = p->priority;
      info.cpu_ticks = p->cpu_ticks;
      safestrcpy(info.name, p->name, sizeof(info.name));
      safestrcpy(info.state, states[p->state], sizeof(info.state));

      // Copy this entry to user space
      if(copyout(myproc()->pagetable, addr + count * sizeof(info),
                 (char*)&info, sizeof(info)) < 0){
        release(&p->lock);
        break;
      }
      count++;
    }
    release(&p->lock);
  }
  return count;
}
