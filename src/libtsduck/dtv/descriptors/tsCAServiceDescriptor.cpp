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

#include "tsCAServiceDescriptor.h"
#include "tsDescriptor.h"
#include "tsNames.h"
#include "tsTablesDisplay.h"
#include "tsPSIRepository.h"
#include "tsPSIBuffer.h"
#include "tsDuckContext.h"
#include "tsxmlElement.h"

#define MY_XML_NAME u"CA_service_descriptor"
#define MY_CLASS ts::CAServiceDescriptor
#define MY_DID ts::DID_ISDB_CA_SERVICE
#define MY_PDS ts::PDS_ISDB
#define MY_STD ts::Standards::ISDB

TS_REGISTER_DESCRIPTOR(MY_CLASS, ts::EDID::Private(MY_DID, MY_PDS), MY_XML_NAME, MY_CLASS::DisplayDescriptor);


//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

ts::CAServiceDescriptor::CAServiceDescriptor() :
    AbstractDescriptor(MY_DID, MY_XML_NAME, MY_STD, 0),
    CA_system_id(0),
    ca_broadcaster_group_id(0),
    message_control(0),
    service_ids()
{
}

void ts::CAServiceDescriptor::clearContent()
{
    CA_system_id = 0;
    ca_broadcaster_group_id = 0;
    message_control = 0;
    service_ids.clear();
}

ts::CAServiceDescriptor::CAServiceDescriptor(DuckContext& duck, const Descriptor& desc) :
    CAServiceDescriptor()
{
    deserialize(duck, desc);
}


//----------------------------------------------------------------------------
// Serialization
//----------------------------------------------------------------------------

void ts::CAServiceDescriptor::serializePayload(PSIBuffer& buf) const
{
    buf.putUInt16(CA_system_id);
    buf.putUInt8(ca_broadcaster_group_id);
    buf.putUInt8(message_control);
    for (auto it = service_ids.begin(); it != service_ids.end(); ++it) {
        buf.putUInt16(*it);
    }
}


//----------------------------------------------------------------------------
// Deserialization
//----------------------------------------------------------------------------

void ts::CAServiceDescriptor::deserializePayload(PSIBuffer& buf)
{
    CA_system_id = buf.getUInt16();
    ca_broadcaster_group_id = buf.getUInt8();
    message_control = buf.getUInt8();
    while (buf.canRead()) {
        service_ids.push_back(buf.getUInt16());
    }
}


//----------------------------------------------------------------------------
// Static method to display a descriptor.
//----------------------------------------------------------------------------

void ts::CAServiceDescriptor::DisplayDescriptor(TablesDisplay& disp, PSIBuffer& buf, const UString& margin, DID did, TID tid, PDS pds)
{
    if (buf.canReadBytes(4)) {
        disp << margin << "CA System Id: " << names::CASId(disp.duck(), buf.getUInt16(), NamesFlags::FIRST) << std::endl;
        disp << margin << UString::Format(u"CA broadcaster group id: 0x%X (%<d)", {buf.getUInt8()}) << std::endl;
        disp << margin << UString::Format(u"Delay time: %d days", {buf.getUInt8()}) << std::endl;
        while (buf.canReadBytes(2)) {
            disp << margin << UString::Format(u"Service id: 0x%X (%<d)", {buf.getUInt16()}) << std::endl;
        }
    }
}


//----------------------------------------------------------------------------
// XML serialization
//----------------------------------------------------------------------------

void ts::CAServiceDescriptor::buildXML(DuckContext& duck, xml::Element* root) const
{
    root->setIntAttribute(u"CA_system_id", CA_system_id, true);
    root->setIntAttribute(u"ca_broadcaster_group_id", ca_broadcaster_group_id, true);
    root->setIntAttribute(u"message_control", message_control);
    for (auto it = service_ids.begin(); it != service_ids.end(); ++it) {
        root->addElement(u"service")->setIntAttribute(u"id", *it, true);
    }
}


//----------------------------------------------------------------------------
// XML deserialization
//----------------------------------------------------------------------------

bool ts::CAServiceDescriptor::analyzeXML(DuckContext& duck, const xml::Element* element)
{
    xml::ElementVector xserv;
    bool ok =
        element->getIntAttribute(CA_system_id, u"CA_system_id", true) &&
        element->getIntAttribute(ca_broadcaster_group_id, u"ca_broadcaster_group_id", true) &&
        element->getIntAttribute(message_control, u"message_control", true) &&
        element->getChildren(xserv, u"service", 0, 125);

    for (auto it = xserv.begin(); ok && it != xserv.end(); ++it) {
        uint16_t id = 0;
        ok = (*it)->getIntAttribute(id, u"id", true);
        service_ids.push_back(id);
    }
    return ok;
}
