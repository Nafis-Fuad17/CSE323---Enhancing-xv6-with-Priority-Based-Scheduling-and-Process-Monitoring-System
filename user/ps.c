// ps.c - Process Monitor command for xv6
// Feature 2: Process Monitoring System
// Md. Naimur Rahman (Nibir) - 2312011042
//
// Calls getprocs() syscall to fetch live process data
// from the kernel and displays it as a formatted table.
// Also prints a summary of total processes and CPU ticks.

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/procinfo.h"
#include "user/user.h"

#define MAXPROCS 64

void
printpad(char *s, int width)
{
  int len = 0;
  while(s[len]) len++;
  printf("%s", s);
  while(len < width){ printf(" "); len++; }
}

void
printpadint(int n, int width)
{
  int tmp = n, digits = 1;
  if(tmp < 0){ digits++; tmp = -tmp; }
  while(tmp >= 10){ digits++; tmp /= 10; }
  printf("%d", n);
  while(digits < width){ printf(" "); digits++; }
}

int
main(void)
{
  struct procinfo procs[MAXPROCS];
  int n = getprocs(procs, MAXPROCS);

  if(n < 0){
    fprintf(2, "ps: getprocs() failed\n");
    exit(1);
  }

  // Header
  printpad("PID",      7);
  printpad("PPID",     7);
  printpad("STATE",    11);
  printpad("NAME",     13);
  printpad("PRIORITY", 11);
  printf("CPU_TICKS\n");
  printf("------------------------------------------------------\n");

  // Rows
  int total_ticks = 0;
  for(int i = 0; i < n; i++){
    printpadint(procs[i].pid,      7);
    printpadint(procs[i].ppid,     7);
    printpad(procs[i].state,       11);
    printpad(procs[i].name,        13);
    printpadint(procs[i].priority, 11);
    printf("%d\n", procs[i].cpu_ticks);
    total_ticks += procs[i].cpu_ticks;
  }

  // Summary line
  printf("------------------------------------------------------\n");
  printf("Total processes: %d  |  Total CPU ticks: %d\n",
         n, total_ticks);
  exit(0);
}
