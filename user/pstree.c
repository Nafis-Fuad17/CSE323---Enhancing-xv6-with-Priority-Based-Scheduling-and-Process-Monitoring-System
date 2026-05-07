#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/procinfo.h"
#include "user/user.h"

#define MAXPROCS 64

void
print_children(struct procinfo *procs, int n, int ppid, int depth)
{
  for(int i = 0; i < n; i++){
    if(procs[i].ppid == ppid){
      // print indentation
      for(int d = 0; d < depth; d++)
        printf("  ");
      printf("|-- [%d] %s (priority=%d)\n",
             procs[i].pid,
             procs[i].name,
             procs[i].priority);
      // recursively print children of this process
      print_children(procs, n, procs[i].pid, depth + 1);
    }
  }
}

int
main(void)
{
  struct procinfo procs[MAXPROCS];
  int n = getprocs(procs, MAXPROCS);

  if(n < 0){
    fprintf(2, "pstree: getprocs() failed\n");
    exit(1);
  }

  printf("=== Process Tree ===\n");
  printf("[0] kernel\n");
  print_children(procs, n, 0, 1);
  printf("\n");
  exit(0);
}
