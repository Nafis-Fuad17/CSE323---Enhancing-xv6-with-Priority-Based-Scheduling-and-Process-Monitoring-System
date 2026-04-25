// procinfo.h - Safe user-facing process info struct
// Feature 2: Process Monitoring System
// Md. Naimur Rahman (Nibir) - 2312011042
//
// Used by getprocs() to safely transfer process data
// from kernel space to user space. Unlike struct proc,
// this contains no kernel pointers or internal fields.

#ifndef PROCINFO_H
#define PROCINFO_H

struct procinfo {
  int pid;          // process ID
  int ppid;         // parent process ID
  char name[16];    // process name
  char state[10];   // RUNNING, SLEEPING, RUNNABLE, ZOMBIE
  int priority;     // scheduling priority (0-10)
  int cpu_ticks;    // total CPU ticks consumed
};

#endif
