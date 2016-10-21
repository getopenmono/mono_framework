// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef font_interface_h
#define font_interface_h

#include <stdint.h>

/**
 * @brief Bitmap based Monospaced Font Structure
 *
 * This struct defines the attributes and properties of a system font.
 *
 * All types are `const` such that the compiler are able to place them in the
 * `.rodata` section, which reside in the flash memory.
 */
struct MonoFont {
    const uint8_t *bitmap;   /**< A pointer the the font bitmap */
    const char *fontName;           /**< The human name of the font face */
    const uint16_t bitmapHeight;    /**< The total height of the complete bitmap */
    const uint16_t bitmapWidth;     /**< The total width of the complete bitmap */
    const uint16_t glyphHeight;     /**< The glyph height */
    const uint16_t glyphWidth;      /**< The glyph width */
    const uint8_t bitrate;          /**< The number of bits per pixel */
    const uint8_t characterOffset;  /**< Offset of the bitmaps first ASCII character */
    const uint8_t baselineOffset;   /**< The number of excess pixels below the fonts baseline */
};

#endif /* font_interface_h */
