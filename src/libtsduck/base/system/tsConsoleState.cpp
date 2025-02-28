//----------------------------------------------------------------------------
//
// TSDuck - The MPEG Transport Stream Toolkit
// Copyright (c) 2005-2021, Thierry Lelegard
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.
//
//----------------------------------------------------------------------------

#include "tsConsoleState.h"
#include "tsSysUtils.h"


//----------------------------------------------------------------------------
// Constructor: save console state and configure it.
//----------------------------------------------------------------------------

ts::ConsoleState::ConsoleState(Report& report)
#if defined(TS_WINDOWS)
  : _input_cp(::GetConsoleCP()),
    _output_cp(::GetConsoleOutputCP())
#endif
{
#if defined(TS_WINDOWS)
    report.debug(u"previous code pages: input: %d, output: %d", {::GetConsoleCP(), ::GetConsoleOutputCP()});

    // Set Windows console input and output to UTF-8.
    if (::SetConsoleCP(CP_UTF8) == 0) {
        report.error(u"SetConsoleCP error: %s", {SysErrorCodeMessage()});
    }
    if (::SetConsoleOutputCP(CP_UTF8) == 0) {
        report.error(u"SetConsoleOutputCP error: %s", {SysErrorCodeMessage()});
    }

    report.debug(u"new code pages: input: %d, output: %d", {::GetConsoleCP(), ::GetConsoleOutputCP()});
#endif
}


//----------------------------------------------------------------------------
// Destructor: restore console state.
//----------------------------------------------------------------------------

ts::ConsoleState::~ConsoleState()
{
    // Restore initial console state.
#if defined(TS_WINDOWS)
    ::SetConsoleCP(_input_cp);
    ::SetConsoleOutputCP(_output_cp);
#endif
}
