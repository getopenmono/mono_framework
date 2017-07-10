// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef wifi_interface_h
#define wifi_interface_h

#include <mono.h>

namespace mono { namespace io {

    class IWifi
    {
    public:

        enum NetworkEvents
        {
            CONNECTED,
            CONNECT_ERROR,
            DISCONNECTED
        };
        
    protected:

        mbed::FunctionPointer networkReadyHandler;
        mbed::FunctionPointer networkErrorHandler;
        mbed::FunctionPointer disconnectHandler;
        mbed::FunctionPointerArg1<void, NetworkEvents> networkEventHandler;

    public:

        virtual bool connect() = 0;
        virtual bool isConnected() const = 0;

        // MARK: Network callbacks

        void setConnectedCallback(void(*cfunc)(void))
        {
            networkReadyHandler.attach(cfunc);
        }

        template <typename Context>
        void setConnectedCallback(Context *cnxt, void(Context::*memptr)(void))
        {
            networkReadyHandler.attach<Context>(cnxt, memptr);
        }

        void setConnectErrorCallback(void(*cfunc)(void))
        {
            networkErrorHandler.attach(cfunc);
        }

        template <typename Context>
        void setConnectErrorCallback(Context *cnxt, void(Context::*memptr)(void))
        {
            networkErrorHandler.attach<Context>(cnxt, memptr);
        }

        template <typename Context>
        void setDisconnectedCallback(Context *cnxt, void(Context::*memptr)(void))
        {
            disconnectHandler.attach(cnxt, memptr);
        }

        void setDisconnectedCallback(void(*cfunc)(void))
        {
            disconnectHandler.attach(cfunc);
        }

        void setEventCallback(void(*cfunc)(NetworkEvents))
        {
            networkEventHandler.attach(cfunc);
        }

        template <typename Context>
        void setEventCallback(Context *cnxt, void(Context::*memptr)(NetworkEvents))
        {
            networkEventHandler.attach<Context>(cnxt, memptr);
        }

    };

} }

#endif /* wifi_interface_h */
