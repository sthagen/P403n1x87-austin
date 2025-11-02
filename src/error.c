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

#define ERROR_C

#include <stdlib.h>

#include "error.h"
#include "platform.h"

#define MAXERROR 15

typedef struct {
    char* msg;
    bool  fatal;
} error_info_t;

const error_info_t _error_info_tab[MAXERROR] = {
    {"No error",                false}, /* OK */
    {"Operating system error",  true }, /* OS */
    {"Permission error",        true }, /* PERM */
    {"Memory copy error",       true }, /* MEMCOPY */
    {"Memory allocation error", true }, /* MALLOC */
    {"I/O error",               true }, /* IO */
    {"Command line error",      true }, /* CMDLINE */
    {"Environment error",       true }, /* ENV */
    {"Value error",             false}, /* VALUE */
    {"Null pointer error",      true }, /* NULL */
    {"Python version error",    true }, /* VERSION */
    {"Binary analysis error",   false}, /* BINARY */
    {"Python object error",     false}, /* PYOBJECT */
    {"VM maps error",           false}, /* VM */
    {"Iteration ended error",   false}, /* ITEREND */
};

const char*
error_get_msg(error_t n) {
    return n < MAXERROR ? _error_info_tab[n].msg : "<Unknown error>";
}

const bool
is_fatal(error_t n) {
    return n < MAXERROR ? _error_info_tab[n].fatal : false;
}
