// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef status_indicator_view_h
#define status_indicator_view_h

#include "view.h"

namespace mono { namespace ui {
    
    /**
     * @brief Indicate a boolean status, true/false, on/off or red/green
     *
     * The status indicator displays a circular LED like widget, that is red or
     * green by default.
     *
     */
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
