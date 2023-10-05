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

#ifndef MSG_H
#define MSG_H

#include <stdarg.h>
#include <stdio.h>

#include "platform.h"

#define URL(x) "\n    🌐 " x "\n\n"

const char * MCMDLINE = \
"👓 I need either a command to run or a PID to attach to";

const char * MMALLOC = \
"😵 Failed to allocate the memory for dumping frame stacks. This is pretty bad\n"
"as Austin doesn't require much memory to run. Try to free up some memory";


const char * MTIMEOUT = \
"👽 The process you are trying to %s doesn't seem to be a Python process. If you\n"
"are sure it is, try increasing the Interpreter state search timeout, or simply\n"
"try again. Sometimes it's just a matter of warming up the caches\n";

#if defined PL_UNIX
const char * MPERM =
#if defined PL_LINUX
"🔒 Insufficient permissions. Austin requires the use of sudo on Linux in order\n"
"to attach to a running Python process. Alternatively, you need to grant the\n"
"Austin binary the CAP_SYS_PTRACE capability. See\n"
#elif defined PL_MACOS
"🔒 Insufficient permissions. Austin requires the use of sudo on Mac OS to read\n"
"the memory of even its child processes. Furthermore, the System \n"
"Integrity Protection prevents Austin from working with Python binaries\n"
"installed in certain areas of the file system. In some cases, using a  virtual\n"
"environment can provide a solution. See\n"
#endif
URL("https://github.com/P403n1x87/austin#on-macos-1")
"for more details";
#endif // PL_UNIX


const char * MFORK =
"❌ Cannot launch the given command or it terminated too quickly";

const char * MATTACH = \
"🛑 Cannot attach to the given process. Make sure that the PID you have provided\n"
"is valid and that the process is still running";

const char * MERROR = \
"☠️ An unexpected error occurred. Please report the issue to\n"
URL("https://github.com/P403n1x87/austin/issues")
"and attach the logs generated by Austin";

const char * MNOPROC = \
"😵 No such process. Something weird just happened. If this happens again,\n"
"please open an issue at\n"
URL("https://github.com/P403n1x87/austin/issues");

const char * MNOPYTHON = \
"👾 It looks like you are trying to profile a process that is not a Python\n"
"process. Make sure that you are targeting the right application. If the Python\n"
"process is actually a child of the target process then use the -C option to\n"
"discover it automatically";

const char * MNOCHILDREN = \
"🚼 It looks like you are trying to profile a process that is not a Python\n"
"process, and that has not spawned any child Python processes. Make sure that\n"
"you are targeting the right application";

const char * MNOVERSION = \
"🔢 Cannot determine the version of the Python interpreter. This could be due\n"
"to the binary not being an actual Python binary, like uWSGI. Please report an\n"
"issue at\n"
URL("https://github.com/P403n1x87/austin/issues")
"together with details about the application that you were trying to profile,\n"
"like frameworks used, command line etc ...";

// ----------------------------------------------------------------------------
static inline void
_msg(const char * fmt, ...) {
  va_list args;

  fputc('\n', stderr);

  va_start(args, fmt);
    vfprintf(stderr, fmt, args);
  va_end(args);

  fputs(".\n", stderr);
}

#endif // MSG_H
