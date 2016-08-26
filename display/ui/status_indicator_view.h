// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef status_indicator_view_h
#define status_indicator_view_h

#include "view.h"

namespace mono { namespace ui {
    
    /**
     * @brief Indicate a boolean status, true/false, on/off or red/green
     *
     * The status indicator displays a circular LED like widget, that is red and
     * green by default. (Green is true, red is false)
     *
     * You use the method @ref setState to change the current state of the indicator.
     * This you change the state, the view automatically repaints itself on the screen.
     *
     * ## Example
     * @code
     * // Initialize the view (you should make it a class member)
     * mono::ui::StatusIndicatorView indicate(mono::geo::Rect(10,10,20,20), false);
     * 
     * // Change the default off color from red to to white
     * indicator.setOnStateColor(mono::display::WhiteColor);
     * 
     * // set the state to be On
     * indicator.setState(true);
     * 
     * // make the view visible
     * indicator.show();
     * @endcode
     */
    class StatusIndicatorView : public View
    {
    protected:
        
        display::Color onColor, offColor, borderColor;
        
        bool state;
        
        void initializer();
        
        void repaint();
        
    public:
        
        /**
        * @brief contruct a StatusIndicator with default parameters
        *
        * this view will be initialized in an empty *viewRect*, that is
        * (0,0,0,0)
        */
        StatusIndicatorView();
        
        /**
        * @brief Construct a Indicator and provide a View rect
        *
        * This create a view rect the exists in the provided @ref Rect and has
        * the default state `false`
        *
        * *Note*: You need to call @ref show before the view can be rendered
        *
        * @param vRct The bounding view rectangle, where the indicator is displayed
        */
        StatusIndicatorView(geo::Rect vRct);
        
        /**
        * @brief Construct a Indicator and provide a View rect and a initial state
        *
        * This create a view rect the exists in the provided @ref Rect
        *
        * *Note*: You need to call @ref show before the view can be rendered
        *
        * @param vRct The bounding view rectangle, where the indicator is displayed
        * @param status The initial state of the indicator
        */
        StatusIndicatorView(geo::Rect vRct, bool status);
        
        /**
        * @brief Sets the *on* state color
        *
        * If you change the color, you must call @ref scheduleRepaint to make the
        * change have effect.
        *
        * @param col The color of the indicator when it is *on*
        */
        void setOnStateColor(display::Color col);
        
        /**
        * @brief Sets the *off* state color
        *
        * If you change the color, you must call @ref scheduleRepaint to make the
        * change have effect.
        *
        * @param col The color of the indicator when it is *off*
        */
        void setOffStateColor(display::Color col);
        
        /**
        * @brief Sets a new on/off state 
        *
        * *Note* that the view repaints itself if the new state differs from
        * the old.
        *
        * @param newState The value of the new state (`true` is *on*)
        */
        void setState(bool newState);
        
        /**
        * @brief Gets the state of the indicator
        *
        * @return `true`if the state if *on*, `false` otherwise.
        */
        bool State() const;
    };
    
} }

#endif /* status_indicator_view_h */
