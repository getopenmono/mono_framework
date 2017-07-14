// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef touch_responder_h
#define touch_responder_h

#include "touch_event.h"
#include "queue.h"
#include "deprecated.h"

class ITouchSystem; //forwd decl

namespace mono {

    /**
     * The TouchResponder is an interface that classes and inherit from to
     * receive touch input events. This class also defined global static method
     * used by Mono's hardware dependend touch system. These static methods
     * receives the touch events and delegates them to all objects in the
     * responder chain.
     *
     * You can make any object a receiver of touch events if you inherit from
     * this interface. You need to override 3 methods:
     *
     * * @ref RespondTouchBegin
     * * @ref RespondTouchMove
     * * @ref RespondTouchEnd
     *
     * These methods are called on any subclass when touch input events are
     * received. *Note* that your subclass will receive all incoming events not
     * handled by other responders.
     *
     * If you want to make touch enabled graphical elements, you should use the
     * interface @ref ResponderView. This class is the parent class for all touch
     * enabled views.
     *
     * @see ResponderView
     * @see ITouchSystem
     * @brief The TouchReponder handles incoming touch events
     */
    class TouchResponder : public IQueueItem
    {
        friend class ITouchSystem;
    protected:
        static GenericQueue<TouchResponder> ResponderChain;
        bool breakReponderLoop;
    public:
        
        /**
         * @brief Get the top of the responder queue, the first to handle touch
         *
         * If there are no objects reponding to touch, this will return `NULL`
         */
        static TouchResponder* FirstResponder();
        
        static void runResponderChainTouchBegin(TouchEvent &event);
        static void runResponderChainTouchEnd(TouchEvent &event);
        static void runResponderChainTouchMove(TouchEvent &event);
        
        /**
         * @brief Create a new responder object that receives touch input.
         *
         * Upon creation, this object is automatically inserted into the
         * responder chain, to receive touch input events.
         *
         */
        TouchResponder();

        virtual void respondTouchBegin(TouchEvent &event);
        virtual void RespondTouchBegin(TouchEvent &event) __DEPRECATED("Please use the lower case variant","respondTouchBegin") { respondTouchBegin(event); }
        
        virtual void respondTouchMove(TouchEvent &event);
        virtual void RespondTouchMove(TouchEvent &event) __DEPRECATED("Please use the lower case variant","respondTouchMove") { respondTouchMove(event); }
        
        virtual void respondTouchEnd(TouchEvent &event);
        virtual void RespondTouchEnd(TouchEvent &event) __DEPRECATED("Please use the lower case variant","respondTouchMove") { respondTouchEnd(event); }

        /**
         * Add this responder to the responder chain
         *
         */
        void activate();

        void Activate() __DEPRECATED("Capitalized method calls syntax is being obsolted", "activate")
        { activate(); }

        /**
         * Remove this responder from the responder chain
         */
        void deactivate();

        void Deactivate() __DEPRECATED("Capitalized method calls syntax is being obsolted", "deactivate")
        { deactivate(); }
    };

}

#endif /* touch_responder_h */
