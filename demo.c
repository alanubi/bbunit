/* Included only for "abort()". */
#include <stdlib.h>

/* To use the library, just include this file. */
#include <bbunit/bbunit.h>

/* A test should be of this type. */
static void testall()
{
	/* You can give a name to the test if you wish.
	 * Make sure this line will be executed.
	 * The name must be a static immutable string. */
	bbname("All tests");

	/* Tests and testsuites are executed in this way.
	 * They do not need to be declared, so they can be placed
	 * in a separate file without redundant headers. */
	bbtest(typical);
	bbtest(version);
	bbtest(cover);
	bbtest(testtime);
}

int main()
{
	/* Top-level testsuites must be executed in a special way. */
	bbmaintest(testall);
	return 0;
}

void typical()
{
	int sum = 0;
	int i = 0, n = 10;

	bbname("Typical test");

	/* Typical test looks like this. */
	for (; i != n; ++i) {
		sum += i;
		bbcheck(sum >= 0);
	}
	bbcheck(sum == 45);
}

void version()
{
	/* The version of library is stored here,
	 * in a static immutable string. */
	bbname(bbunit_version);
}

static void naked_pass()
{
	bbname("Naked pass");

	/* If nothing happens, the test passes. */
}

static void naked_skip()
{
	bbname("Naked skip");

	/* This will disable the test. */
	bbskip();

	/* Unreachable location. */
}

static void naked_fail()
{
	bbname("Naked fail");

	/* This will cause the test to fail.
	 * The place of the failure would be indicated. */
	bbfail();

	/* Unreachable location. */
}

static void naked_checkfail()
{
	bbname("Naked check-fail");

	/* Another way to fail the test. */
	bbcheck(0);

	/* Unreachable location. */
}

static void naked_hang()
{
	bbname("Naked hang");

	/* The execution time of this test or one of
	 * the containing testsuites must be limited,
	 * otherwise the whole testing will hang. */
	while (1)
		(void)0;

	/* Unreachable location. */
}

static void naked_crash()
{
	bbname("Naked crash");

	/* If a test crashes, that will be reported.
	 * It is safe for the rest of the program. */
	abort();
}

static void naked()
{
	/* A test can be nameless. */
	bbtest(naked_pass);
	bbtest(naked_skip);
	bbtest(naked_fail);
	bbtest(naked_checkfail);
	bbtesttime(naked_hang, 0.2f);
	bbtest(naked_crash);
	bbtesttime(naked_pass, 0.2f);
	bbtesttime(naked_skip, 0.2f);
}

static void suitefail()
{
	/* Usual call, not a subtest. */
	naked();

	/* Normally you would like a testsuite to pass.
	 * This is just for demonstration. */
	bbfail();
}

void cover()
{
	bbname("Cover everything");
	bbtest(naked);
	bbtest(suitefail);

	/* Disabling a testsuite covers everything.
	 * Nevertheless, the reports will be printed. */
	bbskip();
}

static void testtime_slow()
{
	int a = 0, b = 0;
	int i = 0, n = 20000000;

	bbname("Time-consuming test");

	/* For time-consuming tests, the time will be printed. */
	for (; i != n; ++i) {
		volatile int t = b;
		b = a + 1;
		a = t + 1;
	}
	bbcheck(a == n);
	bbcheck(b == n);
}

void testtime()
{
	int i = 0, n = 4;

	/* The time consumed by sub-tests will sum up. */
	for (; i != n; ++i)
		bbtest(testtime_slow);
}
