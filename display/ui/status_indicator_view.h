//
//  status_indicator_view.h
//
//
//  Created by Kristoffer Andersen on 07/12/2015.
//  Copyright © 2015 your name. All rights reserved.
//

#ifndef status_indicator_view_h
#define status_indicator_view_h

#include "view.h"

namespace mono { namespace ui {
    
    class StatusIndicatorView : public View
    {
    protected:
        
        display::Color onColor, offColor, borderColor;
        
        bool state;
        
        void initializer();
        
        void repaint();
        
    public:
        
        StatusIndicatorView();
        
        StatusIndicatorView(geo::Rect vRct);
        
        StatusIndicatorView(geo::Rect vRct, bool status);
        
        void setOnStateColor(display::Color col);
        
        void setOffStateColor(display::Color col);
        
        void setState(bool newState);
        
        bool State() const;
    };
    
} }

#endif /* status_indicator_view_h */
