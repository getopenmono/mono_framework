//
//  application_run_loop.cpp
//  displaySimTest
//
//  Created by Kristoffer Lyder Andersen on 12/07/15.
//
//

#include "application_run_loop.h"
#include <mbed.h>

using namespace mono;

AppRunLoop::AppRunLoop()
{
    runLoopActive = true;
    lastDtrValue = true;
    resetOnDTR = true;
}

void AppRunLoop::exec()
{
    mono::defaultSerial.printf("mono enter run loop!\n\r");
    
    lastDtrValue = mono::defaultSerial.DTR();
    
    while (runLoopActive) {
        
        if (mono::defaultSerial.IsReady())
        {
            bool dtr = mono::defaultSerial.DTR();
            if (resetOnDTR && (!dtr) && lastDtrValue)
            {
                mono::defaultSerial.printf("mono reboot!\n\r");
                CySoftwareReset();
            }
            lastDtrValue = dtr;
        }
        
        
        // what else todo
        mbed::wait_ms(1000);
    }
    
    mono::defaultSerial.printf("run loop terminated!");
}