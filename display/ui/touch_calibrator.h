//
//  touch_calibrator.h
//  mono framework
//
//  Created by Kristoffer Andersen on 06/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#ifndef touch_calibrator_h
#define touch_calibrator_h

#include "touch_system_interface.h"
#include <responder_view.h>
#include "text_label_view.h"

#include <FunctionPointer.h>

namespace mono { namespace ui {
    
    
    class TouchCalibrateView : public ResponderView
    {
    protected:
        
        mbed::FunctionPointer doneCallback;
        
        ITouchSystem::Calibration currentCal;
        
        uint8_t blockSize, blockMargin;
        TextLabelView textLbl, textHeader;
        display::Color blockColor;
        
        geo::Point cals[4];
        int calStep;
        
        bool activateTouchOnRepaint;
        
        void drawTouchBlock(geo::Rect &rect, geo::Rect &prevRect);
        
        void calDone();
        
        void makeFirstResponder();
        
    public:
        
        TouchCalibrateView();
        
        TouchCalibrateView(geo::Rect viewRct);
        
        void repaint();
        
        void RespondTouchBegin(TouchEvent &event);
        
        template <typename Owner>
        void setCalibrationDoneCallback(Owner *obj,void (Owner::*memPtr)(void))
        {
            doneCallback.attach<Owner>(obj, memPtr);
        }
    };
    
} }

#endif /* touch_calibrator_h */
