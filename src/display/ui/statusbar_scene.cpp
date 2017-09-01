#include <statusbar_scene.h>

using namespace mono::ui;

StatusbarScene::StatusbarScene(const Point &pos) : SceneController(Rect(pos.X(), pos.Y(), 176,20)),
    battery(Rect(viewRect.X2()-23, pos.Y()+5, 18, 10)),
    wifiOn(Point(viewRect.X2() - 25 - 20, 2), mono::display::wifiMediumSignalSymbol16)
{
    addView(battery);
    addView(wifiOn);
    
    battery.setBlinkWhenCritical(true);
    battery.setBatteryPercent(100);
    
    wifiOn.setForeground(BackgroundColor());
}


void StatusbarScene::setBatteryLevel(uint8_t percent)
{
    battery.setBatteryPercent(percent);
}

void StatusbarScene::setWifiOn(bool on)
{
    if (on)
        wifiOn.setForeground(View::StandardBorderColor);
    else
    {
        wifiOn.setForeground(BackgroundColor());
    }
    scheduleRepaint();
}
