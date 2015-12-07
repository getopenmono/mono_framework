//
//  color.h
//  displaySimTest
//
//  Created by Kristoffer Andersen on 10/07/15.
//
//

#ifndef displaySimTest_color_h
#define displaySimTest_color_h

#include <stdint.h>



namespace mono { namespace display {
    
    class Color
    {
    public:
        uint16_t value;
        
        uint8_t* BytePointer();
        
        Color();
        
        Color(const int col);
        
        Color(const Color &color);
        
        Color(uint8_t R, uint8_t G, uint8_t B);
        
        uint16_t operator=(Color col);
        
        bool operator==(const Color &col);
        
        bool operator!=(const Color &col);
    };
    
    static const Color BlackColor = Color(1, 1, 1); // the value 0x0000 is transparent
    static const Color TransparentColor = Color(0,0,0);
    static const Color RedColor = Color(0xFF,0,0);
    static const Color GreenColor = Color(0,0xFF,0);
    static const Color BlueColor = Color(0,0,0xFF);
    static const Color WhiteColor = Color(0xFF,0xFF,0xFF);
    
    static const Color       CloudsColor(236,240,241);
    static const Color   WetAsphaltColor(52,73,94);
    static const Color     ConcreteColor(149,165,166);
    static const Color       SilverColor(189,195,199);
    static const Color     AsbestosColor(127,140,141);
    static const Color   BelizeHoleColor(41,128,185);
    static const Color MidnightBlueColor(44,62,80);
    static const Color     AlizarinColor(231,76,60);
    static const Color    TurquoiseColor(26,188,156);
    
} }

#endif
