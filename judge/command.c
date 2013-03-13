/*
 * -*- coding: utf-8 -*-
 * Filename      : command.c
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-13
 */

#include "command.h"

extern int ltime;
extern int fsize;
extern int memory;
extern int preused;
extern char * workdir;
extern char * const * command;
extern GString * input;
extern GString * output;
extern GString * answer;
extern GSList * resource_rule;
extern GSList * environ_rule;
extern int signal_rule[];
extern gboolean syscall_rule[];
extern Result * result;

static int pre_time;
static int pre_memory;
static jmp_buf jbuf;

static int get_vmsize(int child);
static void alarm_func(int signo);
static void trace_child(int child);
static void setup_child();
static void setup_io();
static void setup_resource();
static void foreach_resource(gpointer data, gpointer user_data);
static void foreach_environ(gpointer data, gpointer user_data);

void
execute_command()
{
    int status;
    pid_t child;
    struct rusage used;
    int signo;
    struct user_regs_struct regs;

    child = fork();
    if (child == 0)
       setup_child();

    wait3(&status, 0, &used);
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
    if (get_vmsize(child) > memory + preused) {
        kill(child, SIGKILL);
        result->code = EXIT_MLE;
        return;
    }
    pre_time = used.ru_utime.tv_sec * 1000 + 
        used.ru_utime.tv_usec / 1000 +
        used.ru_stime.tv_sec * 1000 +
        used.ru_stime.tv_usec / 1000;
    pre_memory = used.ru_minflt * getpagesize() / 1024;


    if (setjmp(jbuf)) {
        result->code = EXIT_TLE;
        return;
    }
    signal(SIGALRM, alarm_func);

    if (ltime % 1000)
        alarm(ltime / 1000 + 3);
    else
        alarm(ltime / 1000 + 2);
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
    int signo;
    int endflag = 1;
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
            result->code = EXIT_AC;
            result->time = lst_time - pre_time - preused;
            result->memory = lst_memory - pre_memory - preused;
            return;
        }

        if (WIFSIGNALED(status)) {
            result->code = EXIT_RE;
            g_string_printf(result->err, "user program killed by signal %d",
                    WTERMSIG(status));
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
                ptrace(PTRACE_CONT, child, NULL, &signo);
                continue;
            }
            
            kill(child, SIGKILL);
            result->code = EXIT_IE;
            g_string_assign(result->msg, "runtime error msg");
            return;
        }

        ptrace(PTRACE_GETREGS, child, NULL, &regs);
        endflag ^= 1;
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

        if (get_vmsize(child) > memory + preused) {
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

    env = g_get_environ();

    chdir(workdir);
    setup_io();
    g_slist_foreach(resource_rule, foreach_resource, NULL);
    g_slist_foreach(environ_rule, foreach_environ, &env);
    setup_resource();

    if (ptrace(PTRACE_TRACEME, 0, NULL, NULL))
        exit(1);

    execvpe(command[0], command, (char * const *)env);
    exit(2);
}

static void
setup_io()
{
    int infd, outfd;

    infd = open(input->str, O_RDONLY);
    outfd = open(output->str, O_WRONLY | O_CREAT, FMODE);

    dup2(infd, STDIN_FILENO);
    dup2(outfd, STDOUT_FILENO);
    
    close(infd);
    close(outfd);
    close(STDERR_FILENO);
}

static void 
setup_resource()
{
    struct rlimit t;

    if (ltime % 1000 == 0)
        t.rlim_cur = ltime / 1000 + 1;
    else
        t.rlim_cur = ltime / 1000 + 2;
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
