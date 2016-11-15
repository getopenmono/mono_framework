
#ifndef http_post_client_hpp
#define http_post_client_hpp

#include "http_client.h"

namespace mono { namespace network {

    class HttpPostClient : public HttpClient
    {
    protected:

        redpine::HttpPostFrame *postFrame;

        mbed::FunctionPointerArg1<uint16_t, void> frameDataLengthHandler;
        mbed::FunctionPointerArg1<void, char*> frameDataHandler;

        void createFrameRequest(String ip);
        bool hasFrameRequest();
        bool shouldPost;

        /** dns resolver completion event handler */
        void dnsComplete(INetworkRequest::CompletionEvent *evnt);

    public:

        HttpPostClient();

        HttpPostClient(String anUrl, String headers = String());

        HttpPostClient(const HttpPostClient &other);

        HttpPostClient &operator=(const HttpPostClient &other);

        template <typename Class>
        void setBodyLengthCallback(Class *context, uint16_t(Class::*method)(void))
        {
            frameDataLengthHandler.attach<Class>(context, method);
        }

        template <typename Class>
        void setBodyDataCallback(Class *context, void(Class::*method)(char*))
        {
            frameDataHandler.attach<Class>(context, method);
        }

        
        void post();
        
    };

} }

#endif
