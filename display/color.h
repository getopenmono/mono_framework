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
        
        Color(const int col);
        
        Color(const Color &color);
        
        uint16_t operator=(Color col);
        
        bool operator==(const Color &col);
        
        bool operator!=(const Color &col);
    };
    
    static const Color BlackColor = 0x0821; // the value 0x0000 is transparent
    static const Color TransparentColor = 0x0000;
    static const Color RedColor = 0xF800;
    static const Color GreenColor = 0x07E0;
    static const Color BlueColor = 0x001F;
    static const Color WhiteColor = 0xFFFF;
    
    
} }

#endif
