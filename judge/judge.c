/*
 * -*- coding: utf-8 -*-
 * Filename      : judge.c
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-13
 */

#include "global.h"

extern Result * result;
extern GString * output;
extern GString * answer;
extern struct passwd *pwd;

static jmp_buf jbuf;

/* alarm 信号处理函数 */ 
static void alarm_func(int signo);
/* 字符串对比，用于判断ac, wa, pe */
static Status compare_string(char * outstr, char * ansstr);

/* 程序答案评测，启动子进程，安装闹钟 */
void special_judge();
void normal_judge();    /* 文本答案评测 */

void
normal_judge()
{
    char * outstr = NULL, * ansstr = NULL;
    GError * gerr = NULL;

    g_file_get_contents(output->str, &outstr, NULL, &gerr);
    if (gerr) {
        result->code = EXIT_IE;
        g_string_assign(result->err, gerr->message);
        g_error_free(gerr);
        return;
    }

    g_file_get_contents(answer->str, &ansstr, NULL, &gerr);
    if (gerr) {
        result->code = EXIT_IE;
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
    int infd;
    int status;
    pid_t child;

    child = fork();
    /* 子进程 */
    if (child == 0) {
        if (setgroups(0, NULL) != 0 ||
                setresgid(pwd->pw_gid, pwd->pw_gid, pwd->pw_gid) != 0 ||
                setresuid(pwd->pw_uid, pwd->pw_uid, pwd->pw_uid) != 0)
            abort();
        infd = open(output->str, O_RDONLY);
        dup2(infd, STDIN_FILENO);
        close(infd);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        execl(answer->str, answer->str, (char *)NULL);
    }

    /* 父进程 */
    signal(SIGALRM, alarm_func);
    if (setjmp(jbuf)) {
        kill(child, SIGKILL);
        result->code = EXIT_IE;
        g_string_sprintf(result->err, "spj time limit exceeded");
        return;
    }
    alarm(SPJTIME);

    /* 检测子进程状态 */
    wait(&status);
    if (!WIFEXITED(status)) {
        kill(child, SIGKILL);
        result->code = EXIT_IE;
        g_string_sprintf(result->err, "unknow spj status");
    } else {
        result->code = WEXITSTATUS(status) % 3;
    }

    alarm(0);
    return;
}

static Status
compare_string(char * outstr, char * ansstr)
{
	int i, j;
    int len1, len2;

    len1 = strlen(outstr);
    FILE * tmpfd = fopen("/tmp/result.txt", "w");
    fprintf(tmpfd, "%s", outstr);
    fclose(tmpfd);
    /*
    printf("%s", outstr);
    printf("============================\n");
    */
    len2 = strlen(ansstr);

    if (len2 == 0)
        return EXIT_WA;

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
