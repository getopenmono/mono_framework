
#ifndef mono_icon_h
#define mono_icon_h

#include <stdint.h>

namespace mono { namespace display {
    
    typedef struct {
        uint16_t width;
        uint16_t height;
        uint8_t *bitmap;
    } MonoIcon;
    
} }

#endif /* mono_icon_h */
