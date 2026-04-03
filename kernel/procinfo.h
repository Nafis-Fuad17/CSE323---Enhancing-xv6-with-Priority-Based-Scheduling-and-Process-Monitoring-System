#ifndef PROCINFO_H
#define PROCINFO_H

struct procinfo {
  int pid;
  int ppid;
  char name[16];
  char state[10];
  int priority;
  int cpu_ticks;
};

#endif
