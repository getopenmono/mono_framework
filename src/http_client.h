// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef http_client_h
#define http_client_h


#include "network_request.h"
#include "dns_resolver.h"
#include "wireless/redpine_command_frames.h"

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
            bool Finished;

            HttpResponseData(HttpClient *cnxt) :
                Context(cnxt),
                HttpHeaderRaw(String()),
                bodyChunk(String()),
                Finished(false)
            {}

            HttpResponseData &operator=(const HttpResponseData &other)
            {
                Context = other.Context;
                HttpHeaderRaw = other.HttpHeaderRaw;
                bodyChunk = other.bodyChunk;
                Finished = other.Finished;
                return *this;
            }
        };

    protected:
        mbed::FunctionPointerArg1<void, const HttpResponseData&> dataHandler;

        static const char *ipRegex;
        static const char *domainRegex;

        String domain;
        String path;
        uint32_t destPort;
        String headers;

        DnsResolver dns;
        HttpResponseData respData;

        redpine::HttpGetFrame *getFrame;

        /** Error callback for dns resolver */
        void dnsResolutionError(INetworkRequest::ErrorEvent *evnt);

        /** dns resolver completion event handler */
        virtual void dnsComplete(INetworkRequest::CompletionEvent *evnt);

        void httpData(redpine::HttpGetFrame::CallbackData *data);

        /** when the HTTP GET frame completes, check for error */
        void httpCompletion(redpine::ManagementFrame::FrameCompletionData *data);

        void triggerDataReady();

        virtual void createFrameRequest(String ipAddress);
        virtual bool hasFrameRequest();

    public:


        HttpClient();


        HttpClient(String anUrl, String headers = String());

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
