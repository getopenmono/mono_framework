// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef http_client_h
#define http_client_h


#include "network_request.h"
#include "dns_resolver.h"
#include "wireless/redpine_command_frames.h"

namespace mono { namespace network {

    /**
     * @brief A HTTP GET Request Client
     *
     * This class implements a HTTP GET Request, and provide you with a 
     * notification, when the response arrives.
     *
     * You provide an URL and one or more callback functions, for getting the
     * response data.
     *
     * ## Example
     *
     * @code
     * HttpClient client("http://www.myip.com");
     * client.setDataReadyCallback<MyClass>(this, &MyClass::httpData);
     * @endcode
     *
     * Now your method `httpData` is called when response data arrives. Note that
     * the callback can be called multiple times. A flag in the returned data 
     * object indicates, when this was the last data to be received.
     *
     * DNS resolution is done automatically. You can also change the HTTP port 
     * to something other than 80 by providing an alternative port via the URL:
     * `http://myhost:8080/hey`. Likewise, you can use raw IPv4 addresses:
     * `http://192.168.1.56/something`.
     *
     * ## Headers
     *
     * You can provide additional headers for the request by using the 
     * constructors optional second argument. Just remember to end each header 
     * line with a \r\n. Like this: `Accept: text/html\r\n`
     *
     * ## Error handling
     *
     * This client class handles error that arise from:
     *
     * * DNS lookup failed (no such domain name)
     * * No server response (host does not respond)
     *
     * This class does not handle errors based on HTTP status codes or content
     * types.
     *
     * If you wish to handle errors you should setup a callback for the @ref
     * setErrorCallback
     */
    class HttpClient : public INetworkRequest
    {
    public:

        /**
         * @brief Class to represent HTTP response data
         * 
         * This class defines the response data chunk return by an HTTP request.
         * It defines only public accessable properties, that indicate the 
         * response state and the raw data.
         *
         */
        class HttpResponseData
        {
        public:

            /** @brief Pointer to the originating @ref HttpClient object */
            HttpClient *Context;

            //String HttpHeaderRaw;

            /** @brief The raw HTTP response chunk. More chunk might arrive later */
            String bodyChunk;

            /** @brief `true` if this response chunk is the final. */
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

        /** @brief Construct an empty invalid client object */
        HttpClient();

        /**
         * @brief Construct a new HTPT request to a given URL
         * 
         * To receive the response, also setup the callback 
         * @ref setDataReadyCallback
         *
         * @param anUrl The URL string
         * @param headers Any additional headers to add
         */
        HttpClient(String anUrl, String headers = String());

        HttpClient(const HttpClient &other);
        HttpClient &operator=(const HttpClient &other);
        ~HttpClient();

        /**
         * @brief Provide your callback function to handle response data
         *
         * @param cnxt A pointer to the callback member function context (the `this` pointer)
         * @param memPtr A pointer to the member function, that is the callback
         */
        template <typename Owner>
        void setDataReadyCallback(Owner *cnxt, void (Owner::*memPtr)(const HttpResponseData&))
        {
            dataHandler.attach(cnxt, memPtr);
        }
    };


} }

#endif /* http_client_h */
