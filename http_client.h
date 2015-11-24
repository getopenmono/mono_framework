//
//  http_client.h
//  wifiTest
//
//  Created by Kristoffer Andersen on 17/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#ifndef http_client_h
#define http_client_h

#include <mono.h>
#include <mbed.h>

#include "network_request.h"
#include "dns_resolver.h"

namespace mono { namespace network {
    
    class HttpClient : public INetworkRequest
    {
    public:
        
        class HttpResponseData
        {
        public:
            HttpClient *Context;
            String HttpHeaderRaw;
            String bodyChunk;
            bool finished;
            
            HttpResponseData(HttpClient *cnxt) :
                Context(cnxt),
                HttpHeaderRaw(String()),
                bodyChunk(String()),
                finished(false)
            {}
            
            HttpResponseData &operator=(const HttpResponseData &other)
            {
                Context = other.Context;
                HttpHeaderRaw = other.HttpHeaderRaw;
                bodyChunk = other.bodyChunk;
                finished = other.finished;
                return *this;
            }
        };
        
    protected:
        mbed::FunctionPointerArg1<void, const HttpResponseData&> dataHandler;
        
        String domain;
        String path;
        DnsResolver dns;
        HttpResponseData respData;
        
        redpine::HttpGetFrame *getFrame;
        
        /** Error callback for dns resolver */
        void dnsResolutionError(INetworkRequest::ErrorEvent *evnt);
        
        /** dns resolver completion event handler */
        void dnsComplete(INetworkRequest::CompletionEvent *evnt);
        
        void httpData(redpine::HttpGetFrame::CallbackData *data);
        
        void triggerDataReady();
    public:
        
        
        HttpClient();
        
        
        HttpClient(String anUrl);
        
        HttpClient(const HttpClient &other);
        
        HttpClient &operator=(const HttpClient &other);
        
        ~HttpClient();
        
        template <typename Owner>
        void setDataReadyCallback(Owner *cnxt, void (Owner::*memPtr)(const HttpResponseData&))
        {
            dataHandler.attach(cnxt, memPtr);
        }
    };
    
    
} }

#endif /* http_client_h */
