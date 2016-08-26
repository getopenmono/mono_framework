// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef progress_bar_view_h
#define progress_bar_view_h

#include "view.h"
#include <FunctionPointer.h>

namespace mono { namespace ui {

    /**
     * A UI widget displaying a common progress bar.
     *
     * This progressbar is simply a rectangle with a line (progress indicator)
     * inside. The indicator value is (by default) a value between 0 and 100,
     * but you can set your own minimum and maximum points.
     *
     * The progress value's minimum and maximum is automtically calculated into
     * the correct pixel value, scaled to the pixel width of the progressbar.
     * 
     * ## Example
     *
     * @code
     * // Create the progressbar object
     * mono::ui::ProgressBarView prgs(mono::geo::Rect(10,10,156,100));
     *
     * // set the progressbars indicator value to 50%
     * prgs.setValue(50);
     *
     * // display it
     * prgs.show();
     * @endcode
     *
     * ## A common mistake
     *
     * Be aware that the progressbar is painted asynchronously. This means you
     * cannot increment its value inside a for-loop or alike.
     * 
     * This code will not work:
     * @code
     * for (int i=0; i<100; i++) {
     *     prgs.setValue(i);
     * }
     * @endcode
     *
     * Even if the loop ran veeery slow, you will not see a moving progress 
     * indicator. The reason is that the view is only painted in the run-loop,
     * so no screen updates can happen from inside the for-loop!
     *
     * You should use a continous timer, with a callback that increments the
     * progress indicator:
     * 
     * @code
     * void updateProgress(){
     *     prgs.setValue(i++);
     * }
     *
     * mono::Timer tim(500);
     * tim.setCallback(&updateProgress);
     * @endcode
     *
     * This code inject a continous task to the run loop that can increment the
     * progressbar. This is the correct way to animate a progressbar.
     */
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

        /**
         * @brief Define a new minimum value for the progress indicator
         * 
         */
        void setMaximum(int max);

        /**
         * @brief define a new maximum value for the progress indicator
         */
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
