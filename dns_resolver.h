//
//  dns_resolver.h
//  wifiTest
//
//  Created by Kristoffer Lyder Andersen on 17/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#ifndef dns_resolver_h
#define dns_resolver_h

#include "network_request.h"
#include <wireless/redpine_module.h>

namespace mono { namespace network {
    
    /**
     * The DnsResolver class converts a domain name to an IP Address (A record)
     * @brief Resolve a domain name to a IPv4 address
     */
    class DnsResolver : public INetworkRequest
    {
    public:
        
        enum IpVersions
        {
            IP_v4 = 4,
            IP_v6 = 6
        };
        
    protected:
        
        String domain;
        uint8_t ipAddress[16];
        IpVersions ipver;
        
        redpine::DnsResolutionFrame *dnsFrame;
        
        void dnsCompletion(redpine::ManagementFrame::FrameCompletionData *data);
        
    public:
        
        /** Create an empty dns resolve request */
        DnsResolver();
        
        DnsResolver(const DnsResolver &other);
        
        DnsResolver &operator=(const DnsResolver &other);
        
        virtual ~DnsResolver();
        
        /** 
         * Create a DNS resolve request, and fetch the IP address right away.
         *
         * **Note** The request runs asynchonously, make sure to install the
         * completion callback right after construction.
         *
         * @see setCompletionCallback
         */
        DnsResolver(String aDomain);
        
        /**
         * Get the resolved IP address.
         * **Note** This is first valid after the request is completed.
         *
         * @brief Get the resolved IP address
         * @returns A text representation of the resolved IP address, or an empty string.
         * @see IsCompleted
         */
        String IpAddress() const;
        
        IpVersions IpVersion() const;
        
        /**
         * Gets the Dns request domain name, that is-to-be or has been resolved.
         *
         * @returns The domain name
         */
        String DomainName() const;
    };
    
} }

#endif /* dns_resolver_h */
