/*
 * -*- coding: utf-8 -*-
 * Filename      : jcommand.c
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-04-14
 */

#include "global.h"

extern int ltime;
extern int fsize;

extern GString * input;
extern GString * output;
extern GString * answer;

extern char * workdir;
extern char * const * command;
extern Result * result;
extern struct passwd * pwd;

static int pre_time;
static int pre_memory;
static jmp_buf jbuf;

static void alarm_func(int signo);
static void trace_child(pid_t child);
static void filter_output();
static void setup_child();
static void setup_io();
static void setup_resource();
static int get_time(struct rusage *used);
static int get_memory(struct rusage *used);

void execute_jcommand();

void
execute_jcommand()
{
    int signo;
    int status;
    pid_t child;
    struct rusage used;
    struct user_regs_struct regs;
    
    child = fork();
    if (child == 0)
        setup_child();

    wait3(&status, 0, &used);
    
    /* 子进程在exec之前退出 */
    if (WIFEXITED(status)) {
        switch (WEXITSTATUS(status)) {
            case 0 : g_string_assign(result->err, "drop privileges error");
                     break;
            case 1 : g_string_assign(result->err, "execvpe error");
                     break;
            case 2 : g_string_assign(result->err, "open input file error");
                     break;
            case 3 : g_string_assign(result->err, "open output file error");
                     break;
            case 4 : g_string_assign(result->err, "dup2 input error");
                     break;
            case 5 : g_string_assign(result->err, "dup2 ouput error");
                     break;
        }
        result->code = EXIT_IE;
        return;
    }

    /* 期待子进程被停止，如果不是，则 */
    if (!WIFSTOPPED(status)) {
        kill(child, SIGKILL);
        result->code = EXIT_IE;
        g_string_assign(result->err,
                "unknow child status before exec syscall");
        return;
    }

    /* 子进程停止原因，期待是SYS_execve和SIGTRAP停止 */
    signo = WSTOPSIG(status);
    ptrace(PTRACE_GETREGS, child, NULL, &regs);
    if (signo != SIGTRAP || regs.orig_eax != SYS_execve) {
        kill(child, SIGKILL);
        result->code = EXIT_IE;
        g_string_assign(result->err,
                "unexpect child stop before exec syscall");
        return;
        
    }

    /* 子进程状态符合，获取exec之前的时间和内存 */
    pre_time = get_time(&used);
    pre_memory = get_memory(&used);

    /* 注册父进程挂种时间及超时回跳位置 */
    signal(SIGALRM, alarm_func);
    if (setjmp(jbuf)) {
        kill(child, SIGKILL);
        result->time = ltime;
        result->code = EXIT_TLE;
        return;
    }
    if (ltime % 1000)
        alarm(ltime / 1000 + 3);
    else
        alarm(ltime / 1000 + 2);

    /* 继续并等待子进程 */
    ptrace(PTRACE_SYSCALL, child, NULL, NULL);
    trace_child(child);
    /* 非超时返回 */
    alarm(0);

    if (result->code == EXIT_AC)
        filter_output();
    return;
}

static void
trace_child(pid_t child)
{
    int status;
    int signo;
    struct rusage used;

    while (1) {
        wait3(&status, 0, &used);

        result->time = get_time(&used);
        result->memory = get_memory(&used);

        /* 子进程退出 */
        if (WIFEXITED(status)) {
            if (result->time > ltime)
                result->code = EXIT_TLE;
            else
                result->code = EXIT_AC;
            return;
        }

        /* 子进程被信号终止 */
        if (WIFSIGNALED(status)) {
            result->code = EXIT_RE;
            g_string_printf(result->err, "Invalid Signal %d", WTERMSIG(status));
            return;
        }

        /* 除了结束外，期待的是停止状态，如果不是 */
        if (!WIFSTOPPED(status)) {
            kill(child, SIGKILL);
            result->code = EXIT_IE;
            g_string_assign(result->err, "unknow user program status");
            return;
        }

        signo = WSTOPSIG(status); 
        if (signo == SIGXCPU)
            result->code = EXIT_TLE;
        else if (signo == SIGXFSZ)
            result->code = EXIT_OLE;
        else {
            ptrace(PTRACE_SYSCALL, child, 0, 0);
            continue;
        }

        kill(child, SIGKILL);
        return;
    } // end while
} // end function

static void
setup_child()
{
    /* io需要在降权之前设置，因为需要打开一个输出文件 */
    setup_io();
    
    /* 关闭所有继承的附加组id， 设置有gid，uid */
    if (setgroups(0, NULL) != 0 ||
            setresgid(pwd->pw_gid, pwd->pw_gid, pwd->pw_gid) != 0 ||
            setresuid(pwd->pw_uid, pwd->pw_uid, pwd->pw_uid) != 0)
        exit(0);

    chdir(workdir);
    setup_resource();
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);

    execvpe(command[0], command, (char * const *)g_get_environ());
    exit(1);
}

static void
setup_io()
{
    int infd, outfd;

    infd = open(input->str, O_RDONLY);
    if (infd == -1) exit(2);
    outfd = open(output->str, O_WRONLY | O_CREAT, FMODE);
    if (outfd == -1) exit(3);

    dup2(infd, STDIN_FILENO);
    dup2(outfd, STDOUT_FILENO);
    dup2(outfd, STDERR_FILENO);
    
    close(infd);
    close(outfd);
}

static void 
setup_resource()
{
    struct rlimit t;

    if (ltime % 1000 == 0)
        t.rlim_cur = ltime / 1000;
    else
        t.rlim_cur = ltime / 1000 + 1;
    t.rlim_max = t.rlim_cur + 1;
    setrlimit(RLIMIT_CPU, &t);

    t.rlim_cur = t.rlim_max = fsize * 1024;
    setrlimit(RLIMIT_FSIZE, &t);

    t.rlim_cur = t.rlim_max = RLIM_INFINITY;
    setrlimit(RLIMIT_AS, &t);

    /*
    t.rlim_cur = t.rlim_max = memory * 1024;
    setrlimit(RLIMIT_DATA, &t);
    */
}

static int
get_time(struct rusage *used)
{
    return (*used).ru_utime.tv_sec * 1000 + 
        (*used).ru_utime.tv_usec / 1000 +
        (*used).ru_stime.tv_sec * 1000 +
        (*used).ru_stime.tv_usec / 1000 - pre_time;
}

static int
get_memory(struct rusage *used)
{
    return  (*used).ru_minflt * getpagesize() / 1024 - pre_memory;
}

static void
alarm_func(int signo)
{
    if (signo == SIGALRM)
        longjmp(jbuf, 1);
}

static void
filter_output()
{
    return;
}
