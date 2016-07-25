// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifdef __cplusplus
extern "C" {
#endif
    #include <project.h>
#ifdef __cplusplus
}
#endif

#include <ili9225g.h>
#include <consoles.h>
#include <application_context_interface.h>
#include "act8600_power_system.h"

#include "../ui/view.h"

using namespace mono::display;

ILI9225G::ILI9225G() : IDisplayController(176,220),
    spi(TFT_SPI_MOSI, TFT_SPI_MISO, TFT_SPI_CLK, TFT_SPI_CS),
    Reset(TFT_RESET, 0),
    RegisterSelect(TFT_REGISTER_SELECT, 1),
    IM0(TFT_IM0, 1),
    tearingEffect(TFT_TEARING_EFFECT),
    curWindow(0,0,ScreenWidth(), ScreenHeight())
{
    setBrightness(0);

    tearingEffect.mode(PullNone);
    tearingInterruptPending = false;
    rebootDisplay = false;

    IApplicationContext::Instance->PowerManager->AppendToPowerAwareQueue(this);
    IApplicationContext::Instance->RunLoop->addDynamicTask(this);
}

void ILI9225G::init()
{
    // set IM0 high
    //CyPins_SetPinDriveMode(CYREG_PRT6_PC0, CY_PINS_DM_RES_UP);
    //CyPins_ClearPin(CYREG_PRT6_PC0);

    setBrightness(0);

    Reset = 0;
    wait_ms(100);
    Reset = 1; // active low
    wait_ms(100);

    writeCommand(0xd0, 0x0003);
    writeCommand(0xeb, 0x0b00);
    writeCommand(0xec, 0x000f);
    writeCommand(0xc7, 0x030f);
    writeCommand(0x01, 0x011C);
    writeCommand(0x02, 0x0100);
    writeCommand(0x03, 0x1030);
    writeCommand(0x07, 0x0000);
    writeCommand(0x08, 0x0808);
    writeCommand(0x0F, 0x0601);

    writeCommand(0x10, 0x0A00);
    writeCommand(0x11, 0x1B41);

    wait_ms(50);

    writeCommand(0x12, 0x200E);
    writeCommand(0x13, 0x0020);
    writeCommand(0x14, 0x4A5F);

    writeCommand(0x30, 0x0000);
    writeCommand(0x31, 0x00DB);
    writeCommand(0x32, 0x0000);
    writeCommand(0x33, 0x0000);
    writeCommand(0x34, 0x00DB);
    writeCommand(0x35, 0x0000);
    writeCommand(0x36, 0x00AF);
    writeCommand(0x37, 0x0000);
    writeCommand(0x38, 0x00DB);
    writeCommand(0x39, 0x0000);

    writeCommand(0x50, 0x0000);
    writeCommand(0x51, 0x0803);
    writeCommand(0x52, 0x0C07);
    writeCommand(0x53, 0x0501);
    writeCommand(0x54, 0x070C);
    writeCommand(0x55, 0x0308);
    writeCommand(0x56, 0x0000);
    writeCommand(0x57, 0x0105);
    writeCommand(0x58, 0x1100);
    writeCommand(0x59, 0x0011);

    writeCommand(0x20, 0x0000);
    writeCommand(0x21, 0x0000);

    wait_ms(50);

    writeCommand(0x07, 0x1017);
    writeRegister(0x22);

    RegisterSelect = 1;

    int start = us_ticker_read();

    for (int i=0; i<176*220; i++) {
        this->write(ui::View::StandardBackgroundColor);
    }
    int end = us_ticker_read();
    debug("\r\ndisplay full paint time: %i\r\n",end-start);

    PWM_Start();
    setBrightness(255);

    //tearingEffect.DeactivateUntilHandled();
    tearingEffect.rise<ILI9225G>(this, &ILI9225G::tearingEffectHandler);

    tearingWatchdog.attach_us<ILI9225G>(this, &ILI9225G::tearingWatchdogHandler, 100000);
}

