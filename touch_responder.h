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
    
    class TouchResponder : IQueueItem
    {
        friend ITouchSystem;
    protected:
        static GenericQueue<TouchResponder> ResponderChain;
        static TouchResponder* FirstResponder();
        
    public:
        
        static void RunResponderChainTouchBegin(TouchEvent &event);
        static void RunResponderChainTouchEnd(TouchEvent &event);
        static void RunResponderChainTouchMove(TouchEvent &event);
        
        /**
         * Create a new responder object that receives touch input
         *
         * @brief <# brief desc #>
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
