#ifndef _statusbar_scene_h
#define _statusbar_scene_h

#include <mono.h>
#include <icons/wifi-medium-signal-symbol-16.h>
#include <battery_view.h>

using namespace mono::ui;
using namespace mono::geo;

namespace mono { namespace ui {
    
    class StatusbarScene : public SceneController
    {
        
        BatteryView battery;
        IconView wifiOn;
        
    public:
        
        void setBatteryLevel(uint8_t percent);
        void setWifiOn(bool on);
        
        StatusbarScene(const Point &pos);
    };
    
} }

#endif
