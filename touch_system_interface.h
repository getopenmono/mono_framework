//
//  touch_system_interface.h
//  analog
//
//  Created by Kristoffer Lyder Andersen on 05/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#ifndef touch_system_interface_h
#define touch_system_interface_h

#include "application_run_loop_task_interface.h"
#include "point.h"
#include "touch_responder.h"
#include "touch_event.h"

#include <us_ticker_api.h>

#include <stdio.h>

namespace mono {
    
    /**
     * Interface for the Touch sub-system
     *
     *
     */
    class ITouchSystem
    {
    protected:
        
        TouchEvent lastTouchBegin;
        geo::Point lastTouchPosition;
        bool touchInProgress;
        
    public:
        
        /**
         * <# description #>
         *
         * @brief Initialize the touch system controller
         */
        virtual void init() = 0;
        
        /**
         * <# description #>
         *
         * @brief <# brief desc #>
         */
        virtual void processTouchInput() = 0;
        
        
        virtual int ToScreenCoordsX(int touchPos, uint16_t screenWidth) = 0;
        
        
        virtual int ToScreenCoordsY(int touchPos, uint16_t screenHeight) = 0;
        
        /**
         * <# description #>
         *
         * @brief <# brief desc #>
         * @param <# param desc #>
         */
        void runTouchBegin(geo::Point &pos)
        {
            lastTouchBegin = TouchEvent(TouchEvent::TOUCH_BEGIN, pos, this);
            TouchResponder::RunResponderChainTouchBegin(lastTouchBegin);
        }
        
        /**
         * <# description #>
         *
         * @brief <# brief desc #>
         * @param <# param desc #>
         */
        void runTouchMove(geo::Point &pos)
        {
            TouchEvent event(TouchEvent::TOUCH_MOVE, pos, this);
            event.TouchBeginEvent = &lastTouchBegin;
            TouchResponder::RunResponderChainTouchMove(event);
        }
        
        /**
         * <# description #>
         *
         * @brief <# brief desc #>
         * @param <# param desc #>
         */
        void runTouchEnd(geo::Point &pos)
        {
            TouchEvent event(TouchEvent::TOUCH_END, pos, this);
            event.TouchBeginEvent = &lastTouchBegin;
            TouchResponder::RunResponderChainTouchEnd(event);
        }
    };

}

#endif /* touch_system_interface_h */
