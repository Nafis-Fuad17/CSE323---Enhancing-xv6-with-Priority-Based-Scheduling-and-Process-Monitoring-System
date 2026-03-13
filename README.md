xv6 — Priority Scheduling & Process Monitor

An extended xv6 kernel with two new features: a priority-based CPU scheduler and a ps-style process monitor. Both are exposed via custom system calls.

Features

1 · Priority-based scheduler

Replaces the default round-robin scheduler with a priority-aware one. Every process has a numeric priority (0–10, default 5). The scheduler always picks the highest-priority runnable process. An aging mechanism prevents starvation — processes that wait too long get a gradual priority boost.


2 · Process monitor (ps)

A getprocs() system call copies live process data from the kernel to user space. A companion ps shell command prints a formatted table of all active processes — PID, name, state, priority, and CPU ticks consumed.

