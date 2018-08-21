#ifndef BBUNIT_REPORT_H_
#define BBUNIT_REPORT_H_

#ifndef NOINCLUDE
#define NOINCLUDE
#include <bbmacro/static.h>
#include <bbunit/engine.h>
#undef NOINCLUDE
#endif

/****************************************
 * Command-line report of a testing.
 ****************************************/

/* Execute a test and print a report. */
void bbunit_nicetest(void (*test)(void), float timelimit_sec, int flags);

/* Execute a test and print a report
 * even without a forward declaration of the test. */
#define bbunit_nicetestname(testname, timelimit_sec, flags) \
do { \
	void testname(void); \
	bbunit_nicetest(testname, timelimit_sec, flags); \
} while (0)

/****************************************
 * Command shorthands.
 ****************************************/

/* Short commands to use inside a test procedure. */
#define bbskip  bbunit_skip  /* () */
#define bbfail  bbunit_fail  /* () */
#define bbcheck bbunit_check /* (value) */
#define bbname  bbunit_name  /* (name) */

/* Short commands for executing a test or a testsuite. */
#define bbmaintesttime(testname, timelimit_sec) \
	bbunit_nicetestname(testname, timelimit_sec, BBUNIT_NOFLAGS)

#define bbtesttime(testname, timelimit_sec) \
	bbunit_nicetestname(testname, timelimit_sec, BBUNIT_SUBORDINATE)

#define bbmaintest(testname) \
	bbunit_nicetestname(testname, 0.0f, BBUNIT_UNLIMITED_TIME)

#define bbtest(testname) \
	bbunit_nicetestname(testname, 0.0f, \
		BBUNIT_SUBORDINATE | BBUNIT_UNLIMITED_TIME)

/****************************************
 * Legacy. Do not use in new code.
 ****************************************/

#define bbunit_maintesttime bbmaintesttime /* (testname, timelimit_sec) */
#define bbunit_testtime     bbtesttime     /* (testname, timelimit_sec) */
#define bbunit_maintest     bbmaintest     /* (testname) */
#define bbunit_test         bbtest         /* (testname) */

#endif
