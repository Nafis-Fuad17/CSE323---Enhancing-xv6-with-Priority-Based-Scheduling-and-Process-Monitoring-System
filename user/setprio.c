#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc != 3){
    fprintf(2, "Usage: setprio <pid> <priority (0-10)>\n");
    exit(1);
  }
  int pid = atoi(argv[1]);
  int pri = atoi(argv[2]);

  if(setpriority(pid, pri) < 0){
    fprintf(2, "setprio: failed (bad pid or priority out of range)\n");
    exit(1);
  }
  printf("setprio: pid %d priority set to %d\n", pid, pri);
  exit(0);
}
