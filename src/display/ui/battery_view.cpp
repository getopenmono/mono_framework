#include <platform.h>
#include <battery_view.h>

using namespace mono::ui;

BatteryView::BatteryView(const Rect &rct) : View(rct),
    blinker(650, false)
{
    lastBatteryPercent = 0;
    criticalAmount = 20;
    dotSize = 1;
    lastBatteryPixelLen = 0;
    lastBatteryPercent = 0;
    batteryCriticalColor = RedColor;
    borderColor = StandardBorderColor;
    backgroundColor = StandardBackgroundColor;
    blinking = false;
    lastBlinkDrawed = false;

    blinker.setCallback<BatteryView>(this, &BatteryView::scheduleRepaint);
}

// MARK: Acessors

void BatteryView::setBatteryPercent(uint8_t percent)
{
    if (lastBatteryPercent == percent)
        return;
    
    lastBatteryPercent = percent;
    int fullLength = viewRect.Width() - dotSize - 4;
    lastBatteryPixelLen = lastBatteryPercent * fullLength / 100;
    
    scheduleRepaint();
}

uint8_t BatteryView::batteryPercent() const
{
    return lastBatteryPercent;
}

void BatteryView::setBatteryCritical(const Color &col)
{
    batteryCriticalColor = col;
}

const Color& BatteryView::batteryCritical() const
{
    return batteryCriticalColor;
}

void BatteryView::setBorder(const Color &col)
{
    borderColor = col;
}

const Color& BatteryView::border() const
{
    return borderColor;
}

void BatteryView::setBackground(const Color &col)
{
    backgroundColor = col;
}

const Color& BatteryView::background() const
{
    return backgroundColor;
}

void BatteryView::setBlinkWhenCritical(bool blink)
{
    blinking = blink;

    if (lastBlinkDrawed <= criticalAmount)
        scheduleRepaint();
}

bool BatteryView::blinkWhenCritical() const
{
    return blinking;
}

void BatteryView::setCriticalPercentAmount(uint8_t critical)
{
    criticalAmount = critical;
}

uint8_t BatteryView::criticalPercentAmount() const
{
    return criticalAmount;
}

// MARK: View overloads

void BatteryView::repaint()
{
    // battery dot size
    painter.setForegroundColor(StandardBorderColor);

    //draw bounds
    Rect bounds(viewRect.X(), viewRect.Y(), viewRect.Width() - dotSize, viewRect.Height());
    painter.drawRect(bounds);

    //draw dot
    Point d1(viewRect.X2() - dotSize, viewRect.Y() + viewRect.Height()/4);
    Point d2(viewRect.X2() - dotSize, viewRect.Y2() - viewRect.Height()/4);
    painter.drawLine(d1, d2);

    // update percentage if delegate is installed
    if (batteryPercentDelegate)
        this->setBatteryPercent(batteryPercentDelegate.call());

    if (lastBatteryPercent <= criticalAmount)
    {
        if (blinking)
            blinker.start();

        if (blinking && lastBlinkDrawed)
            painter.setForegroundColor(backgroundColor);
        else
            painter.setForegroundColor(batteryCriticalColor);
    }
    else
        blinker.stop();

    // draw battery level
    Rect bat(bounds.X()+2, bounds.Y()+2,
             lastBatteryPixelLen, bounds.Height() - 4);
    painter.drawFillRect(bat);

    if (blinking && lastBatteryPercent <= criticalAmount)
    {
        lastBlinkDrawed = !lastBlinkDrawed;
    }
}

