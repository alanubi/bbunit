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

#ifndef BBUNIT_ENGINE_H_
#define BBUNIT_ENGINE_H_

#ifndef NOINCLUDE
#define NOINCLUDE
#include <bbdump/specifier.h>
#undef NOINCLUDE
#endif

/* Argument to a test execution. */
enum bbunit_flags {
	BBUNIT_SUBORDINATE = 1 << 0,
	BBUNIT_UNLIMITED_TIME = 1 << 1,
	BBUNIT_NOFLAGS = 0
};

/* Version is a static immutable string. */
extern const char *const bbunit_version;

/* A test may be skipped. */
NORETURN void bbunit_skip();

/* A test may fail, directly or by failing an assertion.
 * `file` must be a static immutable string. */
NORETURN void bbunit_failhere(const char *file, int line);

#define bbunit_fail() \
	bbunit_failhere(__FILE__, __LINE__)

#define bbunit_check(value) \
do { \
	if (!(value)) \
		bbunit_fail(); \
} while (0)

/* A test may have a name. It must be a static immutable string. */
void bbunit_name(const char *name);

#endif
