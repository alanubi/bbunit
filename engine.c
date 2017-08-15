#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <bbdump/specifier.h>
#include <bbdump/ansicolor.h>
#include <bbdump/switch.h>
#include <bbunit/engine.h>
#include <bbunit/internal.h>

const char *const bbunit_version = ANSICOLOR_XSTR(BBUNIT_VERSION);

static float timespec_dif(struct timespec *former, struct timespec *latter)
{
	return
		(latter->tv_sec + (float)latter->tv_nsec / 1000000000L) -
		(former->tv_sec + (float)former->tv_nsec / 1000000000L);
}

static void timeval_fromfloat(float time_sec, struct timeval *tv)
{
	tv->tv_sec = (int)time_sec;
	tv->tv_usec = (long)(time_sec * 1000000L) % 1000000L;
}

static void writemsg(void *msg, size_t size, int writefd)
{
	char *addr = msg;
	while (size) {
		ssize_t n = write(writefd, addr, size);
		if (n == 0 || n == -1)
			abort();
		addr += n;
		size -= n;
	}
}

/* Return 1 if EOF, 0 otherwise. */
static int readmsg(void *msg, size_t size, int readfd)
{
	char *addr = msg;
	while (size) {
		ssize_t n = read(readfd, addr, size);
		if (n == 0)
			return 1;
		if (n == -1)
			abort();
		addr += n;
		size -= n;
	}
	return 0;
}

/********************/

/* Pipe to transport messages to parent process. */
static int bbunit_writefd;

enum msgkind {
	MSGKIND_VERDICT,
	MSGKIND_FAILHERE,
	MSGKIND_NAME,
	MSGKIND_RAISE
};

struct position {
	const char *file;
	int line;
};

struct raise {
	enum bbunit_verdict verdict;
	unsigned long num_off;
	unsigned long num_bad;
};

static INLINE void writemsgkind(enum msgkind kind)
{
	writemsg(&kind, sizeof(kind), bbunit_writefd);
}

static NORETURN void bbunit_pass()
{
	enum bbunit_verdict verdict = BBUNIT_PASS;

	writemsgkind(MSGKIND_VERDICT);
	writemsg(&verdict, sizeof(verdict), bbunit_writefd);
	close(bbunit_writefd);
	exit(0);
}

NORETURN void bbunit_skip()
{
	enum bbunit_verdict verdict = BBUNIT_SKIP;

	writemsgkind(MSGKIND_VERDICT);
	writemsg(&verdict, sizeof(verdict), bbunit_writefd);
	close(bbunit_writefd);
	exit(0);
}

NORETURN void bbunit_failhere(const char *file, int line)
{
	struct position pos;

	pos.file = file;
	pos.line = line;
	writemsgkind(MSGKIND_FAILHERE);
	writemsg(&pos, sizeof(pos), bbunit_writefd);
	close(bbunit_writefd);
	exit(0);
}

void bbunit_name(const char *name)
{
	writemsgkind(MSGKIND_NAME);
	writemsg(&name, sizeof(name), bbunit_writefd);
}

static void bbunit_raise(enum bbunit_verdict verdict,
	unsigned long num_off, unsigned long num_bad)
{
	struct raise raise;

	raise.verdict = verdict;
	raise.num_off = num_off;
	raise.num_bad = num_bad;
	writemsgkind(MSGKIND_RAISE);
	writemsg(&raise, sizeof(raise), bbunit_writefd);
}

/********************/

/* Nesting level of a current child process.
 * The value is just a foolproof. */
static unsigned bbunit_nesting = 0U - 1U;

