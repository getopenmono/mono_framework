// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef responder_view_h
#define responder_view_h

#include <touch_system_interface.h>
#include "view.h"

namespace mono { namespace ui {
    
    class ResponderView : public View, public mono::TouchResponder
    {
        
    protected:
        
        ResponderView();
        ResponderView(geo::Rect rect);
        
        bool touchActive;
        
        /** Internal touch handler, you should not overwrite this */
        virtual void respondTouchBegin(TouchEvent &event);
        
        /** Internal touch handler, you should not overwrite this */
        virtual void respondTouchMove(TouchEvent &event);
        
        /** Internal touch handler, you should not overwrite this */
        virtual void respondTouchEnd(TouchEvent &event);
        
        
        virtual void touchBegin(TouchEvent &event);
        virtual void TouchBegin(TouchEvent &event) __DEPRECATED("Capitalized method calls syntax is being obsoleted","touchBegin") { touchBegin(event); }
        
        virtual void touchEnd(TouchEvent &event);
        virtual void TouchEnd(TouchEvent &event) __DEPRECATED("Capitalized method calls syntax is being obsoleted","touchEnd") { touchEnd(event); }
        
        virtual void touchMove(TouchEvent &event);
        virtual void TouchMove(TouchEvent &event) __DEPRECATED("Capitalized method calls syntax is being obsoleted","touchMove") { touchMove(event); }
        
        virtual void toScreenCoords(TouchEvent *event);
        virtual void ToScreenCoords(TouchEvent *event) __DEPRECATED("Capitalized method calls syntax is being obsoleted","toScreenCoords") { toScreenCoords(event); }
        
    public:
        
        /**
         * 
         *
         * @brief Shows (repaints) the view and insert into the responder chain
         * @see View::show
         */
        virtual void show();
        
        /**
         * 
         *
         * @brief hides the view, and remove from the responder chain
         * @see View::hide
         */
        virtual void hide();
    };
    
} }

#endif /* responder_view_h */
