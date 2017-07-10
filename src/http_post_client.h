// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef http_post_client_hpp
#define http_post_client_hpp

#include "http_client.h"

namespace mono { namespace network {

    /**
     * @brief HTTP POST Request Client
     *
     * This class can send a HTTP POST request with body data. You provide
     * callbacks to provide the body length (in bytes) and content. This is to
     * avoid caching the content, and save memory.
     *
     * Before this class works you must setup a working wifi connection.
     *
     * ## Example
     *
     * @code
     * HttpPostClient client("http://www.postrequest.org/");
     * client.setBodyLengthCallback<MyClass>(this, &MyClass::getDataLength);
     * client.setBodyDataCallback<MyClass>(this, &MyClass::getData);
     * client.post();
     * @endcode
     *
     * This will post the request with no default `Content-Type` header. You
     * can provide your own `Content-Type` by using the second (optional) argument
     * in the constructor. If you do, remember to insert a `\r\n` at the end of
     * of the header string.
     *
     * ## Trailing white space
     *
     * Due to a bug in the Redpine module protocol, the post body risk getting
     * trailing whitespace characters (spaces: 0x20). There can up to 3 trailing
     * spaces: `   `. The reason is the redpine frame must end on a 4-byte byte 
     * boundry. Instead of appending NULL characters, we append spaces. Most HTTP
     * servers does not respond well to `NULL` character being part of a text 
     * body.
     *
     * This is of course an issue when sending binary data via POST requests. In
     * these cases you should (at this point) avoid binary formats. (Maybe use
     * base64 encoding.)
     */
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

        /**
         * @brief Construct an empty invalid HTTP client
         */
        HttpPostClient();

        /**
         * @brief Construct a client with a target URL
         *
         * Headers like `Host` and `Content-Length` are created automatically.
         * The DNS resolution of the hostname is also done automatically.
         *
         * The actual POST request is not execute before you call @ref post. 
         * However, DNS resolution is started immediately.
         *
         * @param anUrl THe URL to call
         * @param headers Optional extra headers to add to the request
         */
        HttpPostClient(String anUrl, String headers = String());

        HttpPostClient(const HttpPostClient &other);

        HttpPostClient &operator=(const HttpPostClient &other);

        /**
         * @brief Callback for providing the length of the HTTP POST body
         * 
         * You should provide a efficient method of getting the body length,
         * since this callback is used multiple times under the request
         * execution.
         *
         * @param context pointer to the object to call the member function on
         * @param method pointer to the member function to be called
         */
        template <typename Class>
        void setBodyLengthCallback(Class *context, uint16_t(Class::*method)(void))
        {
            frameDataLengthHandler.attach<Class>(context, method);
        }

        /**
         * @brief Callback for providing the length of the HTTP POST body
         *
         * You should provide a efficient method of getting the body length,
         * since this callback is used multiple times under the request
         * execution.
         *
         * @param cfunc Pointer to the function to call for request data length
         */
        void setBodyLengthCallback(uint16_t(*cfunc)(void))
        {
            frameDataLengthHandler.attach(cfunc);
        }

        /**
         * @brief Callback for providing the body content of the HTTP POST
         *
         * The internals of the request will ensure the provided `char*` is
         * large enough to hold the size of the HTTP body.
         *
         * @param context pointer to the object to call the member function on
         * @param method pointer to the member function to be called
         */
        template <typename Class>
        void setBodyDataCallback(Class *context, void(Class::*method)(char*))
        {
            frameDataHandler.attach<Class>(context, method);
        }

        /**
         * @brief Callback for providing the body content of the HTTP POST
         *
         * The internals of the request will ensure the provided `char*` is
         * large enough to hold the size of the HTTP body.
         *
         * @param cfunc Pointer to the function to call for request body data
         */
        void setBodyDataCallback(void(*cfunc)(char*))
        {
            frameDataHandler.attach(cfunc);
        }

        /**
         * @brief Execute the POST request
         * 
         * Commit the request sending it to the server.
         */
        void post();
        
    };

} }

#endif
