//
//  mono_touch_system.h
//  analog
//
//  Created by Kristoffer Lyder Andersen on 05/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#ifndef mono_touch_system_h
#define mono_touch_system_h

#include <touch_system_interface.h>
#include "power_aware_interface.h"

extern "C" {
#include <project.h>
}

namespace mono {
    
    class MonoTouchSystem : public ITouchSystem, power::IPowerAware
    {
    protected:
        
        /** We try not to sample with more than 100 Hz */
        uint32_t lastTouchProcess;
        
        /** Calibration minimum touch input */
        uint16_t CalMinX, CalMinY;
        
        /** Calibration maximum touch input */
        uint16_t CalMaxX, CalMaxY;
        
        geo::Point lastLastPosition;
        
        uint16_t sampleX();
        uint16_t sampleY();
        
        void onSystemPowerOnReset();
        void onSystemEnterSleep();
        void onSystemWakeFromSleep();
        void OnSystemBatteryLow();
        
    public:
        
        MonoTouchSystem();
        
        void init();
        
        void processTouchInput();
        
        void setCalibration(ITouchSystem::Calibration &cal);
        
        int ToScreenCoordsX(int touchPos, uint16_t screenWidth);
        int ToScreenCoordsY(int touchPos, uint16_t screenHeight);
    };
}

#endif /* mono_touch_system_h */
