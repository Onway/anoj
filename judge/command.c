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
extern int preused;
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

/* 子进程在执行exec调用之前的内存和时间，用于最后的资源统计 */
static int pre_time;
static int pre_memory;

static jmp_buf jbuf;

static int get_vmsize(int child);       /* /proc/pid/statm 虚拟内存值 */
static void alarm_func(int signo);      /* alarm 信号处理函数 */
static void trace_child(int child);     /* 子进程信号和系统调用捕捉 */
static void setup_child();              /* 子进程在exec前的设置 */
static void setup_io();                 /* 子进程io重定向 */
static void setup_resource();           /* 子进程资源特定设置 */

/* 启动子进程，安装时钟信号，获取子进程在exec之前的时间内存 */
void execute_command();

/* 子进程资源和环境变量设置，从配置文件指定 */
static void foreach_resource(gpointer data, gpointer user_data);
static void foreach_environ(gpointer data, gpointer user_data);

void
execute_command()
{
    int status;
    pid_t child;
    struct rusage used;
    int vmsize;
    int signo;
    struct user_regs_struct regs;

    /* for java */
    int len = 0;
    char xmn[100];
    char ** tmp;
    char * const * ix;

    /* for java */
    if (!strcmp(lang, "JAVA")) {
        for (ix = command; *ix != NULL; ++ix)
            ++len;
        tmp = (char **)malloc((len + 3) * sizeof(char *));

        tmp[0] = (char *)malloc(strlen(command[0]) + 1);
        strcpy(tmp[0], command[0]);

        sprintf(xmn, "-Xms%dk", memory);
        tmp[1] = (char *)malloc(strlen(xmn) + 1);
        strcpy(tmp[1], xmn);

        sprintf(xmn, "-Xmx%dk", memory);
        tmp[2] = (char *)malloc(strlen(xmn) + 1);
        strcpy(tmp[2], xmn);

        for (len = 3, ix = command + 1; *ix != NULL; ++ix, ++len) {
            tmp[len] = (char *)malloc(strlen(*ix) + 1);
            strcpy(tmp[len], *ix);
        }
        tmp[len] = NULL;

        command = (char * const *)tmp;
        memory = 1024 * 1024;    /* 1G */
    }

    child = fork();
    if (child == 0)
       setup_child();

    wait3(&status, 0, &used);
    if (WIFEXITED(status)) {
        switch (WEXITSTATUS(status)) {
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

    if (!WIFSTOPPED(status)) {
        kill(child, SIGKILL);
        result->code = EXIT_IE;
        g_string_assign(result->err,
                "unknow child status before exec syscall");
        return;
    }

    signo = WSTOPSIG(status);
    ptrace(PTRACE_GETREGS, child, NULL, &regs);
    if (signo != SIGTRAP && regs.orig_eax != SYS_execve) {
        kill(child, SIGKILL);
        result->code = EXIT_IE;
        g_string_assign(result->err,
                "unexpect child stop before exec syscall");
        return;
        
    }

    /* memory的值不应该包含程序空载时的值 */
    if ((vmsize = get_vmsize(child)) > memory + preused) {
        kill(child, SIGKILL);
        result->code = EXIT_MLE;
        result->memory = vmsize;
        return;
    }

    pre_time = used.ru_utime.tv_sec * 1000 + 
        used.ru_utime.tv_usec / 1000 +
        used.ru_stime.tv_sec * 1000 +
        used.ru_stime.tv_usec / 1000;
    pre_memory = used.ru_minflt * getpagesize() / 1024;

    if (setjmp(jbuf)) {
        kill(child, SIGKILL);
        result->code = EXIT_TLE;
        return;
    }
    signal(SIGALRM, alarm_func);

    if (ltime % 1000)
        alarm(ltime / 1000 + 2);
    else
        alarm(ltime / 1000 + 1);
    ptrace(PTRACE_SYSCALL, child, NULL, NULL);
    trace_child(child);
    alarm(0);

    return;
}

static void
trace_child(int child)
{
    int status;
    int lst_time;
    int lst_memory;
    int vmsize;
    int signo;
    int endflag = 1;    /* endflag == 0 为进入系统调用 */
    struct rusage used;
    struct user_regs_struct regs;

    while (1) {
        wait3(&status, 0, &used);

        if (WIFEXITED(status)) {
            lst_time = used.ru_utime.tv_sec * 1000 + 
                used.ru_utime.tv_usec / 1000 +
                used.ru_stime.tv_sec * 1000 +
                used.ru_stime.tv_usec / 1000;
            lst_memory = used.ru_minflt * getpagesize() / 1024;
            result->time = lst_time - pre_time;
            result->memory = lst_memory - preused - pre_memory;
            if (result->time > ltime) {
                result->code = EXIT_TLE;
                return;
            }
            if (result->memory > memory) {
                result->code = EXIT_MLE;
                return;
            }
            result->code = EXIT_AC;
            return;
        }

        if (WIFSIGNALED(status)) {
            result->code = EXIT_RE;
            g_string_assign(result->err, feedback[WTERMSIG(status)]);
            g_string_printf(result->err,
                    "killed by signal %d", WTERMSIG(status));
            return;
        }

        if (!WIFSTOPPED(status)) {
            kill(child, SIGKILL);
            result->code = EXIT_IE;
            g_string_assign(result->err, "unknow user program status");
            return;
        }

        signo = WSTOPSIG(status);
        if (signo != SIGTRAP) {
            if (signal_rule[signo] == SIG_NOTHING) {
                ptrace(PTRACE_CONT, child, NULL, NULL);
                continue;
            }
            else if (signal_rule[signo] == SIG_DELIVER) {
                ptrace(PTRACE_CONT, child, NULL, signo);
                continue;
            }

            kill(child, SIGKILL);
            if (signo == SIGXFSZ) {
                result->code = EXIT_OLE;
            } else if (signo == SIGXCPU) {
                result->code = EXIT_TLE;   
            } else {
                result->code = EXIT_RE;
                g_string_assign(result->msg, feedback[signo]);
            }
            return;
        }

        ptrace(PTRACE_GETREGS, child, NULL, &regs);
        endflag ^= 1;
        
        /* endflag == 0 进入系统调用前 */
        if (endflag == 0 && syscall_rule[regs.orig_eax]) {
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
            continue;
        } else if (endflag == 0) {
            kill(child, SIGKILL);
            result->code = EXIT_RE;
            g_string_printf(result->err, "user program used invalid syscall %ld",
                    regs.orig_eax);
            return;
        }

        /* endflag == 1 系统调用之后 */
        if ((vmsize = get_vmsize(child)) > memory + preused) {
            kill(child, SIGKILL);
            result->code = EXIT_MLE;
            result->memory = vmsize;
            return;
        }

        ptrace(PTRACE_SYSCALL, child, NULL, NULL);
    } // end while
} // end function

static void
setup_child()
{
    char ** env;
    env = g_get_environ();

    chdir(workdir);
    setup_io();
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
    if (infd == -1)
        exit(2);

    outfd = open(output->str, O_WRONLY | O_CREAT, FMODE);
    if (outfd == -1)
        exit(3);

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
}

static int
get_vmsize(pid_t child)
{
    FILE *fd;
    int vmsize;
    char tmpbuf[64];

    sprintf(tmpbuf, "/proc/%d/statm", child);
    if ((fd = fopen(tmpbuf, "r")) == NULL)
        return 0;

    fscanf(fd, "%d", &vmsize);
    fclose(fd);

    return vmsize * getpagesize() / 1024;
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
    
    /* these resource will be set later */
    if (ptr->resource == RLIMIT_CPU || ptr->resource == RLIMIT_AS ||
            ptr->resource == RLIMIT_FSIZE)
        return;
    
    /* for java */
    if (ptr->resource == RLIMIT_NPROC && !strcmp(lang, "JAVA"))
        ptr->lmt.rlim_cur = ptr->lmt.rlim_max = 350;
    if (ptr->resource == RLIMIT_NOFILE && !strcmp(lang, "JAVA"))
        ptr->lmt.rlim_cur = ptr->lmt.rlim_max = 20;

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
