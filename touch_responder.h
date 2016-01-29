//
//  touch_responder.h
//  mono framework
//
//  Created by Kristoffer Lyder Andersen on 05/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#ifndef touch_responder_h
#define touch_responder_h

#include "touch_event.h"
#include "queue.h"

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
    class TouchResponder : IQueueItem
    {
        friend class ITouchSystem;
    protected:
        static GenericQueue<TouchResponder> ResponderChain;
        static TouchResponder* FirstResponder();

    public:

        static void RunResponderChainTouchBegin(TouchEvent &event);
        static void RunResponderChainTouchEnd(TouchEvent &event);
        static void RunResponderChainTouchMove(TouchEvent &event);

        /**
         * @brief Create a new responder object that receives touch input.
         *
         * Upon creation, this object is automatically inserted into the
         * responder chain, to receive touch input events.
         *
         *
         */
        TouchResponder();

        virtual void RespondTouchBegin(TouchEvent &event);
        virtual void RespondTouchMove(TouchEvent &event);
        virtual void RespondTouchEnd(TouchEvent &event);

        /**
         * Add this responder to the responder chain
         *
         */
        void Activate();

        /**
         * Remove this responder from the responder chain
         */
        void Deactivate();
    };

}

#endif /* touch_responder_h */
