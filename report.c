#include <stdlib.h>
#include <stdio.h>
#include <bbdump/ansicolor.h>
#include <bbdump/specifier.h>
#include <bbdump/switch.h>
#include <bbunit/engine.h>
#include <bbunit/internal.h>
#include <bbunit/report.h>

void bbunit_nicetest(void (*test)(), float timelimit_sec, int flags)
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
			printf(STYLE2(BOLD, G, "[ok]"));
		else
			printf(STYLE2(BOLD, R, "[%lu bad]"), info.num_bad);
	when (BBUNIT_SKIP)  fputs(STYLE2(BOLD, Y, "[skip]" ), stdout);
	when (BBUNIT_FAIL)  fputs(STYLE2(BOLD, R, "[fail]" ), stdout);
	when (BBUNIT_CRASH) fputs(STYLE2(BOLD, R, "[crash]"), stdout);
	when (BBUNIT_TIME)  fputs(STYLE2(BOLD, R, "[time]" ), stdout);
	}
	if (info.num_off != 0)
		printf(STYLE2(BOLD, Y, "(+%lu off)"), info.num_off);
	if (info.time_sec > 1e-3)
		printf(STYLE1(BOLD, "(%.3f s)"), info.time_sec);
	if (info.verdict == BBUNIT_FAIL)
		printf(" " STYLE1(BOLD, "%s:%d:"), info.file, info.line);
	if (info.name)
		printf(" " STYLE0("%s"), info.name);

	fputs("\n", stdout);
}
