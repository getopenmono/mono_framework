//
//  ili9225g.cpp
//  scrTest
//
//  Created by Kristoffer Andersen on 21/09/15.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

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

using namespace mono::display;

ILI9225G::ILI9225G() : IDisplayController(176,220),
    spi(TFT_SPI_MOSI, TFT_SPI_MISO, TFT_SPI_CLK, TFT_SPI_CS),
    Reset(TFT_RESET, 0),
    RegisterSelect(TFT_REGISTER_SELECT, 1),
    IM0(TFT_IM0, 1),
    curWindow(0,0,ScreenWidth(), ScreenHeight())
{
    
    //CY_SET_REG8(CYREG_PRT12_SIO_CFG, CY_GET_REG8(CYREG_PRT12_SIO_CFG) | 0x01);
    //
    IApplicationContext::Instance->PowerManager->AppendToPowerAwareQueue(this);
}

void ILI9225G::init()
{
    //mono::defaultSerial.printf("In constructor:\n\rDR: 0x%x, DM0: 0x%x, DM1: 0x%x, DM2: 0x%x\n\r",dr,dm0,dm1,dm2);
    //mono::defaultSerial.printf("After\n\rDR: 0x%x, DM0: 0x%x, DM1: 0x%x, DM2: 0x%x\n\r",CY_GET_REG8(CYREG_PRT12_DR), CY_GET_REG8(CYREG_PRT12_DM0), CY_GET_REG8(CYREG_PRT12_DM1),CY_GET_REG8(CYREG_PRT12_DM2));
    
    
    
    // setting pins agin since setting them in global constructor does not work YET!!
    // TODO: make pin assignment work (not get overwritten) in global constructors!
    Reset = mbed::DigitalOut(TFT_RESET, 1);
    RegisterSelect = mbed::DigitalOut(TFT_REGISTER_SELECT, 1);
    IM0 = mbed::DigitalOut(TFT_IM0, 1);
    spi.format(0);
    
    Reset = 0;
    wait_ms(15);
    Reset = 1; // active low
    wait_ms(50);
    
    defaultSerial.printf("display init routine!\n\r");
    
    //************* Start Initial Sequence **********//
    writeCommand(0x0011, 0x0000);
    writeCommand(0x0012, 0x0000);
    writeCommand(0x0013, 0x0000);
    writeCommand(0x0014, 0x0000);
    writeCommand(0x0011, 0x0041);
    writeCommand(0x0012, 0x5011);
    writeCommand(0x0013, 0x0062);
    writeCommand(0x0014, 0x4959);
    writeCommand(0x00C7, 0x0303);
    
    writeCommand(0x0010, 0x0200);
    writeCommand(0x0011, 0x1041);
    writeCommand(0x0001, 0x011C);
    writeCommand(0x0002, 0x0100);
    writeCommand(0x0003, 0x1030);
    writeCommand(0x0033, 0x1030);
    writeCommand(0x0007, 0x0000);
    writeCommand(0x0008, 0x0808);
    writeCommand(0x000B, 0x3100);
    writeCommand(0x000C, 0x0000);
    
    writeCommand(0x0030, 0x0000);
    writeCommand(0x0031, 0x00DB);
    writeCommand(0x0032, 0x0000);
    writeCommand(0x0033, 0x0000);
    writeCommand(0x0034, 0x00DB);
    writeCommand(0x0035, 0x0000);
    writeCommand(0x0036, 0x00AF);
    writeCommand(0x0037, 0x0000);
    writeCommand(0x0038, 0x00DB);
    writeCommand(0x0039, 0x0000);
    
    writeCommand(0x0050, 0x0001);
    writeCommand(0x0051, 0x030A);
    writeCommand(0x0052, 0x0C04);
    writeCommand(0x0053, 0x0804);
    writeCommand(0x0054, 0x040C);
    writeCommand(0x0055, 0x0A03);
    writeCommand(0x0056, 0x0100);
    writeCommand(0x0057, 0x0408);
    writeCommand(0x0058, 0x0000);
    writeCommand(0x0059, 0x0000);
    
    writeCommand(0x000F, 0x0701);
    writeCommand(0x0007, 0x1017);
    writeCommand(0x0020, 0x0000);
    writeCommand(0x0021, 0x0000);
    
    wait_ms(50);
    writeRegister(0x0022);
    RegisterSelect = 1;
    
    for (int i=0; i<176*220; i++) {
        this->write(BlackColor);
    }
    
    PWM_WriteCompare1(64);
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
    setWindow(x, y, 2, 2);
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
    SPI1_WriteTxData(pixelColor.value >> 8);
    SPI1_WriteTxData(pixelColor.value);
    //spi.write(pixelColor.value);
}

void ILI9225G::writeData(uint16_t data)
{
    RegisterSelect = 1;
    SPI1_WriteTxData(data >> 8);
    SPI1_WriteTxData(data);
}

void ILI9225G::writeRegister(uint16_t regData)
{
    RegisterSelect = 0;
    spi.write(regData & 0xFF);
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
    CyPins_SetPinDriveMode(TFT_TEARING_EFFECT, CY_PINS_DM_DIG_HIZ);
    CyPins_SetPinDriveMode(TFT_LED_PWR, CY_PINS_DM_RES_DWN);
    CyPins_ClearPin(TFT_LED_PWR);
    
    //PWM_Start();
    
    //PWM_WriteCompare1(0); // turn off display light
}

void ILI9225G::onSystemEnterSleep()
{
    PWM_Sleep();
    //CyPins_ClearPin(TFT_LED_PWR);
    writeCommand(0x07, 0x00);
    wait_ms(50);
    writeCommand(0x11, 0x07);
    wait_ms(50);
    writeCommand(0x10, 0x0A01); // put controller into standby
    
}

void ILI9225G::onSystemWakeFromSleep()
{
    writeCommand(0x10, 0x0A00);
    wait_ms(50);
    writeCommand(0x07, 0x1017);
    
    PWM_Wakeup();
}

void ILI9225G::OnSystemBatteryLow()
{
    PWM_WriteCompare1(32);
}
