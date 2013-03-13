/*
 * -*- coding: utf-8 -*-
 * Filename      : judger.c
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-13
 */

#include "judger.h"

extern memory;
extern ltime;
extern Result * result;
extern int signal_rule[];
extern gboolean syscall_rule[];

static int pre_time;
static int pre_memory;
static jmp_buf user_jbuf;

static void user_alarm(int signo);
static int get_vmsize(int child);
static void trace_user_child(int child);

void
judge_user_program()
{
    int status;
    pid_t child;
    struct rusage used;
    int signo;
    struct user_regs_struct regs;

    child = fork();
    if (child == 0)
       start_user_child();

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
    if (get_vmsize(child) > memory) {
        kill(child, SIGKILL);
        result->code = EXIT_MLE;
        return;
    }
    pre_time = used.ru_utime.tv_sec * 1000 + 
        used.ru_utime.tv_usec / 1000 +
        used.ru_stime.tv_sec * 1000 +
        used.ru_stime.tv_usec / 1000;
    pre_memory = used.ru_minflt * getpagesize() / 1024;


    if (setjmp(user_jbuf)) {
        result->code = EXIT_TLE;
        return;
    }
    signal(SIGALRM, user_alarm);

    if (ltime % 1000)
        alarm(ltime / 1000 + 3);
    else
        alarm(ltime / 1000 + 2);
    ptrace(PTRACE_SYSCALL, child, NULL, NULL);
    trace_user_child(child);
    alarm(0);

    return;
}

static void
trace_user_child(int child)
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
            result->time = lst_time - pre_time;
            result->memory = lst_memory - pre_memory;
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

        if (get_vmsize(child) > memory) {
            kill(child, SIGKILL);
            result->code = EXIT_MLE;
            return;
        }

        ptrace(PTRACE_SYSCALL, child, NULL, NULL);
    } // end while
} // end function

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
user_alarm(int signo)
{
    if (signo == SIGALRM)
        longjmp(user_jbuf, 1);
}
