// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef wifi_interface_h
#define wifi_interface_h

#include <mono.h>

namespace mono { namespace io {
    
    /**
     * @brief Interface for Wifi setup methods
     *
     * Subclasses that implement this interface will control Wifi hardware
     * and encapsulate connection processes inside this convenient interface.
     *
     * You need only to call the @ref connect method and wait the *connected*
     * callback event.
     *
     * The implementation of this interface must take care of associating with
     * a Wifi Access Point and getting an dynamic IP address from AP's DHCP.
     *
     * ### Sleep mode
     *
     * When mono enters sleep, the wifi will loose power. Upon wake-up this
     * interface will notify you with a *disconnected* callback.
     *
     * ### Example
     *
     * You should have only one *IWifi* based object in your application.
     * Therefore add it as a member of your *AppController* class:
     *
     * @code
     * class AppController : public mono::IApplication
     * {
     *   public:
     *     mono::io::Wifi wifi;
     * }
     * @endcode
     *
     * Then, when you need network access the first time, just call:
     *
     * @code
     * AppController::monoWakeFromReset()
     * {
     *   bool connecting = wifi.connect();
     *
     *   if (!connecting)
     *     printf("Error, can not connect");
     * }
     * @endcode
     *
     */
    class IWifi
    {
    public:
        
        /**
         * @brief The Wifi connection status event types
         */
        enum NetworkEvents
        {
            CONNECTED,      /**< When Wifi is connected and ready */
            CONNECT_ERROR,  /**< Wifi failed to connect to access point */
            DISCONNECTED    /**< Wifi was disconnected */
        };
        
    protected:

        mbed::FunctionPointer networkReadyHandler;
        mbed::FunctionPointer networkErrorHandler;
        mbed::FunctionPointer disconnectHandler;
        mbed::FunctionPointerArg1<void, NetworkEvents> networkEventHandler;

    public:
        
        /**
         * @brief Connect to the Wifi network
         *
         * Use the parameters provided in the constructor to connect to the
         * configured access point. This method will also obtain an IP address
         * from the AP's DHCP server.
         *
         * You can all this method multiple times, even if you are already
         * connected. The method will simply ignore attempt to connect, when
         * already connected.
         *
         * @return `true` is configuration is valid and Wifi hardware is OK.
         */
        virtual bool connect() = 0;
        
        /**
         * @brief Return `true` Wifi is connected
         *
         * This will return `true` only if a connection is currently present.
         * This means it will return `false` even if the Wifi hardware is
         * currently in process of connecting to an access point or obtaining
         * an IP address.
         */
        virtual bool isConnected() const = 0;

        // MARK: Network callbacks
        
        /**
         * @brief Set a callback for the *connected* event
         *
         * Set a callback function to get notified when network access is
         * established.
         *
         * @param cfunc A pointer to the C function that handles the event
         */
        void setConnectedCallback(void(*cfunc)(void))
        {
            networkReadyHandler.attach(cfunc);
        }
        
        /**
         * @brief Set a callback for the *connected* event
         *
         * Set a callback function to get notified when network access is
         * established.
         *
         * @param cnxt A pointer to the object that handles the event
         * @param memptr A pointer to the method on the class that handles the event
         */
        template <typename Context>
        void setConnectedCallback(Context *cnxt, void(Context::*memptr)(void))
        {
            networkReadyHandler.attach<Context>(cnxt, memptr);
        }
        
        /**
         * @brief Set a callback for the *connect failed* event
         *
         * Set a callback function to get notified when network access could
         * not be established, and the Wifi hardware has given up.
         *
         * @param cfunc A pointer to the C function that handles the event
         */
        void setConnectErrorCallback(void(*cfunc)(void))
        {
            networkErrorHandler.attach(cfunc);
        }
        
        /**
         * @brief Set a callback for the *connect failed* event
         *
         * Set a callback function to get notified when network access could
         * not be established, and the Wifi hardware has given up.
         *
         * @param cnxt A pointer to the object that handles the event
         * @param memptr A pointer to the method on the class that handles the event
         */
        template <typename Context>
        void setConnectErrorCallback(Context *cnxt, void(Context::*memptr)(void))
        {
            networkErrorHandler.attach<Context>(cnxt, memptr);
        }
        
        /**
         * @brief Set a callback for the *disconnected* event
         *
         * Set a callback function to get notified when network access is
         * terminated.
         *
         * this event will always fire after mono comes out of sleep, if network
         * was established when it went to sleep.
         *
         * @param cnxt A pointer to the object that handles the event
         * @param memptr A pointer to the method on the class that handles the event
         */
        template <typename Context>
        void setDisconnectedCallback(Context *cnxt, void(Context::*memptr)(void))
        {
            disconnectHandler.attach(cnxt, memptr);
        }
        
        /**
         * @brief Set a callback for the *disconnected* event
         *
         * Set a callback function to get notified when network access is
         * terminated.
         *
         * this event will always fire after mono comes out of sleep, if network
         * was established when it went to sleep.
         *
         * @param cfunc A pointer to the C function that handles the event
         */
        void setDisconnectedCallback(void(*cfunc)(void))
        {
            disconnectHandler.attach(cfunc);
        }
        
        /**
         * @brief Set a callback for the all events
         *
         * Set a callback function to get notified when network access state is
         * changed.
         *
         * @param cfunc A pointer to the C function that handles the event
         */
        void setEventCallback(void(*cfunc)(NetworkEvents))
        {
            networkEventHandler.attach(cfunc);
        }
        
        /**
         * @brief Set a callback for the all events
         *
         * Set a callback function to get notified when network access state is
         * changed.
         *
         * @param cnxt A pointer to the object that handles the event
         * @param memptr A pointer to the method on the class that handles the event
         */
        template <typename Context>
        void setEventCallback(Context *cnxt, void(Context::*memptr)(NetworkEvents))
        {
            networkEventHandler.attach<Context>(cnxt, memptr);
        }

    };

} }

#endif /* wifi_interface_h */
