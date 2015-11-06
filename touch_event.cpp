//
//  touch_event.cpp
//  mono framework
//
//  Created by Kristoffer Lyder Andersen on 05/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#include "touch_event.h"

#include <us_ticker_api.h>
#include "touch_system_interface.h"

using namespace mono;

TouchEvent::TouchEvent()
{
    this->EventTimestamp = us_ticker_read();
    TouchBeginEvent = NULL;
    handled = false;
    IsScreenCoords = false;
}

TouchEvent::TouchEvent(TouchEventType type, geo::Point &pos, ITouchSystem *ctrlRef)
{
    this->EventTimestamp = us_ticker_read();
    TouchBeginEvent = NULL;
    handled = false;
    IsScreenCoords = false;
    
    Position = pos;
    EventType = type;
    TouchController = ctrlRef;
}