/*
  bbunit -- C library for unit testing
  Copyright (C) 2017 Alexey Gantura <ganturagantura@yandex.ru>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#ifndef BBUNIT_INTERNAL_H_
#define BBUNIT_INTERNAL_H_

#ifndef NOINCLUDE
#define NOINCLUDE
#include <bbdump/specifier.h>
#include <bbunit/engine.h>
#undef NOINCLUDE
#endif

enum bbunit_verdict {
	BBUNIT_PASS,
	BBUNIT_SKIP,
	BBUNIT_FAIL,
	BBUNIT_CRASH,
	BBUNIT_TIME
};

/* Complete information about the testing. */
struct bbunit_info {
	/* Concise verdict of the testing. */
	enum bbunit_verdict verdict;

	/* Number of skipped and bad (not passed or skipped)
	 * tests in the testsuite and its subsuites. */
	unsigned long num_off;
	unsigned long num_bad;

	/* Known position in the test.
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
int bbunit_execute(void (*test)(), float timelimit_sec, int flags,
	struct bbunit_info *info);

#endif
