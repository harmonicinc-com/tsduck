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

#include "tsTargetIPv6SlashDescriptor.h"
#include "tsDescriptor.h"
#include "tsTablesDisplay.h"
#include "tsPSIRepository.h"
#include "tsPSIBuffer.h"
#include "tsDuckContext.h"
#include "tsxmlElement.h"

#define MY_XML_NAME u"target_IPv6_slash_descriptor"
#define MY_CLASS ts::TargetIPv6SlashDescriptor
#define MY_DID ts::DID_INT_IPV6_SLASH
#define MY_TID ts::TID_INT
#define MY_STD ts::Standards::DVB

TS_REGISTER_DESCRIPTOR(MY_CLASS, ts::EDID::TableSpecific(MY_DID, MY_TID), MY_XML_NAME, MY_CLASS::DisplayDescriptor);


//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

ts::TargetIPv6SlashDescriptor::TargetIPv6SlashDescriptor() :
    AbstractDescriptor(MY_DID, MY_XML_NAME, MY_STD, 0),
    addresses()
{
}

void ts::TargetIPv6SlashDescriptor::clearContent()
{
    addresses.clear();
}

ts::TargetIPv6SlashDescriptor::TargetIPv6SlashDescriptor(DuckContext& duck, const Descriptor& desc) :
    TargetIPv6SlashDescriptor()
{
    deserialize(duck, desc);
}

ts::TargetIPv6SlashDescriptor::Address::Address(const IPv6Address& addr, uint8_t mask) :
    IPv6_addr(addr),
    IPv6_slash_mask(mask)
{
}


//----------------------------------------------------------------------------
// Serialization
//----------------------------------------------------------------------------

void ts::TargetIPv6SlashDescriptor::serializePayload(PSIBuffer& buf) const
{
    for (auto it = addresses.begin(); it != addresses.end(); ++it) {
        buf.putBytes(it->IPv6_addr.toBytes());
        buf.putUInt8(it->IPv6_slash_mask);
    }
}


//----------------------------------------------------------------------------
// Deserialization
//----------------------------------------------------------------------------

void ts::TargetIPv6SlashDescriptor::deserializePayload(PSIBuffer& buf)
{
    while (buf.canRead()) {
        Address addr;
        addr.IPv6_addr = IPv6Address(buf.getBytes(16));
        addr.IPv6_slash_mask = buf.getUInt8();
        addresses.push_back(addr);
    }
}


//----------------------------------------------------------------------------
// Static method to display a descriptor.
//----------------------------------------------------------------------------

void ts::TargetIPv6SlashDescriptor::DisplayDescriptor(TablesDisplay& disp, PSIBuffer& buf, const UString& margin, DID did, TID tid, PDS pds)
{
    while (buf.canReadBytes(17)) {
        disp << margin << "Address/mask: " << IPv6Address(buf.getBytes(16));
        disp << "/" << int(buf.getUInt8()) << std::endl;
    }
}


//----------------------------------------------------------------------------
// XML serialization
//----------------------------------------------------------------------------

void ts::TargetIPv6SlashDescriptor::buildXML(DuckContext& duck, xml::Element* root) const
{
    for (auto it = addresses.begin(); it != addresses.end(); ++it) {
        xml::Element* e = root->addElement(u"address");
        e->setIPv6Attribute(u"IPv6_addr", it->IPv6_addr);
        e->setIntAttribute(u"IPv6_slash_mask", it->IPv6_slash_mask);
    }
}


//----------------------------------------------------------------------------
// XML deserialization
//----------------------------------------------------------------------------

bool ts::TargetIPv6SlashDescriptor::analyzeXML(DuckContext& duck, const xml::Element* element)
{
    xml::ElementVector children;
    bool ok = element->getChildren(children, u"address", 0, MAX_ENTRIES);

    for (size_t i = 0; ok && i < children.size(); ++i) {
        Address addr;
        ok = children[i]->getIPv6Attribute(addr.IPv6_addr, u"IPv6_addr", true) &&
             children[i]->getIntAttribute(addr.IPv6_slash_mask, u"IPv6_slash_mask", true);
        addresses.push_back(addr);
    }
    return ok;
}
