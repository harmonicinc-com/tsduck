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

#include "tsAVCAccessUnitDelimiter.h"
#include "tsAVC.h"


//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------

ts::AVCAccessUnitDelimiter::AVCAccessUnitDelimiter(const uint8_t* data, size_t size) :
    SuperClass(),
    primary_pic_type(0)
{
    parse(data, size);
}


//----------------------------------------------------------------------------
// Clear all values
//----------------------------------------------------------------------------

void ts::AVCAccessUnitDelimiter::clear()
{
    SuperClass::clear();
    primary_pic_type = 0;
}


//----------------------------------------------------------------------------
// Parse the body of the binary access unit. Return the "valid" flag.
//----------------------------------------------------------------------------

bool ts::AVCAccessUnitDelimiter::parseBody(AVCParser& parser, std::initializer_list<uint32_t>)
{
    return nal_unit_type == AVC_AUT_DELIMITER && parser.u(primary_pic_type, 3);
}


//----------------------------------------------------------------------------
// Display structure content
//----------------------------------------------------------------------------

std::ostream& ts::AVCAccessUnitDelimiter::display(std::ostream& out, const UString& margin, int level) const
{
#define DISP(n) disp(out, margin, u ## #n, n)

    if (valid) {
        DISP(forbidden_zero_bit);
        DISP(nal_ref_idc);
        DISP(nal_unit_type);
        DISP(primary_pic_type);
        DISP(rbsp_trailing_bits_valid);
        DISP(rbsp_trailing_bits_count);
    }
    return out;

#undef DISP
}
