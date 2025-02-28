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

#include "tsExternalESIdDescriptor.h"
#include "tsDescriptor.h"
#include "tsNames.h"
#include "tsTablesDisplay.h"
#include "tsPSIRepository.h"
#include "tsPSIBuffer.h"
#include "tsDuckContext.h"
#include "tsxmlElement.h"

#define MY_XML_NAME u"external_ES_ID_descriptor"
#define MY_CLASS ts::ExternalESIdDescriptor
#define MY_DID ts::DID_EXT_ES_ID
#define MY_STD ts::Standards::MPEG

TS_REGISTER_DESCRIPTOR(MY_CLASS, ts::EDID::Standard(MY_DID), MY_XML_NAME, MY_CLASS::DisplayDescriptor);


//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

ts::ExternalESIdDescriptor::ExternalESIdDescriptor() :
    AbstractDescriptor(MY_DID, MY_XML_NAME, MY_STD, 0),
    external_ES_ID(0)
{
}

void ts::ExternalESIdDescriptor::clearContent()
{
    external_ES_ID = 0;
}

ts::ExternalESIdDescriptor::ExternalESIdDescriptor(DuckContext& duck, const Descriptor& desc) :
    ExternalESIdDescriptor()
{
    deserialize(duck, desc);
}


//----------------------------------------------------------------------------
// Serialization
//----------------------------------------------------------------------------

void ts::ExternalESIdDescriptor::serializePayload(PSIBuffer& buf) const
{
    buf.putUInt16(external_ES_ID);
}

void ts::ExternalESIdDescriptor::deserializePayload(PSIBuffer& buf)
{
    external_ES_ID = buf.getUInt16();
}


//----------------------------------------------------------------------------
// Static method to display a descriptor.
//----------------------------------------------------------------------------

void ts::ExternalESIdDescriptor::DisplayDescriptor(TablesDisplay& disp, PSIBuffer& buf, const UString& margin, DID did, TID tid, PDS pds)
{
    if (buf.canReadBytes(2)) {
        disp << margin << UString::Format(u"External ES id: 0x%X (%<d)", {buf.getUInt16()}) << std::endl;
    }
}


//----------------------------------------------------------------------------
// XML serialization
//----------------------------------------------------------------------------

void ts::ExternalESIdDescriptor::buildXML(DuckContext& duck, xml::Element* root) const
{
    root->setIntAttribute(u"external_ES_ID", external_ES_ID, true);
}

bool ts::ExternalESIdDescriptor::analyzeXML(DuckContext& duck, const xml::Element* element)
{
    return element->getIntAttribute(external_ES_ID, u"external_ES_ID", true);
}
