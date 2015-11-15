//
//  touch_event.h
//  mono framework
//
//  Created by Kristoffer Lyder Andersen on 05/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#ifndef touch_event_h
#define touch_event_h

#include <stdint.h>
#include <point.h>

namespace mono {
    
    class ITouchSystem; // forward decl
    
    class TouchEvent
    {
    
    public:
        
        enum TouchEventType
        {
            TOUCH_BEGIN,
            TOUCH_END,
            TOUCH_MOVE
        };
        
        
        TouchEvent();
        
        TouchEvent(TouchEventType type, geo::Point &pos, ITouchSystem *ctrlRef);
        
        TouchEvent(const TouchEvent &event);
        
        TouchEvent& operator=(const TouchEvent &event);
        
        geo::Point Position;
        TouchEventType EventType;
        uint32_t EventTimestamp;
        bool handled;
        bool IsScreenCoords;
        ITouchSystem *TouchController;
        
        /**
         * If
         *
         */
        TouchEvent *TouchBeginEvent;
    };
    
}

#endif /* touch_event_h */