/* Return 1 if EOF, 0 otherwise. */
static int read_grandchild(int readfd, struct bbunit_info *info)
{
	int eof;
	enum msgkind kind;

	eof = readmsg(&kind, sizeof(kind), readfd);
	if (eof) return 1;

	switch (kind) {
	when (MSGKIND_VERDICT) {
		enum bbunit_verdict verdict;
		eof = readmsg(&verdict, sizeof(verdict), readfd);
		if (eof) return 1;

		info->verdict = verdict;
	}
	when (MSGKIND_FAILHERE) {
		struct position pos;
		eof = readmsg(&pos, sizeof(pos), readfd);
		if (eof) return 1;

		info->file = pos.file;
		info->line = pos.line;
		info->verdict = BBUNIT_FAIL;
	}
	when (MSGKIND_NAME) {
		eof = readmsg(&info->name, sizeof(info->name), readfd);
		if (eof) return 1;
	}
	when (MSGKIND_RAISE) {
		struct raise raise;
		eof = readmsg(&raise, sizeof(raise), readfd);
		if (eof) return 1;

		if (raise.verdict == BBUNIT_SKIP) {
			++info->num_off;
			return 0;
		}
		info->num_off += raise.num_off;
		info->num_bad += raise.num_bad;
		if (raise.verdict != BBUNIT_PASS)
			++info->num_bad;
	}
	}
	return 0;
}

static int child(void (*test)(), float timelimit_sec, int flags, int writefd)
{
	int res;
	int pipefd[2];
	int old_writefd;
	pid_t pid;
	struct itimerval itv;
	struct timespec ts;

	if (pipe(pipefd) != 0)
		return -1;
	old_writefd = bbunit_writefd;
	bbunit_writefd = pipefd[1];

	if (flags & BBUNIT_SUBORDINATE) {
		++bbunit_nesting;
		if (bbunit_nesting == 0)
			abort();
	} else {
		bbunit_nesting = 0;
	}

	itv.it_interval.tv_sec = 0;
	itv.it_interval.tv_usec = 0;
	timeval_fromfloat(timelimit_sec, &itv.it_value);
	clock_gettime(CLOCK_MONOTONIC, &ts);

	if ((pid = fork()) == 0) {
		if (!(flags & BBUNIT_UNLIMITED_TIME))
			setitimer(ITIMER_REAL, &itv, 0);
		close(pipefd[0]);
		test();
		bbunit_pass();
	}

	close(pipefd[1]);
	bbunit_writefd = old_writefd;
	if (pid == -1) {
		res = -1;
	} else {
		int eof;
		struct bbunit_info info;
		int wstatus;
		struct timespec ts2;

		info.verdict = BBUNIT_CRASH;
		info.num_off = 0;
		info.num_bad = 0;
		info.name = NULL;
		info.nesting = bbunit_nesting;

		do {
			eof = read_grandchild(pipefd[0], &info);
		} while (!eof);

		waitpid(pid, &wstatus, 0);
		clock_gettime(CLOCK_MONOTONIC, &ts2);
		info.time_sec = timespec_dif(&ts, &ts2);
		if (WIFSIGNALED(wstatus) && WTERMSIG(wstatus) == SIGALRM)
			info.verdict = BBUNIT_TIME;

		writemsg(&info, sizeof(info), writefd);
		if (flags & BBUNIT_SUBORDINATE)
			bbunit_raise(info.verdict, info.num_off, info.num_bad);

		res = 0;
	}
	close(pipefd[0]);
	return res;
}

int bbunit_execute(void (*test)(), float timelimit_sec, int flags,
	struct bbunit_info *info)
{
	int res;
	int pipefd[2];
	pid_t pid;

	if (pipe(pipefd) != 0)
		return -1;

	if ((pid = fork()) == 0) {
		close(pipefd[0]);
		res = child(test, timelimit_sec, flags, pipefd[1]);
		close(pipefd[1]);
		exit((res == 0)? 0: 1);
	}

	close(pipefd[1]);
	if (pid == -1) {
		res = -1;
	} else {
		int eof;
		int wstatus;

		eof = readmsg(info, sizeof(*info), pipefd[0]);
		waitpid(pid, &wstatus, 0);
		res = (eof || WEXITSTATUS(wstatus) != 0)? -1: 0;
	}
	close(pipefd[0]);
	return res;
}
