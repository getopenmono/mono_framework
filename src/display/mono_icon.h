
#ifndef mono_icon_h
#define mono_icon_h

#include <stdint.h>

namespace mono { namespace display {
    
    /**
     * @brief The icon type object for the Mono's icon system
     *
     * This struct defines an icon bitmap. The bitmap is a row-first array of
     * pixel values, that represents blending between two colors.
     *
     * @see mono::ui::IconView
     */
    typedef struct {
        uint16_t width;     /**< The icons width in pixels */
        uint16_t height;    /**< The icons height in pixels */
        uint8_t *bitmap;    /**< The raw icon pixel data */
    } MonoIcon;
    
} }

#endif /* mono_icon_h */
