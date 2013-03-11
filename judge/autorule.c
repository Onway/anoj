/*
 * -*- coding: utf-8 -*-
 * Filename      : autorule.c
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-11
 */

/*
 * reference to:
 * kill -l
 * /usr/include/i386-linux-gnu/bits/syscall.h
 */

#include "autorule.h"

extern int signal_rule[];
extern int syscall_rule[];
extern GSList * resource_rule;
extern GSList * environ_rule;

void
auto_signal_rule(const char * key, int value)
{   
    if (!strcmp(key, "SIGHUP"))
        signal_rule[SIGHUP] = value;
    else if (!strcmp(key, "SIGINT"))
        signal_rule[SIGINT] = value;
    else if (!strcmp(key, "SIGQUIT"))
        signal_rule[SIGQUIT] = value;
    else if (!strcmp(key, "SIGILL"))
        signal_rule[SIGILL] = value;
    else if (!strcmp(key, "SIGTRAP"))
        signal_rule[SIGTRAP] = value;
    else if (!strcmp(key, "SIGABRT"))
        signal_rule[SIGABRT] = value;
    else if (!strcmp(key, "SIGBUS"))
        signal_rule[SIGBUS] = value;
    else if (!strcmp(key, "SIGFPE"))
        signal_rule[SIGFPE] = value;
    else if (!strcmp(key, "SIGKILL"))
        signal_rule[SIGKILL] = value;
    else if (!strcmp(key, "SIGUSR1"))
        signal_rule[SIGUSR1] = value;
    else if (!strcmp(key, "SIGSEGV"))
        signal_rule[SIGSEGV] = value;
    else if (!strcmp(key, "SIGUSR2"))
        signal_rule[SIGUSR2] = value;
    else if (!strcmp(key, "SIGPIPE"))
        signal_rule[SIGPIPE] = value;
    else if (!strcmp(key, "SIGALRM"))
        signal_rule[SIGALRM] = value;
    else if (!strcmp(key, "SIGTERM"))
        signal_rule[SIGTERM] = value;
    else if (!strcmp(key, "SIGSTKFLT"))
        signal_rule[SIGSTKFLT] = value;
    else if (!strcmp(key, "SIGCHLD"))
        signal_rule[SIGCHLD] = value;
    else if (!strcmp(key, "SIGCONT"))
        signal_rule[SIGCONT] = value;
    else if (!strcmp(key, "SIGSTOP"))
        signal_rule[SIGSTOP] = value;
    else if (!strcmp(key, "SIGTSTP"))
        signal_rule[SIGTSTP] = value;
    else if (!strcmp(key, "SIGTTIN"))
        signal_rule[SIGTTIN] = value;
    else if (!strcmp(key, "SIGTTOU"))
        signal_rule[SIGTTOU] = value;
    else if (!strcmp(key, "SIGURG"))
        signal_rule[SIGURG] = value;
    else if (!strcmp(key, "SIGXCPU"))
        signal_rule[SIGXCPU] = value;
    else if (!strcmp(key, "SIGXFSZ"))
        signal_rule[SIGXFSZ] = value;
    else if (!strcmp(key, "SIGVTALRM"))
        signal_rule[SIGVTALRM] = value;
    else if (!strcmp(key, "SIGPROF"))
        signal_rule[SIGPROF] = value;
    else if (!strcmp(key, "SIGWINCH"))
        signal_rule[SIGWINCH] = value;
    else if (!strcmp(key, "SIGIO"))
        signal_rule[SIGIO] = value;
    else if (!strcmp(key, "SIGPWR"))
        signal_rule[SIGPWR] = value;
    else if (!strcmp(key, "SIGSYS"))
        signal_rule[SIGSYS] = value;
    else if (!strcmp(key, "SIGRTMIN"))
        signal_rule[SIGRTMIN] = value;
    else if (!strcmp(key, "SIGRTMIN+1"))
        signal_rule[SIGRTMIN+1] = value;
    else if (!strcmp(key, "SIGRTMIN+2"))
        signal_rule[SIGRTMIN+2] = value;
    else if (!strcmp(key, "SIGRTMIN+3"))
        signal_rule[SIGRTMIN+3] = value;
    else if (!strcmp(key, "SIGRTMIN+4"))
        signal_rule[SIGRTMIN+4] = value;
    else if (!strcmp(key, "SIGRTMIN+5"))
        signal_rule[SIGRTMIN+5] = value;
    else if (!strcmp(key, "SIGRTMIN+6"))
        signal_rule[SIGRTMIN+6] = value;
    else if (!strcmp(key, "SIGRTMIN+7"))
        signal_rule[SIGRTMIN+7] = value;
    else if (!strcmp(key, "SIGRTMIN+8"))
        signal_rule[SIGRTMIN+8] = value;
    else if (!strcmp(key, "SIGRTMIN+9"))
        signal_rule[SIGRTMIN+9] = value;
    else if (!strcmp(key, "SIGRTMIN+10"))
        signal_rule[SIGRTMIN+10] = value;
    else if (!strcmp(key, "SIGRTMIN+11"))
        signal_rule[SIGRTMIN+11] = value;
    else if (!strcmp(key, "SIGRTMIN+12"))
        signal_rule[SIGRTMIN+12] = value;
    else if (!strcmp(key, "SIGRTMIN+13"))
        signal_rule[SIGRTMIN+13] = value;
    else if (!strcmp(key, "SIGRTMIN+14"))
        signal_rule[SIGRTMIN+14] = value;
    else if (!strcmp(key, "SIGRTMIN+15"))
        signal_rule[SIGRTMIN+15] = value;
    else if (!strcmp(key, "SIGRTMAX-14"))
        signal_rule[SIGRTMAX-14] = value;
    else if (!strcmp(key, "SIGRTMAX-13"))
        signal_rule[SIGRTMAX-13] = value;
    else if (!strcmp(key, "SIGRTMAX-12"))
        signal_rule[SIGRTMAX-12] = value;
    else if (!strcmp(key, "SIGRTMAX-11"))
        signal_rule[SIGRTMAX-11] = value;
    else if (!strcmp(key, "SIGRTMAX-10"))
        signal_rule[SIGRTMAX-10] = value;
    else if (!strcmp(key, "SIGRTMAX-9"))
        signal_rule[SIGRTMAX-9] = value;
    else if (!strcmp(key, "SIGRTMAX-8"))
        signal_rule[SIGRTMAX-8] = value;
    else if (!strcmp(key, "SIGRTMAX-7"))
        signal_rule[SIGRTMAX-7] = value;
    else if (!strcmp(key, "SIGRTMAX-6"))
        signal_rule[SIGRTMAX-6] = value;
    else if (!strcmp(key, "SIGRTMAX-5"))
        signal_rule[SIGRTMAX-5] = value;
    else if (!strcmp(key, "SIGRTMAX-4"))
        signal_rule[SIGRTMAX-4] = value;
    else if (!strcmp(key, "SIGRTMAX-3"))
        signal_rule[SIGRTMAX-3] = value;
    else if (!strcmp(key, "SIGRTMAX-2"))
        signal_rule[SIGRTMAX-2] = value;
    else if (!strcmp(key, "SIGRTMAX-1"))
        signal_rule[SIGRTMAX-1] = value;
    else if (!strcmp(key, "SIGRTMAX"))
        signal_rule[SIGRTMAX] = value;
}