void ILI9225G::setWindow(int x, int y, int width, int height)
{
//    uint8_t buf[19] = {
//        0x36, 0, (x + width - 1)&0xFF,
//        0x37, 0, x,
//        0x38, 0, (y + height - 1),
//        0x39, 0, y,
//        0x20, 0, x,
//        0x21, 0, y,
//        0x22
//    };
    writeCommand(0x36, x + width - 1); 					//x end point
    writeCommand(0x37, x);							//x start point
    writeCommand(0x38, y + height - 1); 					    //y end point
    writeCommand(0x39, y);	                        //y start point

    // Set the initial value of address Counter
    writeCommand(0x20, x);
    writeCommand(0x21, y);
    writeRegister(0x22);
    RegisterSelect = 1;
}

void ILI9225G::tearingEffectHandler()
{
    LastTearningEffectTime = us_ticker_read();
    tearingInterruptPending = true;
}

void ILI9225G::taskHandler()
{
    if ((tearingInterruptPending && refreshHandler))
    {
        refreshHandler->call();
        tearingInterruptPending = false;
    }

}

void ILI9225G::tearingWatchdogHandler()
{
    watTime = us_ticker_read();
    teWat = LastTearningEffectTime;
}

uint16_t ILI9225G::ScreenWidth() const
{
    return 176;
}

uint16_t ILI9225G::ScreenHeight() const
{
    return 220;
}


void ILI9225G::setCursor(int x, int y)
{
    // Set the initial value of address Counter
    writeCommand(0x20, x);
    writeCommand(0x21, y);
    writeRegister(0x22);
    RegisterSelect = 1;
}


int ILI9225G::getCursorX()
{
    return 0;
}

int ILI9225G::getCursorY()
{
    return 0;
}

void ILI9225G::write(Color pixelColor)
{
    // Bit 8 is 1 for Data
    SPI1_WriteTxData(0x0100 | (*(((uint8_t*)&pixelColor)+1)));
    SPI1_WriteTxData(0x0100 | pixelColor.value);
}

void ILI9225G::writeData(uint16_t data)
{

    //RegisterSelect = 1;
    // Bit 8 is 1 for Data
    SPI1_WriteTxData(0x0100 | (*(((uint8_t*)&data)+1)));
    SPI1_WriteTxData(0x0100 | data);
}

void ILI9225G::writeRegister(uint16_t regData)
{
    //RegisterSelect = 0;
    // Bit 8 is 0 for Command
    SPI1_WriteTxData(0x0000 | (regData & 0xFF));
}

void ILI9225G::writeCommand(uint16_t regData, uint16_t data)
{
    writeRegister(regData);
    writeData(data);
}

uint16_t ILI9225G::read()
{
    return 0;
}

void ILI9225G::setBrightness(uint8_t value)
{
    PWM_WriteCompare1(value);
}

uint8_t ILI9225G::Brightness() const
{
    return PWM_ReadCompare1();
}


/// Power Aware protocol

void ILI9225G::onSystemPowerOnReset()
{
    SPI1_Start();
}

void ILI9225G::onSystemEnterSleep()
{
//    PWM_Sleep();
    SPI1_Stop();
    SPI1_Sleep();
    //CyPins_ClearPin(TFT_LED_PWR);
//    writeCommand(0x07, 0x00);
//    wait_ms(50);
//    writeCommand(0x11, 0x07);
//    wait_ms(50);
//    writeCommand(0x10, 0x0A01); // put controller into standby

}

void ILI9225G::onSystemWakeFromSleep()
{
//    writeCommand(0x10, 0x0A00);
//    wait_ms(50);
//    writeCommand(0x07, 0x1017);
//
    SPI1_Wakeup();
    SPI1_Start();
//    PWM_Wakeup();

    init();
}

void ILI9225G::OnSystemBatteryLow()
{
    debug("ILI9225: battery low, set brightness to 12%\t\n");
    setBrightness(32);
}
