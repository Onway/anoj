/*
 * -*- coding: utf-8 -*-
 * Filename      : answer.c
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-13
 */

#include "answer.h"

extern int ltime;
extern Result * result;
extern GString * output;
extern GString * answer;
extern GSList * resource_rule;
extern gboolean signal_rule[];
extern int syscall_rule[];

static jmp_buf spj_env;
static Status compare_string(char * outstr, char * ansstr);
static void spj_alarm(int signo);
static void trace_special_child(int child);

void
judge_common_answer()
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
judge_special_answer()
{
    pid_t child;
    int wtime = 0;
    GSList * ele;

    ele = resource_rule;
    while ((ele = g_slist_next(ele)) != NULL)
        if (((Resource *)ele)->resource == RLIMIT_AS) {
            wtime = ((Resource *)ele)->lmt.rlim_cur;
            break;
        }
    if (wtime == 0) wtime = ltime / 1000 + 3;

    child = fork();
    if (child == 0)
        start_special_child();

    if (setjmp(spj_env)) {
        kill(child, SIGKILL);
        result->code = EXIT_IE;
        g_string_sprintf(result->err, "spj time limited");
        return;
    }

    signal(SIGALRM, spj_alarm);
    alarm(wtime);
    trace_special_child(child);
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
spj_alarm(int signo)
{
    if (signo == SIGALRM)
        longjmp(spj_env, 1);
}

static void
trace_special_child(int child)
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