void
auto_syscall_rule(const char * key, gboolean value)
{
    if (!strcmp(key, "SYS__llseek"))
        syscall_rule[SYS__llseek] = value;
    else if (!strcmp(key, "SYS__newselect"))
        syscall_rule[SYS__newselect] = value;
    else if (!strcmp(key, "SYS__sysctl"))
        syscall_rule[SYS__sysctl] = value;
    else if (!strcmp(key, "SYS_access"))
        syscall_rule[SYS_access] = value;
    else if (!strcmp(key, "SYS_acct"))
        syscall_rule[SYS_acct] = value;
    else if (!strcmp(key, "SYS_add_key"))
        syscall_rule[SYS_add_key] = value;
    else if (!strcmp(key, "SYS_adjtimex"))
        syscall_rule[SYS_adjtimex] = value;
    else if (!strcmp(key, "SYS_afs_syscall"))
        syscall_rule[SYS_afs_syscall] = value;
    else if (!strcmp(key, "SYS_alarm"))
        syscall_rule[SYS_alarm] = value;
    else if (!strcmp(key, "SYS_bdflush"))
        syscall_rule[SYS_bdflush] = value;
    else if (!strcmp(key, "SYS_break"))
        syscall_rule[SYS_break] = value;
    else if (!strcmp(key, "SYS_brk"))
        syscall_rule[SYS_brk] = value;
    else if (!strcmp(key, "SYS_capget"))
        syscall_rule[SYS_capget] = value;
    else if (!strcmp(key, "SYS_capset"))
        syscall_rule[SYS_capset] = value;
    else if (!strcmp(key, "SYS_chdir"))
        syscall_rule[SYS_chdir] = value;
    else if (!strcmp(key, "SYS_chmod"))
        syscall_rule[SYS_chmod] = value;
    else if (!strcmp(key, "SYS_chown"))
        syscall_rule[SYS_chown] = value;
    else if (!strcmp(key, "SYS_chown32"))
        syscall_rule[SYS_chown32] = value;
    else if (!strcmp(key, "SYS_chroot"))
        syscall_rule[SYS_chroot] = value;
    else if (!strcmp(key, "SYS_clock_adjtime"))
        syscall_rule[SYS_clock_adjtime] = value;
    else if (!strcmp(key, "SYS_clock_getres"))
        syscall_rule[SYS_clock_getres] = value;
    else if (!strcmp(key, "SYS_clock_gettime"))
        syscall_rule[SYS_clock_gettime] = value;
    else if (!strcmp(key, "SYS_clock_nanosleep"))
        syscall_rule[SYS_clock_nanosleep] = value;
    else if (!strcmp(key, "SYS_clock_settime"))
        syscall_rule[SYS_clock_settime] = value;
    else if (!strcmp(key, "SYS_clone"))
        syscall_rule[SYS_clone] = value;
    else if (!strcmp(key, "SYS_close"))
        syscall_rule[SYS_close] = value;
    else if (!strcmp(key, "SYS_creat"))
        syscall_rule[SYS_creat] = value;
    else if (!strcmp(key, "SYS_create_module"))
        syscall_rule[SYS_create_module] = value;
    else if (!strcmp(key, "SYS_delete_module"))
        syscall_rule[SYS_delete_module] = value;
    else if (!strcmp(key, "SYS_dup"))
        syscall_rule[SYS_dup] = value;
    else if (!strcmp(key, "SYS_dup2"))
        syscall_rule[SYS_dup2] = value;
    else if (!strcmp(key, "SYS_dup3"))
        syscall_rule[SYS_dup3] = value;
    else if (!strcmp(key, "SYS_epoll_create"))
        syscall_rule[SYS_epoll_create] = value;
    else if (!strcmp(key, "SYS_epoll_create1"))
        syscall_rule[SYS_epoll_create1] = value;
    else if (!strcmp(key, "SYS_epoll_ctl"))
        syscall_rule[SYS_epoll_ctl] = value;
    else if (!strcmp(key, "SYS_epoll_pwait"))
        syscall_rule[SYS_epoll_pwait] = value;
    else if (!strcmp(key, "SYS_epoll_wait"))
        syscall_rule[SYS_epoll_wait] = value;
    else if (!strcmp(key, "SYS_eventfd"))
        syscall_rule[SYS_eventfd] = value;
    else if (!strcmp(key, "SYS_eventfd2"))
        syscall_rule[SYS_eventfd2] = value;
    else if (!strcmp(key, "SYS_execve"))
        syscall_rule[SYS_execve] = value;
    else if (!strcmp(key, "SYS_exit"))
        syscall_rule[SYS_exit] = value;
    else if (!strcmp(key, "SYS_exit_group"))
        syscall_rule[SYS_exit_group] = value;
    else if (!strcmp(key, "SYS_faccessat"))
        syscall_rule[SYS_faccessat] = value;
    else if (!strcmp(key, "SYS_fadvise64"))
        syscall_rule[SYS_fadvise64] = value;
    else if (!strcmp(key, "SYS_fadvise64_64"))
        syscall_rule[SYS_fadvise64_64] = value;
    else if (!strcmp(key, "SYS_fallocate"))
        syscall_rule[SYS_fallocate] = value;
    else if (!strcmp(key, "SYS_fanotify_init"))
        syscall_rule[SYS_fanotify_init] = value;
    else if (!strcmp(key, "SYS_fanotify_mark"))
        syscall_rule[SYS_fanotify_mark] = value;
    else if (!strcmp(key, "SYS_fchdir"))
        syscall_rule[SYS_fchdir] = value;
    else if (!strcmp(key, "SYS_fchmod"))
        syscall_rule[SYS_fchmod] = value;
    else if (!strcmp(key, "SYS_fchmodat"))
        syscall_rule[SYS_fchmodat] = value;
    else if (!strcmp(key, "SYS_fchown"))
        syscall_rule[SYS_fchown] = value;
    else if (!strcmp(key, "SYS_fchown32"))
        syscall_rule[SYS_fchown32] = value;
    else if (!strcmp(key, "SYS_fchownat"))
        syscall_rule[SYS_fchownat] = value;
    else if (!strcmp(key, "SYS_fcntl"))
        syscall_rule[SYS_fcntl] = value;
    else if (!strcmp(key, "SYS_fcntl64"))
        syscall_rule[SYS_fcntl64] = value;
    else if (!strcmp(key, "SYS_fdatasync"))
        syscall_rule[SYS_fdatasync] = value;
    else if (!strcmp(key, "SYS_fgetxattr"))
        syscall_rule[SYS_fgetxattr] = value;
    else if (!strcmp(key, "SYS_flistxattr"))
        syscall_rule[SYS_flistxattr] = value;
    else if (!strcmp(key, "SYS_flock"))
        syscall_rule[SYS_flock] = value;
    else if (!strcmp(key, "SYS_fork"))
        syscall_rule[SYS_fork] = value;
    else if (!strcmp(key, "SYS_fremovexattr"))
        syscall_rule[SYS_fremovexattr] = value;
    else if (!strcmp(key, "SYS_fsetxattr"))
        syscall_rule[SYS_fsetxattr] = value;
    else if (!strcmp(key, "SYS_fstat"))
        syscall_rule[SYS_fstat] = value;
    else if (!strcmp(key, "SYS_fstat64"))
        syscall_rule[SYS_fstat64] = value;
    else if (!strcmp(key, "SYS_fstatat64"))
        syscall_rule[SYS_fstatat64] = value;
    else if (!strcmp(key, "SYS_fstatfs"))
        syscall_rule[SYS_fstatfs] = value;
    else if (!strcmp(key, "SYS_fstatfs64"))
        syscall_rule[SYS_fstatfs64] = value;
    else if (!strcmp(key, "SYS_fsync"))
        syscall_rule[SYS_fsync] = value;
    else if (!strcmp(key, "SYS_ftime"))
        syscall_rule[SYS_ftime] = value;
    else if (!strcmp(key, "SYS_ftruncate"))
        syscall_rule[SYS_ftruncate] = value;
    else if (!strcmp(key, "SYS_ftruncate64"))
        syscall_rule[SYS_ftruncate64] = value;
    else if (!strcmp(key, "SYS_futex"))
        syscall_rule[SYS_futex] = value;
    else if (!strcmp(key, "SYS_futimesat"))
        syscall_rule[SYS_futimesat] = value;
    else if (!strcmp(key, "SYS_get_kernel_syms"))
        syscall_rule[SYS_get_kernel_syms] = value;
    else if (!strcmp(key, "SYS_get_mempolicy"))
        syscall_rule[SYS_get_mempolicy] = value;
    else if (!strcmp(key, "SYS_get_robust_list"))
        syscall_rule[SYS_get_robust_list] = value;
    else if (!strcmp(key, "SYS_get_thread_area"))
        syscall_rule[SYS_get_thread_area] = value;
    else if (!strcmp(key, "SYS_getcpu"))
        syscall_rule[SYS_getcpu] = value;
    else if (!strcmp(key, "SYS_getcwd"))
        syscall_rule[SYS_getcwd] = value;
    else if (!strcmp(key, "SYS_getdents"))
        syscall_rule[SYS_getdents] = value;
    else if (!strcmp(key, "SYS_getdents64"))
        syscall_rule[SYS_getdents64] = value;
    else if (!strcmp(key, "SYS_getegid"))
        syscall_rule[SYS_getegid] = value;
    else if (!strcmp(key, "SYS_getegid32"))
        syscall_rule[SYS_getegid32] = value;
    else if (!strcmp(key, "SYS_geteuid"))
        syscall_rule[SYS_geteuid] = value;
    else if (!strcmp(key, "SYS_geteuid32"))
        syscall_rule[SYS_geteuid32] = value;
    else if (!strcmp(key, "SYS_getgid"))
        syscall_rule[SYS_getgid] = value;
    else if (!strcmp(key, "SYS_getgid32"))
        syscall_rule[SYS_getgid32] = value;
    else if (!strcmp(key, "SYS_getgroups"))
        syscall_rule[SYS_getgroups] = value;
    else if (!strcmp(key, "SYS_getgroups32"))
        syscall_rule[SYS_getgroups32] = value;
    else if (!strcmp(key, "SYS_getitimer"))
        syscall_rule[SYS_getitimer] = value;
    else if (!strcmp(key, "SYS_getpgid"))
        syscall_rule[SYS_getpgid] = value;
    else if (!strcmp(key, "SYS_getpgrp"))
        syscall_rule[SYS_getpgrp] = value;
    else if (!strcmp(key, "SYS_getpid"))
        syscall_rule[SYS_getpid] = value;
    else if (!strcmp(key, "SYS_getpmsg"))
        syscall_rule[SYS_getpmsg] = value;
    else if (!strcmp(key, "SYS_getppid"))
        syscall_rule[SYS_getppid] = value;
    else if (!strcmp(key, "SYS_getpriority"))
        syscall_rule[SYS_getpriority] = value;
    else if (!strcmp(key, "SYS_getresgid"))
        syscall_rule[SYS_getresgid] = value;
    else if (!strcmp(key, "SYS_getresgid32"))
        syscall_rule[SYS_getresgid32] = value;
    else if (!strcmp(key, "SYS_getresuid"))
        syscall_rule[SYS_getresuid] = value;
    else if (!strcmp(key, "SYS_getresuid32"))
        syscall_rule[SYS_getresuid32] = value;
    else if (!strcmp(key, "SYS_getrlimit"))
        syscall_rule[SYS_getrlimit] = value;
    else if (!strcmp(key, "SYS_getrusage"))
        syscall_rule[SYS_getrusage] = value;
    else if (!strcmp(key, "SYS_getsid"))
        syscall_rule[SYS_getsid] = value;
    else if (!strcmp(key, "SYS_gettid"))
        syscall_rule[SYS_gettid] = value;
    else if (!strcmp(key, "SYS_gettimeofday"))
        syscall_rule[SYS_gettimeofday] = value;
    else if (!strcmp(key, "SYS_getuid"))
        syscall_rule[SYS_getuid] = value;
    else if (!strcmp(key, "SYS_getuid32"))
        syscall_rule[SYS_getuid32] = value;
    else if (!strcmp(key, "SYS_getxattr"))
        syscall_rule[SYS_getxattr] = value;
    else if (!strcmp(key, "SYS_gtty"))
        syscall_rule[SYS_gtty] = value;
    else if (!strcmp(key, "SYS_idle"))
        syscall_rule[SYS_idle] = value;
    else if (!strcmp(key, "SYS_init_module"))
        syscall_rule[SYS_init_module] = value;
    else if (!strcmp(key, "SYS_inotify_add_watch"))
        syscall_rule[SYS_inotify_add_watch] = value;
    else if (!strcmp(key, "SYS_inotify_init"))
        syscall_rule[SYS_inotify_init] = value;
    else if (!strcmp(key, "SYS_inotify_init1"))
        syscall_rule[SYS_inotify_init1] = value;
    else if (!strcmp(key, "SYS_inotify_rm_watch"))
        syscall_rule[SYS_inotify_rm_watch] = value;
    else if (!strcmp(key, "SYS_io_cancel"))
        syscall_rule[SYS_io_cancel] = value;
    else if (!strcmp(key, "SYS_io_destroy"))
        syscall_rule[SYS_io_destroy] = value;
    else if (!strcmp(key, "SYS_io_getevents"))
        syscall_rule[SYS_io_getevents] = value;
    else if (!strcmp(key, "SYS_io_setup"))
        syscall_rule[SYS_io_setup] = value;
    else if (!strcmp(key, "SYS_io_submit"))
        syscall_rule[SYS_io_submit] = value;
    else if (!strcmp(key, "SYS_ioctl"))
        syscall_rule[SYS_ioctl] = value;
    else if (!strcmp(key, "SYS_ioperm"))
        syscall_rule[SYS_ioperm] = value;
    else if (!strcmp(key, "SYS_iopl"))
        syscall_rule[SYS_iopl] = value;
    else if (!strcmp(key, "SYS_ioprio_get"))
        syscall_rule[SYS_ioprio_get] = value;
    else if (!strcmp(key, "SYS_ioprio_set"))
        syscall_rule[SYS_ioprio_set] = value;
    else if (!strcmp(key, "SYS_ipc"))
        syscall_rule[SYS_ipc] = value;
    else if (!strcmp(key, "SYS_kexec_load"))
        syscall_rule[SYS_kexec_load] = value;
    else if (!strcmp(key, "SYS_keyctl"))
        syscall_rule[SYS_keyctl] = value;
    else if (!strcmp(key, "SYS_kill"))
        syscall_rule[SYS_kill] = value;
    else if (!strcmp(key, "SYS_lchown"))
        syscall_rule[SYS_lchown] = value;
    else if (!strcmp(key, "SYS_lchown32"))
        syscall_rule[SYS_lchown32] = value;
    else if (!strcmp(key, "SYS_lgetxattr"))
        syscall_rule[SYS_lgetxattr] = value;
    else if (!strcmp(key, "SYS_link"))
        syscall_rule[SYS_link] = value;
    else if (!strcmp(key, "SYS_linkat"))
        syscall_rule[SYS_linkat] = value;
    else if (!strcmp(key, "SYS_listxattr"))
        syscall_rule[SYS_listxattr] = value;
    else if (!strcmp(key, "SYS_llistxattr"))
        syscall_rule[SYS_llistxattr] = value;
    else if (!strcmp(key, "SYS_lock"))
        syscall_rule[SYS_lock] = value;
    else if (!strcmp(key, "SYS_lookup_dcookie"))
        syscall_rule[SYS_lookup_dcookie] = value;
    else if (!strcmp(key, "SYS_lremovexattr"))
        syscall_rule[SYS_lremovexattr] = value;
    else if (!strcmp(key, "SYS_lseek"))
        syscall_rule[SYS_lseek] = value;
    else if (!strcmp(key, "SYS_lsetxattr"))
        syscall_rule[SYS_lsetxattr] = value;
    else if (!strcmp(key, "SYS_lstat"))
        syscall_rule[SYS_lstat] = value;
    else if (!strcmp(key, "SYS_lstat64"))
        syscall_rule[SYS_lstat64] = value;
    else if (!strcmp(key, "SYS_madvise"))
        syscall_rule[SYS_madvise] = value;
    else if (!strcmp(key, "SYS_madvise1"))
        syscall_rule[SYS_madvise1] = value;
    else if (!strcmp(key, "SYS_mbind"))
        syscall_rule[SYS_mbind] = value;
    else if (!strcmp(key, "SYS_migrate_pages"))
        syscall_rule[SYS_migrate_pages] = value;
    else if (!strcmp(key, "SYS_mincore"))
        syscall_rule[SYS_mincore] = value;
    else if (!strcmp(key, "SYS_mkdir"))
        syscall_rule[SYS_mkdir] = value;
    else if (!strcmp(key, "SYS_mkdirat"))
        syscall_rule[SYS_mkdirat] = value;
    else if (!strcmp(key, "SYS_mknod"))
        syscall_rule[SYS_mknod] = value;
    else if (!strcmp(key, "SYS_mknodat"))
        syscall_rule[SYS_mknodat] = value;
    else if (!strcmp(key, "SYS_mlock"))
        syscall_rule[SYS_mlock] = value;
    else if (!strcmp(key, "SYS_mlockall"))
        syscall_rule[SYS_mlockall] = value;
    else if (!strcmp(key, "SYS_mmap"))
        syscall_rule[SYS_mmap] = value;
    else if (!strcmp(key, "SYS_mmap2"))
        syscall_rule[SYS_mmap2] = value;
    else if (!strcmp(key, "SYS_modify_ldt"))
        syscall_rule[SYS_modify_ldt] = value;
    else if (!strcmp(key, "SYS_mount"))
        syscall_rule[SYS_mount] = value;
    else if (!strcmp(key, "SYS_move_pages"))
        syscall_rule[SYS_move_pages] = value;
    else if (!strcmp(key, "SYS_mprotect"))
        syscall_rule[SYS_mprotect] = value;
    else if (!strcmp(key, "SYS_mpx"))
        syscall_rule[SYS_mpx] = value;
    else if (!strcmp(key, "SYS_mq_getsetattr"))
        syscall_rule[SYS_mq_getsetattr] = value;
    else if (!strcmp(key, "SYS_mq_notify"))
        syscall_rule[SYS_mq_notify] = value;
    else if (!strcmp(key, "SYS_mq_open"))
        syscall_rule[SYS_mq_open] = value;
    else if (!strcmp(key, "SYS_mq_timedreceive"))
        syscall_rule[SYS_mq_timedreceive] = value;
    else if (!strcmp(key, "SYS_mq_timedsend"))
        syscall_rule[SYS_mq_timedsend] = value;
    else if (!strcmp(key, "SYS_mq_unlink"))
        syscall_rule[SYS_mq_unlink] = value;
    else if (!strcmp(key, "SYS_mremap"))
        syscall_rule[SYS_mremap] = value;
    else if (!strcmp(key, "SYS_msync"))
        syscall_rule[SYS_msync] = value;
    else if (!strcmp(key, "SYS_munlock"))
        syscall_rule[SYS_munlock] = value;
    else if (!strcmp(key, "SYS_munlockall"))
        syscall_rule[SYS_munlockall] = value;
    else if (!strcmp(key, "SYS_munmap"))
        syscall_rule[SYS_munmap] = value;
    else if (!strcmp(key, "SYS_name_to_handle_at"))
        syscall_rule[SYS_name_to_handle_at] = value;
    else if (!strcmp(key, "SYS_nanosleep"))
        syscall_rule[SYS_nanosleep] = value;
    else if (!strcmp(key, "SYS_nfsservctl"))
        syscall_rule[SYS_nfsservctl] = value;
    else if (!strcmp(key, "SYS_nice"))
        syscall_rule[SYS_nice] = value;
    else if (!strcmp(key, "SYS_oldfstat"))
        syscall_rule[SYS_oldfstat] = value;
    else if (!strcmp(key, "SYS_oldlstat"))
        syscall_rule[SYS_oldlstat] = value;
    else if (!strcmp(key, "SYS_oldolduname"))
        syscall_rule[SYS_oldolduname] = value;
    else if (!strcmp(key, "SYS_oldstat"))
        syscall_rule[SYS_oldstat] = value;
    else if (!strcmp(key, "SYS_olduname"))
        syscall_rule[SYS_olduname] = value;
    else if (!strcmp(key, "SYS_open"))
        syscall_rule[SYS_open] = value;
    else if (!strcmp(key, "SYS_open_by_handle_at"))
        syscall_rule[SYS_open_by_handle_at] = value;
    else if (!strcmp(key, "SYS_openat"))
        syscall_rule[SYS_openat] = value;
    else if (!strcmp(key, "SYS_pause"))
        syscall_rule[SYS_pause] = value;
    else if (!strcmp(key, "SYS_perf_event_open"))
        syscall_rule[SYS_perf_event_open] = value;
    else if (!strcmp(key, "SYS_personality"))
        syscall_rule[SYS_personality] = value;
    else if (!strcmp(key, "SYS_pipe"))
        syscall_rule[SYS_pipe] = value;
    else if (!strcmp(key, "SYS_pipe2"))
        syscall_rule[SYS_pipe2] = value;
    else if (!strcmp(key, "SYS_pivot_root"))
        syscall_rule[SYS_pivot_root] = value;
    else if (!strcmp(key, "SYS_poll"))
        syscall_rule[SYS_poll] = value;
    else if (!strcmp(key, "SYS_ppoll"))
        syscall_rule[SYS_ppoll] = value;
    else if (!strcmp(key, "SYS_prctl"))
        syscall_rule[SYS_prctl] = value;
    else if (!strcmp(key, "SYS_pread64"))
        syscall_rule[SYS_pread64] = value;
    else if (!strcmp(key, "SYS_preadv"))
        syscall_rule[SYS_preadv] = value;
    else if (!strcmp(key, "SYS_prlimit64"))
        syscall_rule[SYS_prlimit64] = value;
    else if (!strcmp(key, "SYS_process_vm_readv"))
        syscall_rule[SYS_process_vm_readv] = value;
    else if (!strcmp(key, "SYS_process_vm_writev"))
        syscall_rule[SYS_process_vm_writev] = value;
    else if (!strcmp(key, "SYS_prof"))
        syscall_rule[SYS_prof] = value;
    else if (!strcmp(key, "SYS_profil"))
        syscall_rule[SYS_profil] = value;
    else if (!strcmp(key, "SYS_pselect6"))
        syscall_rule[SYS_pselect6] = value;
    else if (!strcmp(key, "SYS_ptrace"))
        syscall_rule[SYS_ptrace] = value;
    else if (!strcmp(key, "SYS_putpmsg"))
        syscall_rule[SYS_putpmsg] = value;
    else if (!strcmp(key, "SYS_pwrite64"))
        syscall_rule[SYS_pwrite64] = value;
    else if (!strcmp(key, "SYS_pwritev"))
        syscall_rule[SYS_pwritev] = value;
    else if (!strcmp(key, "SYS_query_module"))
        syscall_rule[SYS_query_module] = value;
    else if (!strcmp(key, "SYS_quotactl"))
        syscall_rule[SYS_quotactl] = value;
    else if (!strcmp(key, "SYS_read"))
        syscall_rule[SYS_read] = value;
    else if (!strcmp(key, "SYS_readahead"))
        syscall_rule[SYS_readahead] = value;
    else if (!strcmp(key, "SYS_readdir"))
        syscall_rule[SYS_readdir] = value;
    else if (!strcmp(key, "SYS_readlink"))
        syscall_rule[SYS_readlink] = value;
    else if (!strcmp(key, "SYS_readlinkat"))
        syscall_rule[SYS_readlinkat] = value;
    else if (!strcmp(key, "SYS_readv"))
        syscall_rule[SYS_readv] = value;
    else if (!strcmp(key, "SYS_reboot"))
        syscall_rule[SYS_reboot] = value;
    else if (!strcmp(key, "SYS_recvmmsg"))
        syscall_rule[SYS_recvmmsg] = value;
    else if (!strcmp(key, "SYS_remap_file_pages"))
        syscall_rule[SYS_remap_file_pages] = value;
    else if (!strcmp(key, "SYS_removexattr"))
        syscall_rule[SYS_removexattr] = value;
    else if (!strcmp(key, "SYS_rename"))
        syscall_rule[SYS_rename] = value;
    else if (!strcmp(key, "SYS_renameat"))
        syscall_rule[SYS_renameat] = value;
    else if (!strcmp(key, "SYS_request_key"))
        syscall_rule[SYS_request_key] = value;
    else if (!strcmp(key, "SYS_restart_syscall"))
        syscall_rule[SYS_restart_syscall] = value;
    else if (!strcmp(key, "SYS_rmdir"))
        syscall_rule[SYS_rmdir] = value;
    else if (!strcmp(key, "SYS_rt_sigaction"))
        syscall_rule[SYS_rt_sigaction] = value;
    else if (!strcmp(key, "SYS_rt_sigpending"))
        syscall_rule[SYS_rt_sigpending] = value;
    else if (!strcmp(key, "SYS_rt_sigprocmask"))
        syscall_rule[SYS_rt_sigprocmask] = value;
    else if (!strcmp(key, "SYS_rt_sigqueueinfo"))
        syscall_rule[SYS_rt_sigqueueinfo] = value;
    else if (!strcmp(key, "SYS_rt_sigreturn"))
        syscall_rule[SYS_rt_sigreturn] = value;
    else if (!strcmp(key, "SYS_rt_sigsuspend"))
        syscall_rule[SYS_rt_sigsuspend] = value;
    else if (!strcmp(key, "SYS_rt_sigtimedwait"))
        syscall_rule[SYS_rt_sigtimedwait] = value;
    else if (!strcmp(key, "SYS_rt_tgsigqueueinfo"))
        syscall_rule[SYS_rt_tgsigqueueinfo] = value;
    else if (!strcmp(key, "SYS_sched_get_priority_max"))
        syscall_rule[SYS_sched_get_priority_max] = value;
    else if (!strcmp(key, "SYS_sched_get_priority_min"))
        syscall_rule[SYS_sched_get_priority_min] = value;
    else if (!strcmp(key, "SYS_sched_getaffinity"))
        syscall_rule[SYS_sched_getaffinity] = value;
    else if (!strcmp(key, "SYS_sched_getparam"))
        syscall_rule[SYS_sched_getparam] = value;
    else if (!strcmp(key, "SYS_sched_getscheduler"))
        syscall_rule[SYS_sched_getscheduler] = value;
    else if (!strcmp(key, "SYS_sched_rr_get_interval"))
        syscall_rule[SYS_sched_rr_get_interval] = value;
    else if (!strcmp(key, "SYS_sched_setaffinity"))
        syscall_rule[SYS_sched_setaffinity] = value;
    else if (!strcmp(key, "SYS_sched_setparam"))
        syscall_rule[SYS_sched_setparam] = value;
    else if (!strcmp(key, "SYS_sched_setscheduler"))
        syscall_rule[SYS_sched_setscheduler] = value;
    else if (!strcmp(key, "SYS_sched_yield"))
        syscall_rule[SYS_sched_yield] = value;
    else if (!strcmp(key, "SYS_select"))
        syscall_rule[SYS_select] = value;
    else if (!strcmp(key, "SYS_sendfile"))
        syscall_rule[SYS_sendfile] = value;
    else if (!strcmp(key, "SYS_sendfile64"))
        syscall_rule[SYS_sendfile64] = value;
    else if (!strcmp(key, "SYS_sendmmsg"))
        syscall_rule[SYS_sendmmsg] = value;
    else if (!strcmp(key, "SYS_set_mempolicy"))
        syscall_rule[SYS_set_mempolicy] = value;
    else if (!strcmp(key, "SYS_set_robust_list"))
        syscall_rule[SYS_set_robust_list] = value;
    else if (!strcmp(key, "SYS_set_thread_area"))
        syscall_rule[SYS_set_thread_area] = value;
    else if (!strcmp(key, "SYS_set_tid_address"))
        syscall_rule[SYS_set_tid_address] = value;
    else if (!strcmp(key, "SYS_setdomainname"))
        syscall_rule[SYS_setdomainname] = value;
    else if (!strcmp(key, "SYS_setfsgid"))
        syscall_rule[SYS_setfsgid] = value;
    else if (!strcmp(key, "SYS_setfsgid32"))
        syscall_rule[SYS_setfsgid32] = value;
    else if (!strcmp(key, "SYS_setfsuid"))
        syscall_rule[SYS_setfsuid] = value;
    else if (!strcmp(key, "SYS_setfsuid32"))
        syscall_rule[SYS_setfsuid32] = value;
    else if (!strcmp(key, "SYS_setgid"))
        syscall_rule[SYS_setgid] = value;
    else if (!strcmp(key, "SYS_setgid32"))
        syscall_rule[SYS_setgid32] = value;
    else if (!strcmp(key, "SYS_setgroups"))
        syscall_rule[SYS_setgroups] = value;
    else if (!strcmp(key, "SYS_setgroups32"))
        syscall_rule[SYS_setgroups32] = value;
    else if (!strcmp(key, "SYS_sethostname"))
        syscall_rule[SYS_sethostname] = value;
    else if (!strcmp(key, "SYS_setitimer"))
        syscall_rule[SYS_setitimer] = value;
    else if (!strcmp(key, "SYS_setns"))
        syscall_rule[SYS_setns] = value;
    else if (!strcmp(key, "SYS_setpgid"))
        syscall_rule[SYS_setpgid] = value;
    else if (!strcmp(key, "SYS_setpriority"))
        syscall_rule[SYS_setpriority] = value;
    else if (!strcmp(key, "SYS_setregid"))
        syscall_rule[SYS_setregid] = value;
    else if (!strcmp(key, "SYS_setregid32"))
        syscall_rule[SYS_setregid32] = value;
    else if (!strcmp(key, "SYS_setresgid"))
        syscall_rule[SYS_setresgid] = value;
    else if (!strcmp(key, "SYS_setresgid32"))
        syscall_rule[SYS_setresgid32] = value;
    else if (!strcmp(key, "SYS_setresuid"))
        syscall_rule[SYS_setresuid] = value;
    else if (!strcmp(key, "SYS_setresuid32"))
        syscall_rule[SYS_setresuid32] = value;
    else if (!strcmp(key, "SYS_setreuid"))
        syscall_rule[SYS_setreuid] = value;
    else if (!strcmp(key, "SYS_setreuid32"))
        syscall_rule[SYS_setreuid32] = value;
    else if (!strcmp(key, "SYS_setrlimit"))
        syscall_rule[SYS_setrlimit] = value;
    else if (!strcmp(key, "SYS_setsid"))
        syscall_rule[SYS_setsid] = value;
    else if (!strcmp(key, "SYS_settimeofday"))
        syscall_rule[SYS_settimeofday] = value;
    else if (!strcmp(key, "SYS_setuid"))
        syscall_rule[SYS_setuid] = value;
    else if (!strcmp(key, "SYS_setuid32"))
        syscall_rule[SYS_setuid32] = value;
    else if (!strcmp(key, "SYS_setxattr"))
        syscall_rule[SYS_setxattr] = value;
    else if (!strcmp(key, "SYS_sgetmask"))
        syscall_rule[SYS_sgetmask] = value;
    else if (!strcmp(key, "SYS_sigaction"))
        syscall_rule[SYS_sigaction] = value;
    else if (!strcmp(key, "SYS_sigaltstack"))
        syscall_rule[SYS_sigaltstack] = value;
    else if (!strcmp(key, "SYS_signal"))
        syscall_rule[SYS_signal] = value;
    else if (!strcmp(key, "SYS_signalfd"))
        syscall_rule[SYS_signalfd] = value;
    else if (!strcmp(key, "SYS_signalfd4"))
        syscall_rule[SYS_signalfd4] = value;
    else if (!strcmp(key, "SYS_sigpending"))
        syscall_rule[SYS_sigpending] = value;
    else if (!strcmp(key, "SYS_sigprocmask"))
        syscall_rule[SYS_sigprocmask] = value;
    else if (!strcmp(key, "SYS_sigreturn"))
        syscall_rule[SYS_sigreturn] = value;
    else if (!strcmp(key, "SYS_sigsuspend"))
        syscall_rule[SYS_sigsuspend] = value;
    else if (!strcmp(key, "SYS_socketcall"))
        syscall_rule[SYS_socketcall] = value;
    else if (!strcmp(key, "SYS_splice"))
        syscall_rule[SYS_splice] = value;
    else if (!strcmp(key, "SYS_ssetmask"))
        syscall_rule[SYS_ssetmask] = value;
    else if (!strcmp(key, "SYS_stat"))
        syscall_rule[SYS_stat] = value;
    else if (!strcmp(key, "SYS_stat64"))
        syscall_rule[SYS_stat64] = value;
    else if (!strcmp(key, "SYS_statfs"))
        syscall_rule[SYS_statfs] = value;
    else if (!strcmp(key, "SYS_statfs64"))
        syscall_rule[SYS_statfs64] = value;
    else if (!strcmp(key, "SYS_stime"))
        syscall_rule[SYS_stime] = value;
    else if (!strcmp(key, "SYS_stty"))
        syscall_rule[SYS_stty] = value;
    else if (!strcmp(key, "SYS_swapoff"))
        syscall_rule[SYS_swapoff] = value;
    else if (!strcmp(key, "SYS_swapon"))
        syscall_rule[SYS_swapon] = value;
    else if (!strcmp(key, "SYS_symlink"))
        syscall_rule[SYS_symlink] = value;
    else if (!strcmp(key, "SYS_symlinkat"))
        syscall_rule[SYS_symlinkat] = value;
    else if (!strcmp(key, "SYS_sync"))
        syscall_rule[SYS_sync] = value;
    else if (!strcmp(key, "SYS_sync_file_range"))
        syscall_rule[SYS_sync_file_range] = value;
    else if (!strcmp(key, "SYS_syncfs"))
        syscall_rule[SYS_syncfs] = value;
    else if (!strcmp(key, "SYS_sysfs"))
        syscall_rule[SYS_sysfs] = value;
    else if (!strcmp(key, "SYS_sysinfo"))
        syscall_rule[SYS_sysinfo] = value;
    else if (!strcmp(key, "SYS_syslog"))
        syscall_rule[SYS_syslog] = value;
    else if (!strcmp(key, "SYS_tee"))
        syscall_rule[SYS_tee] = value;
    else if (!strcmp(key, "SYS_tgkill"))
        syscall_rule[SYS_tgkill] = value;
    else if (!strcmp(key, "SYS_time"))
        syscall_rule[SYS_time] = value;
    else if (!strcmp(key, "SYS_timer_create"))
        syscall_rule[SYS_timer_create] = value;
    else if (!strcmp(key, "SYS_timer_delete"))
        syscall_rule[SYS_timer_delete] = value;
    else if (!strcmp(key, "SYS_timer_getoverrun"))
        syscall_rule[SYS_timer_getoverrun] = value;
    else if (!strcmp(key, "SYS_timer_gettime"))
        syscall_rule[SYS_timer_gettime] = value;
    else if (!strcmp(key, "SYS_timer_settime"))
        syscall_rule[SYS_timer_settime] = value;
    else if (!strcmp(key, "SYS_timerfd_create"))
        syscall_rule[SYS_timerfd_create] = value;
    else if (!strcmp(key, "SYS_timerfd_gettime"))
        syscall_rule[SYS_timerfd_gettime] = value;
    else if (!strcmp(key, "SYS_timerfd_settime"))
        syscall_rule[SYS_timerfd_settime] = value;
    else if (!strcmp(key, "SYS_times"))
        syscall_rule[SYS_times] = value;
    else if (!strcmp(key, "SYS_tkill"))
        syscall_rule[SYS_tkill] = value;
    else if (!strcmp(key, "SYS_truncate"))
        syscall_rule[SYS_truncate] = value;
    else if (!strcmp(key, "SYS_truncate64"))
        syscall_rule[SYS_truncate64] = value;
    else if (!strcmp(key, "SYS_ugetrlimit"))
        syscall_rule[SYS_ugetrlimit] = value;
    else if (!strcmp(key, "SYS_ulimit"))
        syscall_rule[SYS_ulimit] = value;
    else if (!strcmp(key, "SYS_umask"))
        syscall_rule[SYS_umask] = value;
    else if (!strcmp(key, "SYS_umount"))
        syscall_rule[SYS_umount] = value;
    else if (!strcmp(key, "SYS_umount2"))
        syscall_rule[SYS_umount2] = value;
    else if (!strcmp(key, "SYS_uname"))
        syscall_rule[SYS_uname] = value;
    else if (!strcmp(key, "SYS_unlink"))
        syscall_rule[SYS_unlink] = value;
    else if (!strcmp(key, "SYS_unlinkat"))
        syscall_rule[SYS_unlinkat] = value;
    else if (!strcmp(key, "SYS_unshare"))
        syscall_rule[SYS_unshare] = value;
    else if (!strcmp(key, "SYS_uselib"))
        syscall_rule[SYS_uselib] = value;
    else if (!strcmp(key, "SYS_ustat"))
        syscall_rule[SYS_ustat] = value;
    else if (!strcmp(key, "SYS_utime"))
        syscall_rule[SYS_utime] = value;
    else if (!strcmp(key, "SYS_utimensat"))
        syscall_rule[SYS_utimensat] = value;
    else if (!strcmp(key, "SYS_utimes"))
        syscall_rule[SYS_utimes] = value;
    else if (!strcmp(key, "SYS_vfork"))
        syscall_rule[SYS_vfork] = value;
    else if (!strcmp(key, "SYS_vhangup"))
        syscall_rule[SYS_vhangup] = value;
    else if (!strcmp(key, "SYS_vm86"))
        syscall_rule[SYS_vm86] = value;
    else if (!strcmp(key, "SYS_vm86old"))
        syscall_rule[SYS_vm86old] = value;
    else if (!strcmp(key, "SYS_vmsplice"))
        syscall_rule[SYS_vmsplice] = value;
    else if (!strcmp(key, "SYS_vserver"))
        syscall_rule[SYS_vserver] = value;
    else if (!strcmp(key, "SYS_wait4"))
        syscall_rule[SYS_wait4] = value;
    else if (!strcmp(key, "SYS_waitid"))
        syscall_rule[SYS_waitid] = value;
    else if (!strcmp(key, "SYS_waitpid"))
        syscall_rule[SYS_waitpid] = value;
    else if (!strcmp(key, "SYS_write"))
        syscall_rule[SYS_write] = value;
    else if (!strcmp(key, "SYS_writev"))
        syscall_rule[SYS_writev] = value;
}

