//
//  responder_view.h
//  mono framework
//
//  Created by Kristoffer Andersen on 06/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#ifndef responder_view_h
#define responder_view_h

#include <touch_system_interface.h>
#include "view.h"

namespace mono { namespace ui {
    
    class ResponderView : public View, protected mono::TouchResponder
    {
        
    protected:
        
        ResponderView();
        ResponderView(geo::Rect rect);
        
        bool touchActive;
        
        /** Internal touch handler, you should not overwrite this */
        virtual void RespondTouchBegin(TouchEvent &event);
        
        /** Internal touch handler, you should not overwrite this */
        virtual void RespondTouchMove(TouchEvent &event);
        
        /** Internal touch handler, you should not overwrite this */
        virtual void RespondTouchEnd(TouchEvent &event);
        
        
        virtual void TouchBegin(TouchEvent &event);
        virtual void TouchEnd(TouchEvent &event);
        virtual void TouchMove(TouchEvent &event);
        
        virtual void ToScreenCoords(TouchEvent *event);
    };
    
} }

#endif /* responder_view_h */
