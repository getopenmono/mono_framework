
#ifndef __DISPLAY_HX8340__
#define __DISPLAY_HX8340__

#include <display_controller_interface.h>
#include <size.h>

namespace mono { namespace display {
    
    class HX8340 : public IDisplayController
    {
    public:
        
        int cursor;
        mono::geo::Size scrSize;
        
        enum DataCommandSelect
        {
            DATA_SELECT = 1,
            COMMAND_SELECT = 0
        };
        
        HX8340();
        
        void init();
       
        void setWindow(int x, int y, int width, int height);
       
        uint16_t ScreenWidth() const;
        uint16_t ScreenHeight() const;
       
        void setCursor(int x, int y);
       
        int getCursorX();
        int getCursorY();
        
        void write(Color pixelColor);
        
        uint16_t read();
        
        void setBrightness(uint8_t value);
        uint8_t Brightness() const;
        
        
//        void writeChar(char c);
//        void writeString(const char *str);
//        
//        void drawPixel(uint16 x, uint16_t y, bool bg = false);
//        void drawFillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool bg = false);
//        void drawChar(int16_t x, uint16_t y, unsigned char c, uint8_t size);
        
        //void setWindow(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
        
        void LCD_Write_COM(uint16_t comm);
        void LCD_Write_DATA(uint16_t data);
        void LCD_Write_COM_DATA(uint16_t reg, uint16_t data);
        
        
        
        
    };
    
    
} }

#endif