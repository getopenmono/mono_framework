
#ifndef socket_interface_h
#define socket_interface_h

#include <stdint.h>
#include <FunctionPointer.h>

namespace mono { namespace net {

    class ISocket {
    public:

        class DataBuffer{
        public:
            const char *data;
            uint32_t length;
        };

    protected:

        mbed::FunctionPointerArg1<void, const void *> didWriteHandler;
        mbed::FunctionPointerArg1<void, const DataBuffer> dataHandler;

    public:

        /**
         * @brief Write some data to the socket, asynchrounous!
         * 
         * The data will eventually be written to the socket, and you
         * will receive a callback on the @ref didWriteHandler
         */
        virtual void write(const char *data, const void *context = 0) = 0;

        /**
         * @brief Set the callback function for the *data-was-written* event
         * 
         */
        template <typename Context>
        void setDidWriteCallback(Context *cntxt, void(Context::*memPtr)(const void*))
        {
            didWriteHandler.attach<Context>(cntxt, memPtr);
        }

        /**
         * @brief Set data receiver callback function
         * 
         */
        template <typename Context>
        void setDataCallback(Context *cntxt, void(Context::*memptr)(const DataBuffer))
        {
            dataHandler.attach<Context>(cntxt, memptr);
        }

    };

} }

#endif /* socket_interface_h */
