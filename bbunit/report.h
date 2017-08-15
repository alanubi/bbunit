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

#ifndef BBUNIT_REPORT_H_
#define BBUNIT_REPORT_H_

#ifndef NOINCLUDE
#define NOINCLUDE
#include <bbdump/specifier.h>
#include <bbunit/engine.h>
#undef NOINCLUDE
#endif

/* Execute a test and print a report. */
void bbunit_nicetest(void (*test)(), float timelimit_sec, int flags);

/* Execute a test and print a report
 * even without a forward declaration of the test. */
#define bbunit_nicetestname(testname, timelimit_sec, flags) \
do { \
	void testname(); \
	bbunit_nicetest(testname, timelimit_sec, flags); \
} while (0)

#define bbunit_maintesttime(testname, timelimit_sec) \
	bbunit_nicetestname(testname, timelimit_sec, BBUNIT_NOFLAGS)

#define bbunit_testtime(testname, timelimit_sec) \
	bbunit_nicetestname(testname, timelimit_sec, BBUNIT_SUBORDINATE)

#define bbunit_maintest(testname) \
	bbunit_nicetestname(testname, 0.0f, BBUNIT_UNLIMITED_TIME)

#define bbunit_test(testname) \
	bbunit_nicetestname(testname, 0.0f, \
		BBUNIT_SUBORDINATE | BBUNIT_UNLIMITED_TIME)

#endif
