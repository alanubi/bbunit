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

#ifndef BBUNIT_BBUNIT_H_
#define BBUNIT_BBUNIT_H_

#ifndef NOINCLUDE
#define NOINCLUDE
#include <bbdump/specifier.h>
#include <bbunit/engine.h>
#include <bbunit/report.h>
#undef NOINCLUDE
#endif

/* Commands to use in a test. */
#define bbskip  bbunit_skip  /* () */
#define bbfail  bbunit_fail  /* () */
#define bbcheck bbunit_check /* (value) */
#define bbname  bbunit_name  /* (name) */

/* Commands for executing a test. */
#define bbmaintesttime bbunit_maintesttime /* (testname, timelimit_sec) */
#define bbtesttime     bbunit_testtime     /* (testname, timelimit_sec) */
#define bbmaintest     bbunit_maintest     /* (testname) */
#define bbtest         bbunit_test         /* (testname) */

#endif
