
#ifndef http_post_client_hpp
#define http_post_client_hpp

#include "http_client.h"

namespace mono { namespace network {

    class HttpPostClient : public HttpClient
    {
    protected:

        redpine::HttpPostFrame *postFrame;

    public:

        HttpPostClient();

        HttpPostClient(String anUrl);

        HttpPostClient(const HttpPostClient &other);

        HttpPostClient &operator=(const HttpPostClient &other);

        template <typename Class>
        void setBodyLengthCallback(Class *context, uint16_t(Class::*method)(void))
        {
            postFrame->requestDataLengthCallback.attach<Class>(context, method);
        }

        template <typename Class>
        void setBodyDataCallback(Class *context, void(Class::*method)(char*))
        {
            postFrame->requestDataCallback.attach<Class>(context, method);
        }

        
        void post();
        
    };

} }

#endif
