//
//  dns_resolver.cpp
//  wifiTest
//
//  Created by Kristoffer Lyder Andersen on 17/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//
#include <mbed.h>

#include "dns_resolver.h"

using namespace mono::network;

DnsResolver::DnsResolver() :
    INetworkRequest(), dnsFrame(NULL)
{
}

DnsResolver::DnsResolver(String aDomain) :
    INetworkRequest()
{
    domain = aDomain;
    
    if (!redpine::Module::IsNetworkReady())
    {
        lastErrorCode = NETWORK_NOT_READY_ERROR;
        this->triggerQueuedErrorHandler();
        return;
    }
    
    dnsFrame = new redpine::DnsResolutionFrame(domain);
    dnsFrame->setCompletionCallback<DnsResolver>(this, &DnsResolver::dnsCompletion);
    dnsFrame->autoReleaseWhenParsed = true;
    dnsFrame->commitAsync();
    
    debug("init dns: %s\n\r",domain());
    
    setState(IN_PROGRESS_STATE);
}

mono::String DnsResolver::IpAddress() const
{
    if (state != COMPLETED_STATE)
        return String();
    
    if (ipver == IP_v4)
        return String::Format("%i.%i.%i.%i",ipAddress[0],ipAddress[1],ipAddress[2],ipAddress[3]);
    else if (ipver == IP_v6)
    {
        return String::Format("%x%x:%x%x:%x%x:%x%x:%x%x:%x%x:%x%x:%x%x:",
                              ipAddress[0],
                              ipAddress[1],
                              ipAddress[2],
                              ipAddress[3],
                              ipAddress[4],
                              ipAddress[5],
                              ipAddress[6],
                              ipAddress[7],
                              ipAddress[8],
                              ipAddress[9],
                              ipAddress[10],
                              ipAddress[11],
                              ipAddress[12],
                              ipAddress[13],
                              ipAddress[14],
                              ipAddress[15]);
    }
    else
        return String();
    
}

DnsResolver::IpVersions DnsResolver::IpVersion() const
{
    return ipver;
}

mono::String DnsResolver::DomainName() const
{
    return domain;
}

void DnsResolver::dnsCompletion(redpine::ManagementFrame::FrameCompletionData *data)
{
    if (!data->Success)
    {
        debug("dns res err\n\r");
        lastErrorCode = DNS_RESOLUTION_FAILED_ERROR;
        triggerDirectErrorHandler();
    }
    else if (data->Context->commandId == redpine::ModuleFrame::DnsResolution)
    {
        redpine::DnsResolutionFrame *dns = (redpine::DnsResolutionFrame*) data->Context;
        
        //success
        ipver = (IpVersions) dns->ipVersion;
        memcpy(this->ipAddress, dns->resIpAddress, ipver == IP_v4 ? 4 : 16);
        triggerCompletionHandler();
        
    }
    else
    {
        debug("dns other comm err\n\r");
        lastErrorCode = COMMUNICATION_ERROR;
        triggerDirectErrorHandler();
    }
    
    if (dnsFrame->autoReleaseWhenParsed)
    {
        dnsFrame = NULL;
    }
}

DnsResolver::DnsResolver(const DnsResolver &other) : INetworkRequest(other), dnsFrame(other.dnsFrame)
{
    domain = other.domain;
    memcpy(ipAddress, other.ipAddress, 16);
    ipver = other.ipver;
    dnsFrame = other.dnsFrame; // copy pointer
    
    //overwrite any dnsFrame callback to myself
    if (dnsFrame->completionHandler)
    {
        dnsFrame->setCompletionCallback<DnsResolver>(this, &DnsResolver::dnsCompletion);
    }
    
    
}

DnsResolver &DnsResolver::operator=(const DnsResolver &other)
{
    
    INetworkRequest::operator=(other);
    
    domain = other.domain;
    memcpy(ipAddress, other.ipAddress, 16);
    ipver = other.ipver;
    
    dnsFrame = other.dnsFrame;
    
    
    //overwrite any dnsFrame callback to myself
    if (dnsFrame->completionHandler)
    {
        dnsFrame->setCompletionCallback<DnsResolver>(this, &DnsResolver::dnsCompletion);
    }
    
    return *this;
}

DnsResolver::~DnsResolver()
{
    if (dnsFrame->handlerContextObject == this)
    {
        dnsFrame->abort();
    }
}



