/*
 * -*- coding: utf-8 -*-
 * Filename      : judge.c
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-13
 */

#include "judge.h"

extern int ltime;
extern int memory;
extern char * workdir;
extern Result * result;
extern GString * output;
extern GString * answer;
extern GSList * environ_rule;
extern GSList * resource_rule;
extern gboolean signal_rule[];
extern int syscall_rule[];

static jmp_buf jbuf;

static void alarm_func(int signo);
static void trace_child(int child);
static void setup_child();
static void setup_io();
static void setup_resource();
static void foreach_resource(gpointer data, gpointer user_data);
static void foreach_environ(gpointer data, gpointer user_data);
static Status compare_string(char * outstr, char * ansstr);

void
normal_judge()
{
    char * outstr, * ansstr;
    GError * gerr = NULL;

    g_file_get_contents(output->str, &outstr, NULL, &gerr);
    if (gerr) {
        g_string_assign(result->err, gerr->message);
        g_error_free(gerr);
        return;
    }

    g_file_get_contents(answer->str, &ansstr, NULL, &gerr);
    if (gerr) {
        g_string_assign(result->err, gerr->message);
        g_error_free(gerr);
        return;
    }

    result->code = compare_string(outstr, ansstr);
    return;
}

void
special_judge()
{
    int wtime = 0;
    pid_t child;
    GSList * ele;

    ele = resource_rule;
    while ((ele = g_slist_next(ele)) != NULL)
        if (((Resource *)ele)->resource == RLIMIT_CPU) {
            wtime = ((Resource *)ele)->lmt.rlim_cur;
            break;
        }
    if (wtime == 0) wtime = ltime / 1000 + 3;

    child = fork();
    if (child == 0)
        setup_child();

    if (setjmp(jbuf)) {
        kill(child, SIGKILL);
        result->code = EXIT_IE;
        g_string_sprintf(result->err, "spj time limited");
        return;
    }

    signal(SIGALRM, alarm_func);
    alarm(wtime);
    trace_child(child);
    alarm(0);

    return;
}

static Status
compare_string(char * outstr, char * ansstr)
{
	int i, j;
    int len1, len2;

    len1 = strlen(outstr);
    len2 = strlen(ansstr);

	/* 去掉字符串尾部的不可打印字符 */
	for (i = len1 - 1; i >= 0; --i)
		if (!is_nonprint(outstr[i]))
				break;
	if (i < 0)
		return EXIT_WA;
	else
		len1 = i + 1;

	for (j = len2 - 1; j >= 0; --j)
		if (!is_nonprint(ansstr[j]))
			break;
	if (j < 0)
		return EXIT_WA;
	else
		len2 = j + 1;

	/* 进行一次整体比较，如果全部相同，则AC */
	for (i = 0, j = 0; i < len1 && j < len2; ++i, ++j)
		if (outstr[i] != ansstr[j])
			break;
	if (i >= len1 && j >= len2)
		return EXIT_AC;

	/* 跳过不可打印字符进行一次比较，相同则PE，否则WA */
	for (i = 0, j = 0; i < len1 && j < len2;) {
		if (is_nonprint(outstr[i])) {
			++i;
			continue;
		}
		if (is_nonprint(ansstr[j])) {
			++j;
			continue;
		}

		if (outstr[i] != ansstr[j])
			return EXIT_WA;
		
		++i;
		++j;
	}

	/* 全部比较完才是PE，否则其中之一是子串，WA */
	if (i >= len1 && j >= len2)
		return EXIT_PE;
	
	return EXIT_WA;
}

static void
alarm_func(int signo)
{
    if (signo == SIGALRM)
        longjmp(jbuf, 1);
}

static void
trace_child(int child)
{
    int status;
    int signo;
    struct user_regs_struct regs;

    while (1) {
        wait(&status);

        if (WIFEXITED(status)) {
            result->code = WEXITSTATUS(status) % 3;
            return;
        } else if (WIFSIGNALED(status)) {
            result->code = EXIT_IE;
            g_string_printf(result->err, "spj killed by signal %d",
                    WTERMSIG(status));
            return;
        } else if (WIFSTOPPED(status)) {
            signo = WSTOPSIG(status);
            ptrace(PTRACE_GETREGS, child, NULL, &regs);

            if (signo != SIGTRAP) {
                if (signal_rule[signo] == SIG_NOTHING)
                    ptrace(PTRACE_CONT, child, NULL, NULL);
                else if (signal_rule[signo] == SIG_DELIVER)
                    ptrace(PTRACE_CONT, child, NULL, &signo);
                else {
                    kill(child, SIGKILL);
                    result->code = EXIT_IE;
                    g_string_printf(result->err,
                            "spj recieved invalid signal %d", signo);
                    return;
                }
                continue;
            }
            if (regs.orig_eax >= 0 && regs.orig_eax < SYSCALL_NUM) {
                if (!syscall_rule[regs.orig_eax]) {
                    kill(child, SIGKILL);
                    result->code = EXIT_IE;
                    g_string_printf(result->err,
                            "spj used invalid syscall %ld", regs.orig_eax);
                    return;
                }
            }
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
        } else {
            kill(child, SIGKILL);
            result->code = EXIT_IE;
            g_string_printf(result->err,
                    "unknow spj status");
            return;
        }
    } // end while;
} // end function;

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

    execle(answer->str, answer->str, (char *)NULL, (char * const *)env);
    exit(2);
}

static void
setup_io()
{
    int infd;

    infd = open(output->str, O_RDONLY);
    dup2(infd, STDIN_FILENO);

    close(infd);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

static void
setup_resource()
{
    struct rlimit t;

    t.rlim_cur = t.rlim_max = 0;
    setrlimit(RLIMIT_FSIZE, &t);

    getrlimit(RLIMIT_CPU, &t);
    if (t.rlim_cur == RLIM_INFINITY || t.rlim_max == RLIM_INFINITY) {
        t.rlim_cur = t.rlim_max = ltime / 1000 + 2;
        setrlimit(RLIMIT_CPU, &t);
    }

    getrlimit(RLIMIT_AS, &t);
    if (t.rlim_cur == RLIM_INFINITY || t.rlim_max == RLIM_INFINITY) {
        t.rlim_cur = t.rlim_max = memory * 1024;
        setrlimit(RLIMIT_CPU, &t);
    }
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
