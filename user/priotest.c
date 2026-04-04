#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int pid1, pid2, pid3;
  volatile long j;

  printf("=== Priority Scheduler Test ===\n");
  printf("Spawning 3 processes:\n");
  printf("  Child A -> priority 2 (LOW)\n");
  printf("  Child B -> priority 5 (MEDIUM)\n");
  printf("  Child C -> priority 9 (HIGH)\n\n");

  pid1 = fork();
  if(pid1 == 0){
    for(j = 0; j < 1000; j++){};
    volatile long i;
    for(i = 0; i < 100000000; i++){};
    printf("[LOW    priority=2] Child A done. PID=%d\n", getpid());
    exit(0);
  }

  pid2 = fork();
  if(pid2 == 0){
    for(j = 0; j < 1000; j++){};
    volatile long i;
    for(i = 0; i < 100000000; i++){};
    printf("[MEDIUM priority=5] Child B done. PID=%d\n", getpid());
    exit(0);
  }

  pid3 = fork();
  if(pid3 == 0){
    for(j = 0; j < 1000; j++){};
    volatile long i;
    for(i = 0; i < 100000000; i++){};
    printf("[HIGH   priority=9] Child C done. PID=%d\n", getpid());
    exit(0);
  }

  setpriority(pid1, 2);
  setpriority(pid2, 5);
  setpriority(pid3, 9);

  printf("Priorities set. Waiting for results...\n\n");

  wait(0);
  wait(0);
  wait(0);

  printf("Expected finish order: HIGH -> MEDIUM -> LOW\n");
  printf("=== Test Complete ===\n");
  exit(0);
}
