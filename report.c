#include <stdlib.h>
#include <stdio.h>
#include <bbmacro/label.h>
#include <bbmacro/ansicolor.h>
#include <bbmacro/static.h>
#include <bbunit/engine.h>
#include <bbunit/report.h>

void bbunit_nicetest(void (*test)(void), float timelimit_sec, int flags)
{
	struct bbunit_info info;
	int res;
	unsigned i;

	res = bbunit_execute(test, timelimit_sec, flags, &info);
	if (res != 0) {
		fputs("Cannot run unit test\n", stderr);
		abort();
	}

	for (i = info.nesting; i != 0; --i)
		fputs(". ", stdout);

	switch (info.verdict) {
	when (BBUNIT_PASS)
		if (info.num_bad == 0)
			printf(ANSICOLOR_2(BOLD, G, "[ok]"));
		else
			printf(ANSICOLOR_2(BOLD, R, "[%lu bad]"), info.num_bad);
	when (BBUNIT_SKIP)  fputs(ANSICOLOR_2(BOLD, Y, "[skip]" ), stdout);
	when (BBUNIT_FAIL)  fputs(ANSICOLOR_2(BOLD, R, "[fail]" ), stdout);
	when (BBUNIT_CRASH) fputs(ANSICOLOR_2(BOLD, R, "[crash]"), stdout);
	when (BBUNIT_TIME)  fputs(ANSICOLOR_2(BOLD, R, "[time]" ), stdout);
	}
	if (info.num_off != 0)
		printf(ANSICOLOR_2(BOLD, Y, "(+%lu off)"), info.num_off);
	if (info.time_sec > 1e-3)
		printf(ANSICOLOR_1(BOLD, "(%.3f s)"), info.time_sec);
	if (info.verdict == BBUNIT_FAIL)
		printf(" " ANSICOLOR_1(BOLD, "%s:%d:"), info.file, info.line);
	if (info.name)
		printf(" " ANSICOLOR_0("%s"), info.name);

	fputs("\n", stdout);
}
