// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef progress_bar_view_h
#define progress_bar_view_h

#include "view.h"
#include <FunctionPointer.h>

namespace mono { namespace ui {
    
    class ProgressBarView : public View
    {
    protected:
        int maxValue, minValue, currentValue, barPixelLength;
        
        mbed::FunctionPointer valueChangedHandler;
        
        display::Color borderColor, barColor;
        
        /** convenience initializer */
        void init();
        
        virtual void repaint();
        
    public:
        
        /**
         * @brief Create a ProgressBar with a zero view rect (0,0,0,0)
         */
        ProgressBarView();
        
        /**
         * Create a new ProgressBarView, with a defined view rect and default
         * progress value span: 0 to 100, with a current value of 0.
         *
         * @brief Create a ProgressBarView with values from 0 to 100
         * @param rect The view rect where the ProgressBar is painted
         */
        ProgressBarView(geo::Rect rect);
        
        /**
         * Set a new current value for the progress bar. The value is truncated 
         * to the existing value span (min and max values).
         *
         * Changes to the value will trigger the *value changed callback* and
         * cause the view to schedule itself for repaint.
         *
         * @brief Set a new progress value
         * @param newValue The new value
         */
        void setValue(int newValue);
        
        void setMaximum(int max);
        
        void setMinimum(int min);
        
        /**
         * Get notification callback everytime this progressbar changes its value
         *
         * @brief Set a progress value change callback function
         * @param cnxt A pointer to the callback member context (the `this` pointer)
         * @param memPtr A pointer to the callback member function
         */
        template <typename Owner>
        void setValueChangedCallback(Owner *cnxt, void(Owner::*memPtr)(void))
        {
            this->valueChangedHandler.attach(cnxt, memPtr);
        }
        
        /// GETTERS
        
        int Minimum() const;
        
        int Maximum() const;
        
        int CurrentValue() const;
        
    };
    
    
} }

#endif /* progress_bar_view_h */