void
auto_resource_rule(const char * key, rlim_t rlim_cur, rlim_t rlim_max)
{
    Resource * tmp = NULL;
    tmp = (Resource *) malloc(sizeof(Resource));
    g_assert(tmp);

    if (!strcmp(key, "RLIMIT_AS"))
        tmp->resource = RLIMIT_AS;
    else if (!strcmp(key, "RLIMIT_CORE"))
        tmp->resource = RLIMIT_CORE;
    else if (!strcmp(key, "RLIMIT_CPU"))
        tmp->resource = RLIMIT_CPU;
    else if (!strcmp(key, "RLIMIT_DATA"))
        tmp->resource = RLIMIT_DATA;
    else if (!strcmp(key, "RLIMIT_FSIZE"))
        tmp->resource = RLIMIT_FSIZE;
    else if (!strcmp(key, "RLIMIT_LOCKS"))
        tmp->resource = RLIMIT_LOCKS;
    else if (!strcmp(key, "RLIMIT_MEMLOCK"))
        tmp->resource = RLIMIT_MEMLOCK;
    else if (!strcmp(key, "RLIMIT_MSGQUEUE"))
        tmp->resource = RLIMIT_MSGQUEUE;
    else if (!strcmp(key, "RLIMIT_NICE"))
        tmp->resource = RLIMIT_NICE;
    else if (!strcmp(key, "RLIMIT_NOFILE"))
        tmp->resource = RLIMIT_NOFILE;
    else if (!strcmp(key, "RLIMIT_NPROC"))
        tmp->resource = RLIMIT_NPROC;
    else if (!strcmp(key, "RLIMIT_RSS"))
        tmp->resource = RLIMIT_RSS;
    else if (!strcmp(key, "RLIMIT_RTPRIO"))
        tmp->resource = RLIMIT_RTPRIO;
    else if (!strcmp(key, "RLIMIT_RTTIME"))
        tmp->resource = RLIMIT_RTTIME;
    else if (!strcmp(key, "RLIMIT_SIGPENDING"))
        tmp->resource = RLIMIT_SIGPENDING;
    else if (!strcmp(key, "RLIMIT_STACK"))
        tmp->resource = RLIMIT_STACK;
    else {
        free(tmp);
        return;
    }

    tmp->lmt.rlim_cur = rlim_cur;
    tmp->lmt.rlim_max = rlim_max;
    resource_rule = g_slist_append(resource_rule, tmp);
}

void
auto_environ_rule(const char * key, const char * value)
{
    Environ * tmp = NULL;
    tmp = (Environ *) malloc(sizeof(Environ));
    g_assert(tmp);

    tmp->key = key;
    tmp->value = value;
    environ_rule = g_slist_append(environ_rule, tmp);
}
