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

#include "tsComponentDescriptor.h"
#include "tsDescriptor.h"
#include "tsNames.h"
#include "tsTablesDisplay.h"
#include "tsPSIRepository.h"
#include "tsPSIBuffer.h"
#include "tsDuckContext.h"
#include "tsxmlElement.h"

#define MY_XML_NAME u"component_descriptor"
#define MY_CLASS ts::ComponentDescriptor
#define MY_DID ts::DID_COMPONENT
#define MY_STD ts::Standards::DVB

TS_REGISTER_DESCRIPTOR(MY_CLASS, ts::EDID::Standard(MY_DID), MY_XML_NAME, MY_CLASS::DisplayDescriptor);


//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

ts::ComponentDescriptor::ComponentDescriptor() :
    AbstractDescriptor(MY_DID, MY_XML_NAME, MY_STD, 0),
    stream_content_ext(0),
    stream_content(0),
    component_type(0),
    component_tag(0),
    language_code(),
    text()
{
}

ts::ComponentDescriptor::ComponentDescriptor(DuckContext& duck, const Descriptor& desc) :
    ComponentDescriptor()
{
    deserialize(duck, desc);
}

void ts::ComponentDescriptor::clearContent()
{
    stream_content_ext = 0;
    stream_content = 0;
    component_type = 0;
    component_tag = 0;
    language_code.clear();
    text.clear();
}


//----------------------------------------------------------------------------
// Serialization
//----------------------------------------------------------------------------

void ts::ComponentDescriptor::serializePayload(PSIBuffer& buf) const
{
    buf.putBits(stream_content_ext, 4);
    buf.putBits(stream_content, 4);
    buf.putUInt8(component_type);
    buf.putUInt8(component_tag);
    buf.putLanguageCode(language_code);
    buf.putString(text);
}


//----------------------------------------------------------------------------
// Deserialization
//----------------------------------------------------------------------------

void ts::ComponentDescriptor::deserializePayload(PSIBuffer& buf)
{
    buf.getBits(stream_content_ext, 4);
    buf.getBits(stream_content, 4);
    component_type = buf.getUInt8();
    component_tag = buf.getUInt8();
    buf.getLanguageCode(language_code);
    buf.getString(text);
}


//----------------------------------------------------------------------------
// Static method to display a descriptor.
//----------------------------------------------------------------------------

void ts::ComponentDescriptor::DisplayDescriptor(TablesDisplay& disp, PSIBuffer& buf, const UString& margin, DID did, TID tid, PDS pds)
{
    if (buf.canReadBytes(6)) {
        disp << margin << "Content/type: " << names::ComponentType(disp.duck(), buf.getUInt16(), NamesFlags::FIRST) << std::endl;
        disp << margin << UString::Format(u"Component tag: %d (0x%<X)", {buf.getUInt8()}) << std::endl;
        disp << margin << "Language: " << buf.getLanguageCode() << std::endl;
        if (buf.canRead()) {
            disp << margin << "Description: \"" << buf.getString() << "\"" << std::endl;
        }
    }
}


//----------------------------------------------------------------------------
// XML serialization
//----------------------------------------------------------------------------

void ts::ComponentDescriptor::buildXML(DuckContext& duck, xml::Element* root) const
{
    root->setIntAttribute(u"stream_content", stream_content, true);
    root->setIntAttribute(u"stream_content_ext", stream_content_ext, true);
    root->setIntAttribute(u"component_type", component_type, true);
    root->setIntAttribute(u"component_tag", component_tag, true);
    root->setAttribute(u"language_code", language_code);
    root->setAttribute(u"text", text);
}


//----------------------------------------------------------------------------
// XML deserialization
//----------------------------------------------------------------------------

bool ts::ComponentDescriptor::analyzeXML(DuckContext& duck, const xml::Element* element)
{
    return element->getIntAttribute(stream_content, u"stream_content", true, 0x00, 0x00, 0x0F) &&
           element->getIntAttribute(stream_content_ext, u"stream_content_ext", false, 0x0F, 0x00, 0x0F) &&
           element->getIntAttribute(component_type, u"component_type", true, 0x00, 0x00, 0xFF) &&
           element->getIntAttribute(component_tag, u"component_tag", false, 0x00, 0x00, 0xFF) &&
           element->getAttribute(language_code, u"language_code", true, u"", 3, 3) &&
           element->getAttribute(text, u"text", false, u"", 0, MAX_DESCRIPTOR_SIZE - 8);
}
