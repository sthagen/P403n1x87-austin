// This file is part of "austin" which is released under GPL.
//
// See file LICENCE or go to http://www.gnu.org/licenses/ for full license
// details.
//
// Austin is a Python frame stack sampler for CPython.
//
// Copyright (c) 2018 Gabriele N. Tornetta <phoenix1987@gmail.com>.
// All rights reserved.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <errno.h>
#include <stdbool.h>

#include "logging.h"

#define AUSTIN_EOK       0
#define AUSTIN_EOS       1
#define AUSTIN_EPERM     2
#define AUSTIN_EMEMCOPY  3
#define AUSTIN_EMALLOC   4
#define AUSTIN_EIO       5
#define AUSTIN_ECMDLINE  6
#define AUSTIN_EENV      7
#define AUSTIN_EVALUE    8
#define AUSTIN_ENULL     9
#define AUSTIN_EVERSION  10
#define AUSTIN_EBINARY   11
#define AUSTIN_EPYOBJECT 12
#define AUSTIN_EVM       13
#define AUSTIN_EITEREND  14

// ----------------------------------------------------------------------------
typedef int error_t;

#ifdef ERROR_C
__thread error_t austin_errno;
__thread char*   austin_error_msg = NULL;
#else
extern __thread error_t austin_errno;
extern __thread char*   austin_error_msg;
#endif // ERROR_C

/**
 * Get the message of the give message number.
 *
 * @param  error_t  the error number
 *
 * @return a pointer to the message as const char *.
 */
const char* error_get_msg(error_t);

/**
 * Get the message of the last error.
 *
 * @return a pointer to the message as const char *.
 */
#define get_last_error() error_get_msg(austin_errno)

/**
 * Determine if the given error is fatal or not.
 *
 * @param  error_t  the error number
 *
 * @return 1 if the error is fatal, 0 otherwise.
 */
const bool is_fatal(error_t);

/**
 * Log the last error
 */
#define log_error()                                                                     \
    {                                                                                   \
        (is_fatal(austin_errno) ? log_f("%s: %s", get_last_error(), austin_error_msg)   \
                                : log_e("%s: %s", get_last_error(), austin_error_msg)); \
    }

#define log_location() log_e("| at %s:%d in %s", __FILE__, __LINE__, __FUNCTION__)

/**
 * Set and log the given error.
 *
 * @param  error_t  the error to set and log.
 */
#define set_error(n, msg)                 \
    {                                     \
        austin_errno     = (AUSTIN_E##n); \
        austin_error_msg = (msg);         \
        log_error();                      \
    }

/**
 * Check if the current error is the given one.
 * @param  x  the error to check
 * @return    true if the current error is the given one, false otherwise.
 */
#define error_is(x) (austin_errno == AUSTIN_E##x)
