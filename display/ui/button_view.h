//
//  button_view.h
//  analog
//
//  Created by Kristoffer Andersen on 06/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#ifndef button_view_h
#define button_view_h

#include "responder_view.h"
#include "text_label_view.h"
#include <FunctionPointer.h>

namespace mono { namespace ui {
    
    class ButtonView : public ResponderView
    {
    protected:
        
        mbed::FunctionPointer clickHandler;
        
        virtual void TouchBegin(TouchEvent &event);
        virtual void TouchMove(TouchEvent &event);
        virtual void TouchEnd(TouchEvent &event);
        
        TextLabelView textLabel;
        
        bool isPressedDown;
        
        display::Color borderColor;
        display::Color borderColorPressed;
        
        void initButton();
        
    public:
        
        ButtonView();
        
        
        ButtonView(geo::Rect rect, String text);
        ButtonView(geo::Rect rect, const char *text);
        
        
        template <typename Owner>
        void setClickCallback(Owner *obj, void(Owner::*memPtr)(void))
        {
            clickHandler.attach<Owner>(obj, memPtr);
        }
        
        virtual void repaint();
        
    };
    
    
} }

#endif /* button_view_h */
