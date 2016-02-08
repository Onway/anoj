/*
 * -*- coding: utf-8 -*-
 * Filename      : conf.c
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-04-24
 */
#include "global.h"

extern int signal_rule[];
extern const char * feedback[];
extern GSList * environ_rule;
extern GSList * resource_rule;
extern gboolean syscall_rule[];

void init_conf();
static void init_signal_rule();
static void init_feedback();
static void init_environ_rule();
static void init_resource_rule();
static void init_syscall_rule();

void
init_conf()
{
    init_signal_rule();
    init_feedback();
    /*
    init_environ_rule();
    init_resource_rule();
    */
    init_syscall_rule();
}

static void
init_signal_rule()
{
    signal_rule[SIGXCPU] = SIG_TERMINATE;
    signal_rule[SIGXFSZ] = SIG_TERMINATE;
    signal_rule[SIGHUP] = SIG_TERMINATE;
    signal_rule[SIGINT] = SIG_TERMINATE;
    signal_rule[SIGQUIT] = SIG_TERMINATE;
    signal_rule[SIGILL] = SIG_TERMINATE;
    signal_rule[SIGABRT] = SIG_TERMINATE;
    signal_rule[SIGBUS] = SIG_TERMINATE;
    signal_rule[SIGFPE] = SIG_TERMINATE;
    signal_rule[SIGSEGV] = SIG_TERMINATE;
    signal_rule[SIGPIPE] = SIG_TERMINATE;
    signal_rule[SIGALRM] = SIG_TERMINATE;
    signal_rule[SIGTERM] = SIG_TERMINATE;
    signal_rule[SIGSTKFLT] = SIG_TERMINATE;
    signal_rule[SIGCHLD] = SIG_TERMINATE;
    signal_rule[SIGCONT] = SIG_TERMINATE;
    signal_rule[SIGTSTP] = SIG_TERMINATE;
    signal_rule[SIGTTIN] = SIG_TERMINATE;
    signal_rule[SIGTTOU] = SIG_TERMINATE;
    signal_rule[SIGURG] = SIG_TERMINATE;
    signal_rule[SIGVTALRM] = SIG_TERMINATE;
    signal_rule[SIGPROF] = SIG_TERMINATE;
    signal_rule[SIGWINCH] = SIG_TERMINATE;
    signal_rule[SIGIO] = SIG_TERMINATE;
    signal_rule[SIGPWR] = SIG_TERMINATE;
    signal_rule[SIGSYS] = SIG_TERMINATE;
}

void
static init_feedback()
{
    int i;
    
    feedback[SIGSEGV] = g_strdup("Invalid Memory Reference");
    feedback[SIGFPE] = g_strdup("Floating Point Exception");

    for (i = 0; i < SIGNAL_NUM; ++i)
        if (feedback[i] == NULL)
            feedback[i] = g_strdup("");
}

static void
init_environ_rule()
{
    Environ * tmp = NULL;

    /*
    tmp = (Environ *)malloc(sizeof(Environ));
    tmp->key = g_strdup("ENV_KEY");
    tmp->key = g_strdup("ENV_VALUE");
    environ_rule = g_slist_append(environ_rule, tmp);
    */
}

static void
init_resource_rule()
{
    Resource * tmp = NULL;

#ifdef RLIMIT_CORE
    tmp = (Resource *)malloc(sizeof(Resource));
    tmp->resource = RLIMIT_CORE;
    tmp->lmt.rlim_cur = tmp->lmt.rlim_max = 0;
    resource_rule = g_slist_append(resource_rule, tmp);
#endif
#ifdef RLIMIT_NPROC
    tmp = (Resource *)malloc(sizeof(Resource));
    tmp->resource = RLIMIT_NPROC;
    tmp->lmt.rlim_cur = tmp->lmt.rlim_max = 0;
    resource_rule = g_slist_append(resource_rule, tmp);
#endif
#ifdef RLIMIT_NOFILE
    tmp = (Resource *)malloc(sizeof(Resource));
    tmp->resource = RLIMIT_NOFILE;
    tmp->lmt.rlim_cur = tmp->lmt.rlim_max = 30;
    resource_rule = g_slist_append(resource_rule, tmp);
#endif
}

