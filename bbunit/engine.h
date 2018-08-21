#ifndef BBUNIT_ENGINE_H_
#define BBUNIT_ENGINE_H_

#ifndef NOINCLUDE
#define NOINCLUDE
#include <bbmacro/static.h>
#undef NOINCLUDE
#endif

/****************************************
 * Contents of a test procedure.
 ****************************************/

/* Version is a static immutable string. */
extern const char *const bbunit_version;

/* A test may be skipped. */
BBNORETURN void bbunit_skip(void);

/* A test may fail, directly or by failing an assertion.
 * `file` must be a static immutable string. */
BBNORETURN void bbunit_failhere(const char *file, int line);

#define bbunit_fail() \
	bbunit_failhere(__FILE__, __LINE__)

#define bbunit_check(value) \
do { \
	if (!(value)) \
		bbunit_fail(); \
} while (0)

/* A test may have a name. It must be a static immutable string. */
void bbunit_name(const char *name);

/****************************************
 * Execution of a test or a testsuite.
 ****************************************/

/* Argument for a test execution. */
enum bbunit_flags {
	BBUNIT_SUBORDINATE = 1 << 0, /* Is the test a part of a testsuite. */
	BBUNIT_UNLIMITED_TIME = 1 << 1, /* Is timelimit argument ignored. */
	BBUNIT_NOFLAGS = 0 /* Do we really need this zero flag here? */
};

/* Concise verdict of a test execution. */
enum bbunit_verdict {
	BBUNIT_PASS, /* Successful execution, nothing has happened. */
	BBUNIT_SKIP, /* A programmer decided not to execute the test. */
	BBUNIT_FAIL, /* An assertion within the test has failed. */
	BBUNIT_CRASH, /* A runtime error has happened during the execution. */
	BBUNIT_TIME /* The test has been interrupted by the timelimit. */
};

/* Complete information about the testing. */
struct bbunit_info {
	/* Concise verdict of the testing. */
	enum bbunit_verdict verdict;

	/* Number of skipped and bad (not passed or skipped) tests
	 * in the testsuite and its subsuites. */
	unsigned long num_off;
	unsigned long num_bad;

	/* The position in the test where an assertion has failed.
	 * Makes sense only if the verdict is BBUNIT_FAIL.
	 * `file` must be a static immutable string. */
	const char *file;
	int line;

	/* Optional name of the test.
	 * It must be a static immutable string or NULL. */
	const char *name;

	/* Nesting level of the testing.
	 * Top-level testsuite has level zero. */
	unsigned nesting;

	/* Time consumed by the testing, in seconds. */
	float time_sec;
};

/* Execute a test in a testsuite or separately.
 * On error return -1, normally return 0. */
int bbunit_execute(void (*test)(void), float timelimit_sec, int flags,
	struct bbunit_info *info);

#endif
