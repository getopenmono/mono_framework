#ifndef _battery_view_h
#define _battery_view_h

#include <view.h>
#include <mn_timer.h>
#include <FunctionPointer.h>

using namespace mono::ui;
using namespace mono::geo;
using namespace mono::display;

namespace mono { namespace ui {
    
    class BatteryView : public View
    {
    protected:
        
        uint8_t lastBatteryPercent;
        uint8_t criticalAmount;
        int lastBatteryPixelLen;
        uint8_t dotSize;
        Color batteryCriticalColor;
        Color borderColor, backgroundColor;
        bool blinking, lastBlinkDrawed;
        mono::Timer blinker;
        
    public:
        
        mbed::FunctionPointerArg1<uint8_t, void> batteryPercentDelegate;
        
        BatteryView(const Rect &rct);
        
        void setBatteryPercent(uint8_t percent);
        uint8_t batteryPercent(void) const;
        
        void setBatteryCritical(const Color &col);
        const Color& batteryCritical() const;
        
        void setBorder(const Color &col);
        const Color& border() const;
        
        void setBackground(const Color &col);
        const Color& background() const;
        
        void setBlinkWhenCritical(bool blink);
        bool blinkWhenCritical() const;
        
        void setCriticalPercentAmount(uint8_t critical);
        uint8_t criticalPercentAmount() const;
        
        void repaint();
    };
    
} }

#endif