static void
init_syscall_rule()
{
#ifdef SYS__llseek
    syscall_rule[SYS__llseek] = TRUE;
#endif
#ifdef SYS__newselect
    syscall_rule[SYS__newselect] = TRUE;
#endif
#ifdef SYS__sysctl
    syscall_rule[SYS__sysctl] = FALSE;
#endif
#ifdef SYS_access
    syscall_rule[SYS_access] = TRUE;
#endif
#ifdef SYS_acct
    syscall_rule[SYS_acct] = FALSE;
#endif
#ifdef SYS_add_key
    syscall_rule[SYS_add_key] = FALSE;
#endif
#ifdef SYS_adjtimex
    syscall_rule[SYS_adjtimex] = TRUE;
#endif
#ifdef SYS_afs_syscall
    syscall_rule[SYS_afs_syscall] = FALSE;
#endif
#ifdef SYS_alarm
    syscall_rule[SYS_alarm] = TRUE;
#endif
#ifdef SYS_bdflush
    syscall_rule[SYS_bdflush] = TRUE;
#endif
#ifdef SYS_break
    syscall_rule[SYS_break] = FALSE;
#endif
#ifdef SYS_brk
    syscall_rule[SYS_brk] = TRUE;
#endif
#ifdef SYS_capget
    syscall_rule[SYS_capget] = TRUE;
#endif
#ifdef SYS_capset
    syscall_rule[SYS_capset] = TRUE;
#endif
#ifdef SYS_chdir
    syscall_rule[SYS_chdir] = TRUE;
#endif
#ifdef SYS_chmod
    syscall_rule[SYS_chmod] = TRUE;
#endif
#ifdef SYS_chown
    syscall_rule[SYS_chown] = TRUE;
#endif
#ifdef SYS_chown32
    syscall_rule[SYS_chown32] = TRUE;
#endif
#ifdef SYS_chroot
    syscall_rule[SYS_chroot] = TRUE;
#endif
#ifdef SYS_clock_adjtime
    syscall_rule[SYS_clock_adjtime] = TRUE;
#endif
#ifdef SYS_clock_getres
    syscall_rule[SYS_clock_getres] = TRUE;
#endif
#ifdef SYS_clock_gettime
    syscall_rule[SYS_clock_gettime] = TRUE;
#endif
#ifdef SYS_clock_nanosleep
    syscall_rule[SYS_clock_nanosleep] = TRUE;
#endif
#ifdef SYS_clock_settime
    syscall_rule[SYS_clock_settime] = TRUE;
#endif
#ifdef SYS_clone
    syscall_rule[SYS_clone] = FALSE;
#endif
#ifdef SYS_close
    syscall_rule[SYS_close] = TRUE;
#endif
#ifdef SYS_creat
    syscall_rule[SYS_creat] = FALSE;
#endif
#ifdef SYS_create_module
    syscall_rule[SYS_create_module] = FALSE;
#endif
#ifdef SYS_delete_module
    syscall_rule[SYS_delete_module] = FALSE;
#endif
#ifdef SYS_dup
    syscall_rule[SYS_dup] = FALSE;
#endif
#ifdef SYS_dup2
    syscall_rule[SYS_dup2] = FALSE;
#endif
#ifdef SYS_dup3
    syscall_rule[SYS_dup3] = FALSE;
#endif
#ifdef SYS_epoll_create
    syscall_rule[SYS_epoll_create] = TRUE;
#endif
#ifdef SYS_epoll_create1
    syscall_rule[SYS_epoll_create1] = TRUE;
#endif
#ifdef SYS_epoll_ctl
    syscall_rule[SYS_epoll_ctl] = TRUE;
#endif
#ifdef SYS_epoll_pwait
    syscall_rule[SYS_epoll_pwait] = TRUE;
#endif
#ifdef SYS_epoll_wait
    syscall_rule[SYS_epoll_wait] = TRUE;
#endif
#ifdef SYS_eventfd
    syscall_rule[SYS_eventfd] = FALSE;
#endif
#ifdef SYS_eventfd2
    syscall_rule[SYS_eventfd2] = FALSE;
#endif
#ifdef SYS_execve
    syscall_rule[SYS_execve] = FALSE;
#endif
#ifdef SYS_exit
    syscall_rule[SYS_exit] = TRUE;
#endif
#ifdef SYS_exit_group
    syscall_rule[SYS_exit_group] = TRUE;
#endif
#ifdef SYS_faccessat
    syscall_rule[SYS_faccessat] = TRUE;
#endif
#ifdef SYS_fadvise64
    syscall_rule[SYS_fadvise64] = TRUE;
#endif
#ifdef SYS_fadvise64_64
    syscall_rule[SYS_fadvise64_64] = TRUE;
#endif
#ifdef SYS_fallocate
    syscall_rule[SYS_fallocate] = FALSE;
#endif
#ifdef SYS_fanotify_init
    syscall_rule[SYS_fanotify_init] = FALSE;
#endif
#ifdef SYS_fanotify_mark
    syscall_rule[SYS_fanotify_mark] = FALSE;
#endif
#ifdef SYS_fchdir
    syscall_rule[SYS_fchdir] = TRUE;
#endif
#ifdef SYS_fchmod
    syscall_rule[SYS_fchmod] = TRUE;
#endif
#ifdef SYS_fchmodat
    syscall_rule[SYS_fchmodat] = TRUE;
#endif
#ifdef SYS_fchown
    syscall_rule[SYS_fchown] = TRUE;
#endif
#ifdef SYS_fchown32
    syscall_rule[SYS_fchown32] = TRUE;
#endif
#ifdef SYS_fchownat
    syscall_rule[SYS_fchownat] = TRUE;
#endif
#ifdef SYS_fcntl
    syscall_rule[SYS_fcntl] = TRUE;
#endif
#ifdef SYS_fcntl64
    syscall_rule[SYS_fcntl64] = TRUE;
#endif
#ifdef SYS_fdatasync
    syscall_rule[SYS_fdatasync] = TRUE;
#endif
#ifdef SYS_fgetxattr
    syscall_rule[SYS_fgetxattr] = TRUE;
#endif
#ifdef SYS_flistxattr
    syscall_rule[SYS_flistxattr] = TRUE;
#endif
#ifdef SYS_flock
    syscall_rule[SYS_flock] = TRUE;
#endif
#ifdef SYS_fork
    syscall_rule[SYS_fork] = FALSE;
#endif
#ifdef SYS_fremovexattr
    syscall_rule[SYS_fremovexattr] = FALSE;
#endif
#ifdef SYS_fsetxattr
    syscall_rule[SYS_fsetxattr] = FALSE;
#endif
#ifdef SYS_fstat
    syscall_rule[SYS_fstat] = TRUE;
#endif
#ifdef SYS_fstat64
    syscall_rule[SYS_fstat64] = TRUE;
#endif
#ifdef SYS_fstatat64
    syscall_rule[SYS_fstatat64] = TRUE;
#endif
#ifdef SYS_fstatfs
    syscall_rule[SYS_fstatfs] = TRUE;
#endif
#ifdef SYS_fstatfs64
    syscall_rule[SYS_fstatfs64] = TRUE;
#endif
#ifdef SYS_fsync
    syscall_rule[SYS_fsync] = TRUE;
#endif
#ifdef SYS_ftime
    syscall_rule[SYS_ftime] = FALSE;
#endif
#ifdef SYS_ftruncate
    syscall_rule[SYS_ftruncate] = FALSE;
#endif
#ifdef SYS_ftruncate64
    syscall_rule[SYS_ftruncate64] = FALSE;
#endif
#ifdef SYS_futex
    syscall_rule[SYS_futex] = TRUE;
#endif
#ifdef SYS_futimesat
    syscall_rule[SYS_futimesat] = FALSE;
#endif
#ifdef SYS_get_kernel_syms
    syscall_rule[SYS_get_kernel_syms] = FALSE;
#endif
#ifdef SYS_get_mempolicy
    syscall_rule[SYS_get_mempolicy] = FALSE;
#endif
#ifdef SYS_get_robust_list
    syscall_rule[SYS_get_robust_list] = TRUE;
#endif
#ifdef SYS_get_thread_area
    syscall_rule[SYS_get_thread_area] = TRUE;
#endif
#ifdef SYS_getcpu
    syscall_rule[SYS_getcpu] = FALSE;
#endif
#ifdef SYS_getcwd
    syscall_rule[SYS_getcwd] = TRUE;
#endif
#ifdef SYS_getdents
    syscall_rule[SYS_getdents] = TRUE;
#endif
#ifdef SYS_getdents64
    syscall_rule[SYS_getdents64] = TRUE;
#endif
#ifdef SYS_getegid
    syscall_rule[SYS_getegid] = TRUE;
#endif
#ifdef SYS_getegid32
    syscall_rule[SYS_getegid32] = TRUE;
#endif
#ifdef SYS_geteuid
    syscall_rule[SYS_geteuid] = TRUE;
#endif
#ifdef SYS_geteuid32
    syscall_rule[SYS_geteuid32] = TRUE;
#endif
#ifdef SYS_getgid
    syscall_rule[SYS_getgid] = TRUE;
#endif
#ifdef SYS_getgid32
    syscall_rule[SYS_getgid32] = TRUE;
#endif
#ifdef SYS_getgroups
    syscall_rule[SYS_getgroups] = TRUE;
#endif
#ifdef SYS_getgroups32
    syscall_rule[SYS_getgroups32] = TRUE;
#endif
#ifdef SYS_getitimer
    syscall_rule[SYS_getitimer] = TRUE;
#endif
#ifdef SYS_getpgid
    syscall_rule[SYS_getpgid] = TRUE;
#endif
#ifdef SYS_getpgrp
    syscall_rule[SYS_getpgrp] = TRUE;
#endif
#ifdef SYS_getpid
    syscall_rule[SYS_getpid] = TRUE;
#endif
#ifdef SYS_getpmsg
    syscall_rule[SYS_getpmsg] = FALSE;
#endif
#ifdef SYS_getppid
    syscall_rule[SYS_getppid] = TRUE;
#endif
#ifdef SYS_getpriority
    syscall_rule[SYS_getpriority] = TRUE;
#endif
#ifdef SYS_getresgid
    syscall_rule[SYS_getresgid] = TRUE;
#endif
#ifdef SYS_getresgid32
    syscall_rule[SYS_getresgid32] = TRUE;
#endif
#ifdef SYS_getresuid
    syscall_rule[SYS_getresuid] = TRUE;
#endif
#ifdef SYS_getresuid32
    syscall_rule[SYS_getresuid32] = TRUE;
#endif
#ifdef SYS_getrlimit
    syscall_rule[SYS_getrlimit] = TRUE;
#endif
#ifdef SYS_getrusage
    syscall_rule[SYS_getrusage] = TRUE;
#endif
#ifdef SYS_getsid
    syscall_rule[SYS_getsid] = TRUE;
#endif
#ifdef SYS_gettid
    syscall_rule[SYS_gettid] = TRUE;
#endif
#ifdef SYS_gettimeofday
    syscall_rule[SYS_gettimeofday] = TRUE;
#endif
#ifdef SYS_getuid
    syscall_rule[SYS_getuid] = TRUE;
#endif
#ifdef SYS_getuid32
    syscall_rule[SYS_getuid32] = TRUE;
#endif
#ifdef SYS_getxattr
    syscall_rule[SYS_getxattr] = FALSE;
#endif
#ifdef SYS_gtty
    syscall_rule[SYS_gtty] = FALSE;
#endif
#ifdef SYS_idle
    syscall_rule[SYS_idle] = TRUE;
#endif
#ifdef SYS_init_module
    syscall_rule[SYS_init_module] = FALSE;
#endif
#ifdef SYS_inotify_add_watch
    syscall_rule[SYS_inotify_add_watch] = FALSE;
#endif
#ifdef SYS_inotify_init
    syscall_rule[SYS_inotify_init] = FALSE;
#endif
#ifdef SYS_inotify_init1
    syscall_rule[SYS_inotify_init1] = FALSE;
#endif
#ifdef SYS_inotify_rm_watch
    syscall_rule[SYS_inotify_rm_watch] = FALSE;
#endif
#ifdef SYS_io_cancel
    syscall_rule[SYS_io_cancel] = FALSE;
#endif
#ifdef SYS_io_destroy
    syscall_rule[SYS_io_destroy] = FALSE;
#endif
#ifdef SYS_io_getevents
    syscall_rule[SYS_io_getevents] = FALSE;
#endif
#ifdef SYS_io_setup
    syscall_rule[SYS_io_setup] = FALSE;
#endif
#ifdef SYS_io_submit
    syscall_rule[SYS_io_submit] = FALSE;
#endif
#ifdef SYS_ioctl
    syscall_rule[SYS_ioctl] = TRUE;
#endif
#ifdef SYS_ioperm
    syscall_rule[SYS_ioperm] = FALSE;
#endif
#ifdef SYS_iopl
    syscall_rule[SYS_iopl] = FALSE;
#endif
#ifdef SYS_ioprio_get
    syscall_rule[SYS_ioprio_get] = FALSE;
#endif
#ifdef SYS_ioprio_set
    syscall_rule[SYS_ioprio_set] = FALSE;
#endif
#ifdef SYS_ipc
    syscall_rule[SYS_ipc] = TRUE;
#endif
#ifdef SYS_kexec_load
    syscall_rule[SYS_kexec_load] = FALSE;
#endif
#ifdef SYS_keyctl
    syscall_rule[SYS_keyctl] = TRUE;
#endif
#ifdef SYS_kill
    syscall_rule[SYS_kill] = FALSE;
#endif
#ifdef SYS_lchown
    syscall_rule[SYS_lchown] = TRUE;
#endif
#ifdef SYS_lchown32
    syscall_rule[SYS_lchown32] = TRUE;
#endif
#ifdef SYS_lgetxattr
    syscall_rule[SYS_lgetxattr] = FALSE;
#endif
#ifdef SYS_link
    syscall_rule[SYS_link] = FALSE;
#endif
#ifdef SYS_linkat
    syscall_rule[SYS_linkat] = TRUE;
#endif
#ifdef SYS_listxattr
    syscall_rule[SYS_listxattr] = FALSE;
#endif
#ifdef SYS_llistxattr
    syscall_rule[SYS_llistxattr] = FALSE;
#endif
#ifdef SYS_lock
    syscall_rule[SYS_lock] = FALSE;
#endif
#ifdef SYS_lookup_dcookie
    syscall_rule[SYS_lookup_dcookie] = FALSE;
#endif
#ifdef SYS_lremovexattr
    syscall_rule[SYS_lremovexattr] = FALSE;
#endif
#ifdef SYS_lseek
    syscall_rule[SYS_lseek] = TRUE;
#endif
#ifdef SYS_lsetxattr
    syscall_rule[SYS_lsetxattr] = FALSE;
#endif
#ifdef SYS_lstat
    syscall_rule[SYS_lstat] = TRUE;
#endif
#ifdef SYS_lstat64
    syscall_rule[SYS_lstat64] = TRUE;
#endif
#ifdef SYS_madvise
    syscall_rule[SYS_madvise] = FALSE;
#endif
#ifdef SYS_madvise1
    syscall_rule[SYS_madvise1] = FALSE;
#endif
#ifdef SYS_mbind
    syscall_rule[SYS_mbind] = FALSE;
#endif
#ifdef SYS_migrate_pages
    syscall_rule[SYS_migrate_pages] = FALSE;
#endif
#ifdef SYS_mincore
    syscall_rule[SYS_mincore] = TRUE;
#endif
#ifdef SYS_mkdir
    syscall_rule[SYS_mkdir] = FALSE;
#endif
#ifdef SYS_mkdirat
    syscall_rule[SYS_mkdirat] = FALSE;
#endif
#ifdef SYS_mknod
    syscall_rule[SYS_mknod] = FALSE;
#endif
#ifdef SYS_mknodat
    syscall_rule[SYS_mknodat] = FALSE;
#endif
#ifdef SYS_mlock
    syscall_rule[SYS_mlock] = TRUE;
#endif
#ifdef SYS_mlockall
    syscall_rule[SYS_mlockall] = TRUE;
#endif
#ifdef SYS_mmap
    syscall_rule[SYS_mmap] = TRUE;
#endif
#ifdef SYS_mmap2
    syscall_rule[SYS_mmap2] = TRUE;
#endif
#ifdef SYS_modify_ldt
    syscall_rule[SYS_modify_ldt] = FALSE;
#endif
#ifdef SYS_mount
    syscall_rule[SYS_mount] = FALSE;
#endif
#ifdef SYS_move_pages
    syscall_rule[SYS_move_pages] = FALSE;
#endif
#ifdef SYS_mprotect
    syscall_rule[SYS_mprotect] = TRUE;
#endif
#ifdef SYS_mpx
    syscall_rule[SYS_mpx] = FALSE;
#endif
#ifdef SYS_mq_getsetattr
    syscall_rule[SYS_mq_getsetattr] = FALSE;
#endif
#ifdef SYS_mq_notify
    syscall_rule[SYS_mq_notify] = FALSE;
#endif
#ifdef SYS_mq_open
    syscall_rule[SYS_mq_open] = FALSE;
#endif
#ifdef SYS_mq_timedreceive
    syscall_rule[SYS_mq_timedreceive] = FALSE;
#endif
#ifdef SYS_mq_timedsend
    syscall_rule[SYS_mq_timedsend] = FALSE;
#endif
#ifdef SYS_mq_unlink
    syscall_rule[SYS_mq_unlink] = FALSE;
#endif
#ifdef SYS_mremap
    syscall_rule[SYS_mremap] = TRUE;
#endif
#ifdef SYS_msync
    syscall_rule[SYS_msync] = TRUE;
#endif
#ifdef SYS_munlock
    syscall_rule[SYS_munlock] = TRUE;
#endif
#ifdef SYS_munlockall
    syscall_rule[SYS_munlockall] = TRUE;
#endif
#ifdef SYS_munmap
    syscall_rule[SYS_munmap] = TRUE;
#endif
#ifdef SYS_name_to_handle_at
    syscall_rule[SYS_name_to_handle_at] = FALSE;
#endif
#ifdef SYS_nanosleep
    syscall_rule[SYS_nanosleep] = TRUE;
#endif
#ifdef SYS_nfsservctl
    syscall_rule[SYS_nfsservctl] = FALSE;
#endif
#ifdef SYS_nice
    syscall_rule[SYS_nice] = FALSE;
#endif
#ifdef SYS_oldfstat
    syscall_rule[SYS_oldfstat] = TRUE;
#endif
#ifdef SYS_oldlstat
    syscall_rule[SYS_oldlstat] = TRUE;
#endif
#ifdef SYS_oldolduname
    syscall_rule[SYS_oldolduname] = TRUE;
#endif
#ifdef SYS_oldstat
    syscall_rule[SYS_oldstat] = TRUE;
#endif
#ifdef SYS_olduname
    syscall_rule[SYS_olduname] = TRUE;
#endif
#ifdef SYS_open
    syscall_rule[SYS_open] = TRUE;
#endif
#ifdef SYS_open_by_handle_at
    syscall_rule[SYS_open_by_handle_at] = FALSE;
#endif
#ifdef SYS_openat
    syscall_rule[SYS_openat] = TRUE;
#endif
#ifdef SYS_pause
    syscall_rule[SYS_pause] = TRUE;
#endif
#ifdef SYS_perf_event_open
    syscall_rule[SYS_perf_event_open] = FALSE;
#endif
#ifdef SYS_personality
    syscall_rule[SYS_personality] = FALSE;
#endif
#ifdef SYS_pipe
    syscall_rule[SYS_pipe] = TRUE;
#endif
#ifdef SYS_pipe2
    syscall_rule[SYS_pipe2] = TRUE;
#endif
#ifdef SYS_pivot_root
    syscall_rule[SYS_pivot_root] = FALSE;
#endif
#ifdef SYS_poll
    syscall_rule[SYS_poll] = TRUE;
#endif
#ifdef SYS_ppoll
    syscall_rule[SYS_ppoll] = TRUE;
#endif
#ifdef SYS_prctl
    syscall_rule[SYS_prctl] = FALSE;
#endif
#ifdef SYS_arch_prctl
    syscall_rule[SYS_arch_prctl] = TRUE;
#endif
#ifdef SYS_pread64
    syscall_rule[SYS_pread64] = TRUE;
#endif
#ifdef SYS_preadv
    syscall_rule[SYS_preadv] = TRUE;
#endif
#ifdef SYS_prlimit64
    syscall_rule[SYS_prlimit64] = TRUE;
#endif
#ifdef SYS_process_vm_readv
    syscall_rule[SYS_process_vm_readv] = TRUE;
#endif
#ifdef SYS_process_vm_writev
    syscall_rule[SYS_process_vm_writev] = TRUE;
#endif
#ifdef SYS_prof
    syscall_rule[SYS_prof] = FALSE;
#endif
#ifdef SYS_profil
    syscall_rule[SYS_profil] = FALSE;
#endif
#ifdef SYS_pselect6
    syscall_rule[SYS_pselect6] = TRUE;
#endif
#ifdef SYS_ptrace
    syscall_rule[SYS_ptrace] = FALSE;
#endif
#ifdef SYS_putpmsg
    syscall_rule[SYS_putpmsg] = FALSE;
#endif
#ifdef SYS_pwrite64
    syscall_rule[SYS_pwrite64] = TRUE;
#endif
#ifdef SYS_pwritev
    syscall_rule[SYS_pwritev] = TRUE;
#endif
#ifdef SYS_query_module
    syscall_rule[SYS_query_module] = FALSE;
#endif
#ifdef SYS_quotactl
    syscall_rule[SYS_quotactl] = FALSE;
#endif
#ifdef SYS_read
    syscall_rule[SYS_read] = TRUE;
#endif
#ifdef SYS_readahead
    syscall_rule[SYS_readahead] = TRUE;
#endif
#ifdef SYS_readdir
    syscall_rule[SYS_readdir] = TRUE;
#endif
#ifdef SYS_readlink
    syscall_rule[SYS_readlink] = TRUE;
#endif
#ifdef SYS_readlinkat
    syscall_rule[SYS_readlinkat] = TRUE;
#endif
#ifdef SYS_readv
    syscall_rule[SYS_readv] = TRUE;
#endif
#ifdef SYS_reboot
    syscall_rule[SYS_reboot] = FALSE;
#endif
#ifdef SYS_recvmmsg
    syscall_rule[SYS_recvmmsg] = TRUE;
#endif
#ifdef SYS_remap_file_pages
    syscall_rule[SYS_remap_file_pages] = FALSE;
#endif
#ifdef SYS_removexattr
    syscall_rule[SYS_removexattr] = FALSE;
#endif
#ifdef SYS_rename
    syscall_rule[SYS_rename] = FALSE;
#endif
#ifdef SYS_renameat
    syscall_rule[SYS_renameat] = FALSE;
#endif
#ifdef SYS_request_key
    syscall_rule[SYS_request_key] = FALSE;
#endif
#ifdef SYS_restart_syscall
    syscall_rule[SYS_restart_syscall] = FALSE;
#endif
#ifdef SYS_rmdir
    syscall_rule[SYS_rmdir] = FALSE;
#endif
#ifdef SYS_rt_sigaction
    syscall_rule[SYS_rt_sigaction] = TRUE;
#endif
#ifdef SYS_rt_sigpending
    syscall_rule[SYS_rt_sigpending] = TRUE;
#endif
#ifdef SYS_rt_sigprocmask
    syscall_rule[SYS_rt_sigprocmask] = TRUE;
#endif
#ifdef SYS_rt_sigqueueinfo
    syscall_rule[SYS_rt_sigqueueinfo] = TRUE;
#endif
#ifdef SYS_rt_sigreturn
    syscall_rule[SYS_rt_sigreturn] = TRUE;
#endif
#ifdef SYS_rt_sigsuspend
    syscall_rule[SYS_rt_sigsuspend] = TRUE;
#endif
#ifdef SYS_rt_sigtimedwait
    syscall_rule[SYS_rt_sigtimedwait] = TRUE;
#endif
#ifdef SYS_rt_tgsigqueueinfo
    syscall_rule[SYS_rt_tgsigqueueinfo] = TRUE;
#endif
#ifdef SYS_sched_get_priority_max
    syscall_rule[SYS_sched_get_priority_max] = FALSE;
#endif
#ifdef SYS_sched_get_priority_min
    syscall_rule[SYS_sched_get_priority_min] = FALSE;
#endif
#ifdef SYS_sched_getaffinity
    syscall_rule[SYS_sched_getaffinity] = FALSE;
#endif
#ifdef SYS_sched_getparam
    syscall_rule[SYS_sched_getparam] = FALSE;
#endif
#ifdef SYS_sched_getscheduler
    syscall_rule[SYS_sched_getscheduler] = FALSE;
#endif
#ifdef SYS_sched_rr_get_interval
    syscall_rule[SYS_sched_rr_get_interval] = FALSE;
#endif
#ifdef SYS_sched_setaffinity
    syscall_rule[SYS_sched_setaffinity] = FALSE;
#endif
#ifdef SYS_sched_setparam
    syscall_rule[SYS_sched_setparam] = FALSE;
#endif
#ifdef SYS_sched_setscheduler
    syscall_rule[SYS_sched_setscheduler] = FALSE;
#endif
#ifdef SYS_sched_yield
    syscall_rule[SYS_sched_yield] = FALSE;
#endif
#ifdef SYS_select
    syscall_rule[SYS_select] = TRUE;
#endif
#ifdef SYS_sendfile
    syscall_rule[SYS_sendfile] = TRUE;
#endif
#ifdef SYS_sendfile64
    syscall_rule[SYS_sendfile64] = TRUE;
#endif
#ifdef SYS_sendmmsg
    syscall_rule[SYS_sendmmsg] = FALSE;
#endif
#ifdef SYS_set_mempolicy
    syscall_rule[SYS_set_mempolicy] = FALSE;
#endif
#ifdef SYS_set_robust_list
    syscall_rule[SYS_set_robust_list] = TRUE;
#endif
#ifdef SYS_set_thread_area
    syscall_rule[SYS_set_thread_area] = TRUE;
#endif
#ifdef SYS_set_tid_address
    syscall_rule[SYS_set_tid_address] = TRUE;
#endif
#ifdef SYS_setdomainname
    syscall_rule[SYS_setdomainname] = FALSE;
#endif
#ifdef SYS_setfsgid
    syscall_rule[SYS_setfsgid] = FALSE;
#endif
#ifdef SYS_setfsgid32
    syscall_rule[SYS_setfsgid32] = FALSE;
#endif
#ifdef SYS_setfsuid
    syscall_rule[SYS_setfsuid] = FALSE;
#endif
#ifdef SYS_setfsuid32
    syscall_rule[SYS_setfsuid32] = FALSE;
#endif
#ifdef SYS_setgid
    syscall_rule[SYS_setgid] = TRUE;
#endif
#ifdef SYS_setgid32
    syscall_rule[SYS_setgid32] = TRUE;
#endif
#ifdef SYS_setgroups
    syscall_rule[SYS_setgroups] = TRUE;
#endif
#ifdef SYS_setgroups32
    syscall_rule[SYS_setgroups32] = TRUE;
#endif
#ifdef SYS_sethostname
    syscall_rule[SYS_sethostname] = FALSE;
#endif
#ifdef SYS_setitimer
    syscall_rule[SYS_setitimer] = TRUE;
#endif
#ifdef SYS_setns
    syscall_rule[SYS_setns] = FALSE;
#endif
#ifdef SYS_setpgid
    syscall_rule[SYS_setpgid] = FALSE;
#endif
#ifdef SYS_setpriority
    syscall_rule[SYS_setpriority] = FALSE;
#endif
#ifdef SYS_setregid
    syscall_rule[SYS_setregid] = TRUE;
#endif
#ifdef SYS_setregid32
    syscall_rule[SYS_setregid32] = TRUE;
#endif
#ifdef SYS_setresgid
    syscall_rule[SYS_setresgid] = TRUE;
#endif
#ifdef SYS_setresgid32
    syscall_rule[SYS_setresgid32] = TRUE;
#endif
#ifdef SYS_setresuid
    syscall_rule[SYS_setresuid] = TRUE;
#endif
#ifdef SYS_setresuid32
    syscall_rule[SYS_setresuid32] = TRUE;
#endif
#ifdef SYS_setreuid
    syscall_rule[SYS_setreuid] = TRUE;
#endif
#ifdef SYS_setreuid32
    syscall_rule[SYS_setreuid32] = TRUE;
#endif
#ifdef SYS_setrlimit
    syscall_rule[SYS_setrlimit] = TRUE;
#endif
#ifdef SYS_setsid
    syscall_rule[SYS_setsid] = FALSE;
#endif
#ifdef SYS_settimeofday
    syscall_rule[SYS_settimeofday] = TRUE;
#endif
#ifdef SYS_setuid
    syscall_rule[SYS_setuid] = TRUE;
#endif
#ifdef SYS_setuid32
    syscall_rule[SYS_setuid32] = TRUE;
#endif
#ifdef SYS_setxattr
    syscall_rule[SYS_setxattr] = FALSE;
#endif
#ifdef SYS_sgetmask
    syscall_rule[SYS_sgetmask] = TRUE;
#endif
#ifdef SYS_sigaction
    syscall_rule[SYS_sigaction] = TRUE;
#endif
#ifdef SYS_sigaltstack
    syscall_rule[SYS_sigaltstack] = TRUE;
#endif
#ifdef SYS_signal
    syscall_rule[SYS_signal] = TRUE;
#endif
#ifdef SYS_signalfd
    syscall_rule[SYS_signalfd] = TRUE;
#endif
#ifdef SYS_signalfd4
    syscall_rule[SYS_signalfd4] = TRUE;
#endif
#ifdef SYS_sigpending
    syscall_rule[SYS_sigpending] = TRUE;
#endif
#ifdef SYS_sigprocmask
    syscall_rule[SYS_sigprocmask] = TRUE;
#endif
#ifdef SYS_sigreturn
    syscall_rule[SYS_sigreturn] = TRUE;
#endif
#ifdef SYS_sigsuspend
    syscall_rule[SYS_sigsuspend] = TRUE;
#endif
#ifdef SYS_socketcall
    syscall_rule[SYS_socketcall] = FALSE;
#endif
#ifdef SYS_splice
    syscall_rule[SYS_splice] = TRUE;
#endif
#ifdef SYS_ssetmask
    syscall_rule[SYS_ssetmask] = TRUE;
#endif
#ifdef SYS_stat
    syscall_rule[SYS_stat] = TRUE;
#endif
#ifdef SYS_stat64
    syscall_rule[SYS_stat64] = TRUE;
#endif
#ifdef SYS_statfs
    syscall_rule[SYS_statfs] = TRUE;
#endif
#ifdef SYS_statfs64
    syscall_rule[SYS_statfs64] = TRUE;
#endif
#ifdef SYS_stime
    syscall_rule[SYS_stime] = TRUE;
#endif
#ifdef SYS_stty
    syscall_rule[SYS_stty] = FALSE;
#endif
#ifdef SYS_swapoff
    syscall_rule[SYS_swapoff] = FALSE;
#endif
#ifdef SYS_swapon
    syscall_rule[SYS_swapon] = FALSE;
#endif
#ifdef SYS_symlink
    syscall_rule[SYS_symlink] = FALSE;
#endif
#ifdef SYS_symlinkat
    syscall_rule[SYS_symlinkat] = FALSE;
#endif
#ifdef SYS_sync
    syscall_rule[SYS_sync] = TRUE;
#endif
#ifdef SYS_sync_file_range
    syscall_rule[SYS_sync_file_range] = TRUE;
#endif
#ifdef SYS_syncfs
    syscall_rule[SYS_syncfs] = TRUE;
#endif
#ifdef SYS_sysfs
    syscall_rule[SYS_sysfs] = TRUE;
#endif
#ifdef SYS_sysinfo
    syscall_rule[SYS_sysinfo] = TRUE;
#endif
#ifdef SYS_syslog
    syscall_rule[SYS_syslog] = FALSE;
#endif
#ifdef SYS_tee
    syscall_rule[SYS_tee] = TRUE;
#endif
#ifdef SYS_tgkill
    syscall_rule[SYS_tgkill] = TRUE;
#endif
#ifdef SYS_time
    syscall_rule[SYS_time] = TRUE;
#endif
#ifdef SYS_timer_create
    syscall_rule[SYS_timer_create] = TRUE;
#endif
#ifdef SYS_timer_delete
    syscall_rule[SYS_timer_delete] = TRUE;
#endif
#ifdef SYS_timer_getoverrun
    syscall_rule[SYS_timer_getoverrun] = TRUE;
#endif
#ifdef SYS_timer_gettime
    syscall_rule[SYS_timer_gettime] = TRUE;
#endif
#ifdef SYS_timer_settime
    syscall_rule[SYS_timer_settime] = TRUE;
#endif
#ifdef SYS_timerfd_create
    syscall_rule[SYS_timerfd_create] = TRUE;
#endif
#ifdef SYS_timerfd_gettime
    syscall_rule[SYS_timerfd_gettime] = TRUE;
#endif
#ifdef SYS_timerfd_settime
    syscall_rule[SYS_timerfd_settime] = TRUE;
#endif
#ifdef SYS_times
    syscall_rule[SYS_times] = TRUE;
#endif
#ifdef SYS_tkill
    syscall_rule[SYS_tkill] = TRUE;
#endif
#ifdef SYS_truncate
    syscall_rule[SYS_truncate] = FALSE;
#endif
#ifdef SYS_truncate64
    syscall_rule[SYS_truncate64] = FALSE;
#endif
#ifdef SYS_ugetrlimit
    syscall_rule[SYS_ugetrlimit] = TRUE;
#endif
#ifdef SYS_ulimit
    syscall_rule[SYS_ulimit] = TRUE;
#endif
#ifdef SYS_umask
    syscall_rule[SYS_umask] = FALSE;
#endif
#ifdef SYS_umount
    syscall_rule[SYS_umount] = FALSE;
#endif
#ifdef SYS_umount2
    syscall_rule[SYS_umount2] = FALSE;
#endif
#ifdef SYS_uname
    syscall_rule[SYS_uname] = TRUE;
#endif
#ifdef SYS_unlink
    syscall_rule[SYS_unlink] = FALSE;
#endif
#ifdef SYS_unlinkat
    syscall_rule[SYS_unlinkat] = FALSE;
#endif
#ifdef SYS_unshare
    syscall_rule[SYS_unshare] = TRUE;
#endif
#ifdef SYS_uselib
    syscall_rule[SYS_uselib] = TRUE;
#endif
#ifdef SYS_ustat
    syscall_rule[SYS_ustat] = TRUE;
#endif
#ifdef SYS_utime
    syscall_rule[SYS_utime] = TRUE;
#endif
#ifdef SYS_utimensat
    syscall_rule[SYS_utimensat] = TRUE;
#endif
#ifdef SYS_utimes
    syscall_rule[SYS_utimes] = TRUE;
#endif
#ifdef SYS_vfork
    syscall_rule[SYS_vfork] = FALSE;
#endif
#ifdef SYS_vhangup
    syscall_rule[SYS_vhangup] = FALSE;
#endif
#ifdef SYS_vm86
    syscall_rule[SYS_vm86] = FALSE;
#endif
#ifdef SYS_vm86old
    syscall_rule[SYS_vm86old] = FALSE;
#endif
#ifdef SYS_vmsplice
    syscall_rule[SYS_vmsplice] = FALSE;
#endif
#ifdef SYS_vserver
    syscall_rule[SYS_vserver] = FALSE;
#endif
#ifdef SYS_wait4
    syscall_rule[SYS_wait4] = TRUE;
#endif
#ifdef SYS_waitid
    syscall_rule[SYS_waitid] = TRUE;
#endif
#ifdef SYS_waitpid
    syscall_rule[SYS_waitpid] = TRUE;
#endif
#ifdef SYS_write
    syscall_rule[SYS_write] = TRUE;
#endif
#ifdef SYS_writev
    syscall_rule[SYS_writev] = TRUE;
#endif
}
