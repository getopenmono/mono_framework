// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef touch_system_interface_h
#define touch_system_interface_h

#include "application_run_loop_task_interface.h"
#include "point.h"
#include "touch_responder.h"
#include "touch_event.h"
#include <deprecated.h>
#include "rect.h"


#include <stdio.h>

namespace mono {

    /**
     * A calibration given for a touch system. It defines origin offsets,
     * and the scales of the mapping between screen coords and touch input
     * values.
     *
     * A calibration should enable a touch controller to accuratly translate
     * its touch into screen coords.
     */
    typedef geo::Rect TouchCalibration;

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
        bool active;
        
        /**
         * @brief Run the system system handlers of the begin touch event
         *
         * NOTE: You should not call this method manually. It is used by the sub-
         * class of this interface.
         *
         * @param pos The begin event position on the screen
         */
        void runTouchBegin(geo::Point &pos)
        {
            lastTouchBegin = TouchEvent(TouchEvent::TOUCH_BEGIN, pos, this);
            TouchResponder::runResponderChainTouchBegin(lastTouchBegin);
        }
        
        /**
         * @brief Run the system system handlers of the move touch event
         *
         * NOTE: You should not call this method manually. It is used by the sub-
         * class of this interface.
         *
         * @param pos The move event position on the screen
         */
        void runTouchMove(geo::Point &pos)
        {
            TouchEvent event(TouchEvent::TOUCH_MOVE, pos, this);
            event.TouchBeginEvent = &lastTouchBegin;
            TouchResponder::runResponderChainTouchMove(event);
        }
        
        /**
         * @brief Run the system system handlers of the end touch event
         *
         * NOTE: You should not call this method manually. It is used by the sub-
         * class of this interface.
         *
         * @param pos The end event position on the screen
         */
        void runTouchEnd(geo::Point &pos)
        {
            TouchEvent event(TouchEvent::TOUCH_END, pos, this);
            event.TouchBeginEvent = &lastTouchBegin;
            TouchResponder::runResponderChainTouchEnd(event);
        }
        
    public:
        
        /**
         * @brief Initialize the touch system controller
         *
         * NOTE: This method is called by the current @ref IApplicationContext
         * automatically upon reset. You should call this manually.
         */
        virtual void init() = 0;
        
        /**
         * @brief Sample the touch inputs and dispatch events if needed
         *
         * This @ref AppRunLoop calls this method automatically on each cycle.
         * If you wish to limit the sample frequency, your subclass must
         * manually abort some calls to this method.
         *
         * Note: Inactive touch systems must ignores calls to this method.
         */
        virtual void processTouchInput() = 0;
        
        /**
         * @brief Sub-classes must convert corrdinates to screen pixels (X axis)
         */
        virtual int toScreenCoordsX(int touchPos, uint16_t screenWidth) = 0;
        virtual int ToScreenCoordsX(int touchPos, uint16_t screenWidth) __DEPRECATED("Capitalized method calls syntax is being obsoleted","toScreenCoordsX") { return toScreenCoordsX(touchPos, screenWidth); }
        
        /**
         * @brief Sub-classes must convert corrdinates to screen pixels (Y axis)
         */
        virtual int toScreenCoordsY(int touchPos, uint16_t screenHeight) = 0;
        virtual int ToScreenCoordsY(int touchPos, uint16_t screenHeight) __DEPRECATED("Capitalized method calls syntax is being obsoleted","toScreenCoordsY") { return toScreenCoordsY(touchPos, screenHeight); }

        /**
         * @brief Get the touch systems current calibration config.
         * 
         */
        virtual TouchCalibration CurrentCalibration() = 0;

        /**
         * @brief Set a new calibration config on the touch system
         * 
         */
        virtual void setCalibration(TouchCalibration &cal) = 0;

        /**
         * @brief Activate (enables) the touch system
         */
        virtual void activate() { active = true; }

        /**
         * @brief Deactivates (disables) the touch system
         */
        virtual void deactivate() { active = false; }

        /**
         * @brief Retuern `true` if touch system is active
         */
        virtual bool isActive() const { return active; }
    };

}

#endif /* touch_system_interface_h */
