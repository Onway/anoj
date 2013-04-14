/*
 * -*- coding: utf-8 -*-
 * Filename      : command.c
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-13
 */

#include "global.h"

extern int ltime;
extern int fsize;
extern int memory;
extern char * lang;
extern char * workdir;
extern char * const * command;
extern GString * input;
extern GString * output;
extern GString * answer;
extern GSList * resource_rule;
extern GSList * environ_rule;
extern int signal_rule[];
extern gboolean syscall_rule[];
extern char * feedback[];
extern Result * result;
extern struct passwd * pwd;

/* 子进程在执行exec调用之前的内存和时间，用于最后的资源统计 */
static int pre_time;
static int pre_memory;

static jmp_buf jbuf;

static void alarm_func(int signo);          /* alarm 信号处理函数 */
static void trace_child(int child);         /* 子进程信号和系统调用捕捉 */
static void setup_child();                  /* 子进程在exec前的设置 */
static void setup_io();                     /* 子进程io重定向 */
static void setup_resource();               /* 子进程资源特定设置 */
static int get_time(struct rusage *used);   /* 子进程时间使用 */
static int get_memory(struct rusage *used); /* 子进程内存使用 */

/* 启动子进程，安装时钟信号，获取子进程在exec之前的时间内存 */
void execute_command();

/* 子进程资源和环境变量设置，从配置文件指定 */
static void foreach_resource(gpointer data, gpointer user_data);
static void foreach_environ(gpointer data, gpointer user_data);

void
execute_command()
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

    /* 继续并跟踪子进程 */
    ptrace(PTRACE_SYSCALL, child, NULL, NULL);
    trace_child(child);

    /* 非超时返回 */
    alarm(0);
    return;
}

static void
trace_child(int child)
{
    int signo;
    int status;
    int endflag = 1;    /* endflag == 0 为进入系统调用 */
    struct rusage used;
    struct user_regs_struct regs;

    while (1) {
        wait3(&status, 0, &used);
        result->time = get_time(&used);
        result->memory = get_memory(&used);

        /* 子进程退出 */
        if (WIFEXITED(status)) {
            if (result->time > ltime)
                result->code = EXIT_TLE;
            else if (result->memory > memory)
                result->code = EXIT_MLE;
            else
                result->code = EXIT_AC;
            return;
        }

        /* 子进程被信号终止 */
        if (WIFSIGNALED(status)) {
            result->code = EXIT_RE;
            g_string_printf(result->err, "%s %d", feedback[WTERMSIG(status)],
                    WTERMSIG(status));
            return;
        }

        /* 除了结束外，期待的是停止状态，如果不是 */
        if (!WIFSTOPPED(status)) {
            kill(child, SIGKILL);
            result->code = EXIT_IE;
            g_string_assign(result->err, "unknow user program status");
            return;
        }

        /* 判断停止的信号 */
        signo = WSTOPSIG(status);
        /* 如果不是由于系统调用产生的信号 */
        if (signo != SIGTRAP) {
            if (signal_rule[signo] == SIG_NOTHING) {
                ptrace(PTRACE_CONT, child, NULL, NULL);
                continue;
            }
            else if (signal_rule[signo] == SIG_DELIVER) {
                ptrace(PTRACE_CONT, child, NULL, signo);
                continue;
            }

            /* 信号规则是终止进程 */
            kill(child, SIGKILL);
            if (signo == SIGXFSZ) {
                result->code = EXIT_OLE;
            } else if (signo == SIGXCPU) {
                result->time = ltime;
                result->code = EXIT_TLE;   
            } else {
                result->code = EXIT_RE;
                g_string_printf(result->err, "%s %d", feedback[signo], signo);
            }
            return;
        }

        /* 由系统调用产生的信号停止，获取调用号 */
        ptrace(PTRACE_GETREGS, child, NULL, &regs);
        endflag ^= 1;
        /* endflag == 0 进入系统调用前，判断是否允许该调用 */
        if (endflag == 0 && syscall_rule[regs.orig_eax]) {
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
            continue;
        } else if (endflag == 0) {
            kill(child, SIGKILL);
            result->code = EXIT_RE;
            g_string_printf(result->err, "Invalid Syscall %ld", regs.orig_eax);
            return;
        }

        /* endflag == 1 系统调用之后，判断内存使用 */
        if (result->memory > memory) {
            kill(child, SIGKILL);
            result->code = EXIT_MLE;
            return;
        }

        ptrace(PTRACE_SYSCALL, child, NULL, NULL);
    } // end while
} // end function

static void
setup_child()
{
    char ** env;
    
    /* io需要在降权之前设置，因为需要打开一个输出文件 */
    setup_io();
    
    /* 关闭所有继承的附加组id， 设置有gid，uid */
    if (setgroups(0, NULL) != 0 ||
            setresgid(pwd->pw_gid, pwd->pw_gid, pwd->pw_gid) != 0 ||
            setresuid(pwd->pw_uid, pwd->pw_uid, pwd->pw_uid) != 0)
        exit(0);

    env = g_get_environ();
    chdir(workdir);
    g_slist_foreach(resource_rule, foreach_resource, NULL);
    g_slist_foreach(environ_rule, foreach_environ, &env);
    setup_resource();
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);

    execvpe(command[0], command, (char * const *)env);
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

static void
alarm_func(int signo)
{
    if (signo == SIGALRM)
        longjmp(jbuf, 1);
}

static void
foreach_resource(gpointer data, gpointer user_data)
{
    Resource * ptr;

    ptr = (Resource *)data;
    /* 以下三个资源配置文件指定无效 */
    if (ptr->resource == RLIMIT_CPU || ptr->resource == RLIMIT_AS ||
            ptr->resource == RLIMIT_FSIZE)
        return;
    setrlimit(ptr->resource, &ptr->lmt);
}

static void
foreach_environ(gpointer data, gpointer user_data)
{
    Environ * ptr;
    char *** env;

    ptr = (Environ *)data;
    env = (char ***)user_data;
    if (!strcmp(ptr->value, ""))
        *env = g_environ_unsetenv(*env, ptr->key);
    else
        *env = g_environ_setenv(*env, ptr->key, ptr->value, TRUE);
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
