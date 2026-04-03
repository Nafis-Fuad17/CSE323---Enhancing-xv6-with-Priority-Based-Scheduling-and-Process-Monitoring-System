#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/procinfo.h"
#include "user/user.h"


#define MAXPROCS 64
int
main(void)
{
  struct procinfo procs[MAXPROCS];
  int n = getprocs(procs, MAXPROCS);

  if(n < 0){
    fprintf(2, "ps: getprocs failed\n");
    exit(1);
  }

  // Header
  printf("PID PPID STATE NAME PRIORITY CPU_TICKS\n");

  for(int i = 0; i < n; i++){
    printf("%d %d %s %s %d %d\n",
           procs[i].pid,
           procs[i].ppid,
           procs[i].state,
           procs[i].name,
           procs[i].priority,
           procs[i].cpu_ticks);
  }
  exit(0);
}
