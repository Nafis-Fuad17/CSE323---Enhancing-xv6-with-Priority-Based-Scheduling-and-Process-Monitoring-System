# xv6 — Priority Scheduling & Process Monitor

**Course:** CSE323 — Operating Systems  
**Group:** Scheduler Squad  
**Members:** Md. Nafis Fuad (2311934042) | Md. Naimur Rahman (2312011042)

---

## Overview

An extended xv6-riscv kernel implementing two new features:
1. **Priority-Based Process Scheduling** — replaces round-robin with a priority-aware scheduler
2. **Process Monitoring System** — a `ps` command that shows live process information

Both features are implemented via custom system calls.

---

## Feature 1 — Priority-Based Scheduler

The default xv6 round-robin scheduler was replaced with a priority-based one.

- Every process has a priority value from **0 (lowest) to 10 (highest)**, default **5**
- The scheduler always picks the highest-priority runnable process
- An **aging mechanism** prevents starvation — processes waiting 50+ ticks get +1 priority boost
- New system call: `setpriority(pid, priority)`

### Demo — priotest output (CPUS=1)
=== Priority Scheduler Test ===
Spawning 3 processes:
Child A -> priority 2 (LOW)
Child B -> priority 5 (MEDIUM)
Child C -> priority 9 (HIGH)
Priorities set. Waiting for results...
[HIGH   priority=9] Child C done. PID=6
[MEDIUM priority=5] Child B done. PID=5
[LOW    priority=2] Child A done. PID=4
Expected finish order: HIGH -> MEDIUM -> LOW
=== Test Complete ===

## Feature 2 — Process Monitoring System

A new `ps` command displays a live table of all active processes.

- New system call: `getprocs(struct procinfo *buf, int max)`
- Safely copies process data from kernel to user space via `copyout()`
- Displays: PID, PPID, STATE, NAME, PRIORITY, CPU_TICKS

### Demo — ps output
$ ps
PID PPID STATE      NAME       PRIORITY CPU_TICKS
1   0    sleep      init       5        27
2   1    sleep      sh         5        13
3   2    running    ps         5        6
$ setprio 1 9
setprio: pid 1 priority set to 9
$ ps
PID PPID STATE      NAME       PRIORITY CPU_TICKS
1   0    sleep      init       9        27
2   1    sleep      sh         5        21
5   2    running    ps         5        1

## Setup & Running
```bash
# Install dependencies
sudo apt install git build-essential qemu-system-riscv64 gcc-riscv64-linux-gnu

# Clone and build
git clone https://github.com/Nafis-Fuad17/CSE323---Enhancing-xv6-with-Priority-Based-Scheduling-and-Process-Monitoring-System.git
cd CSE323---Enhancing-xv6-with-Priority-Based-Scheduling-and-Process-Monitoring-System
make qemu

# For scheduler demo (single CPU)
make qemu CPUS=1
```

---

## Usage
```sh
# Inside xv6 shell

# Show all processes
$ ps

# Set priority of a process (0=lowest, 10=highest)
$ setprio <pid> <priority>

# Run priority scheduler demo
$ priotest
```

---

## Files Changed

| File | Change |
|------|--------|
| `kernel/proc.h` | Added `priority`, `wait_ticks`, `cpu_ticks` to `struct proc` |
| `kernel/procinfo.h` | New: safe user-facing `struct procinfo` |
| `kernel/proc.c` | Priority scheduler + aging in `scheduler()`, defaults in `allocproc()` |
| `kernel/sysproc.c` | Handlers for `sys_setpriority` and `sys_getprocs` |
| `kernel/syscall.h` | Syscall numbers 22 and 23 |
| `kernel/syscall.c` | Registered both new syscall handlers |
| `user/usys.pl` | Stubs for `setpriority` and `getprocs` |
| `user/user.h` | Declarations for both syscalls |
| `user/ps.c` | New: `ps` command |
| `user/setprio.c` | New: `setprio` command |
| `user/priotest.c` | New: priority scheduler demo/test |
| `Makefile` | Registered `ps`, `setprio`, `priotest` |
