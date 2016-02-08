// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef touch_calibrator_h
#define touch_calibrator_h

#include "touch_system_interface.h"
#include "responder_view.h"
#include "text_label_view.h"

#include <FunctionPointer.h>

namespace mono { namespace ui {
    

    /**
     * @brief Touch Calibration View, that calibrates the touch system
     * 
     * This calibration is stored in Mono persistent settings storage
     * @ref ISettings. If there already exists a calibration, then this calibration
     * will be loaded and the @ref setCalibrationDoneCallback handler will be called
     * immediately after the call to @ref show.
     *
     * @see ISettings
     */
    class TouchCalibrateView : public ResponderView
    {
    protected:
        
        mbed::FunctionPointer doneCallback;
        
        ITouchSystem::Calibration currentCal;
        
        uint8_t blockSize, blockMargin;
        TextLabelView textLbl, textHeader;
        display::Color blockColor, backgroundColor;
        
        geo::Point cals[4];
        int calStep;
        
        bool activateTouchOnRepaint;
        
        bool calibrationDone;

        bool firstRun;
        
        void drawTouchBlock(geo::Rect &rect, geo::Rect &prevRect);
        
        void calDone();
        
        void makeFirstResponder();
        
        void repaint();
        
        void initialize();
        
    public:

        /**
         * @brief Construct a CalibrationView that takes the whole display
         * 
         */
        TouchCalibrateView();

        /**
         * @brief Construct a CalibrationView inside a given rectangle
         * 
         * @param viewRct The rectangle on the display, where the view the shown
         */
        TouchCalibrateView(geo::Rect viewRct);

        /**
         * @brief Trigger a new calibration of the touch
         * 
         * This will force at new calibration to be done. The Calibration will
         * take over the touch input and screen and run its calibration routine.
         */
        void StartNewCalibration();
        
        void RespondTouchBegin(TouchEvent &event);
        
        template <typename Owner>
        void setCalibrationDoneCallback(Owner *obj,void (Owner::*memPtr)(void))
        {
            doneCallback.attach<Owner>(obj, memPtr);
        }
    };
    
} }

#endif /* touch_calibrator_h */
