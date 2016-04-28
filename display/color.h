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

    /**
     * @brief A RGB color representation, generic for display devices
     *
     * This class implements a 16-bit RGB 5-6-5 color model. It support methods
     * for calculating color blendings and more.
     *
     * THe class definition also define a set of global constant predefined 
     * colors, like white, red, green, blue and black. Further, it includes a
     * set of the FlatUI colors that Mono uses:
     *
     * * Clouds
     * * WetAsphalt
     * * Concrete
     * * Silver
     * * Asbestos
     * * BelizeHole
     * * MidnightBlue
     * * Alizarin
     * * Turquoise
     * * Emerald
     *
     */
    class Color
    {
    public:
        uint16_t value;
        


        /// MARK: Constructors

        
        Color();
        
        Color(const int col);
        
        Color(const Color &color);
        
        Color(uint8_t R, uint8_t G, uint8_t B);

        /// MARK: Getters

        /** Return 8-bit red color component */
        uint8_t Red() const;

        /** Return 8-bit green color component */
        uint8_t Green() const;

        /** Return 8-bit blue color component */
        uint8_t Blue() const;

        /// Misc.

        /**
         * Multiply each RGB channel by a factor from 0-255
         * @return the scaled color
         */
        Color scale(uint8_t factor) const;

        /**
         * Return the product of two colors
         * @return the multiply blended color
         */
        Color blendMultiply(Color other) const;

        /**
         * Add this color with another
         */
        Color blendAdditive(Color other) const;

        /** Return the inverse */
        Color invert() const;

        Color alphaBlend(uint8_t intensity, Color const &other) const;

        uint8_t* BytePointer();

        /// MARK: Operator overloads
        
        uint16_t operator=(Color col);
        
        bool operator==(const Color &col);
        
        bool operator!=(const Color &col);

        Color operator*(const Color &col);

        Color operator+(const Color &col);
    };

    /** A Black color */
    static const Color BlackColor = Color(1, 1, 1); // the value 0x0000 is transparent
    /** Transparent (Opacity = 0) color (Not supported right now) */
    static const Color TransparentColor = Color(0,0,0);
    /** Fully red color */
    static const Color RedColor = Color(0xFF,0,0);
    /** Fully green color */
    static const Color GreenColor = Color(0,0xFF,0);
    /** Fully blue color */
    static const Color BlueColor = Color(0,0,0xFF);
    /** Completely white color */
    static const Color WhiteColor = Color(0xFF,0xFF,0xFF);

    /** Clouds color (mostly white) */
    static const Color       CloudsColor(236,240,241);
    /** FlatUI WetAsphalt color (deep blue) */
    static const Color   WetAsphaltColor(52,73,94);
    /** FlatUI Concrete color (gray) */
    static const Color     ConcreteColor(149,165,166);
    /** FlatUI Silver color (light gray) */
    static const Color       SilverColor(189,195,199);
    /** FlatUI Asbestos color (dark gray) */
    static const Color     AsbestosColor(127,140,141);
    /** FlatUI BelizeHole color (light blue) */
    static const Color   BelizeHoleColor(41,128,185);
    /** FlatUI Midnight blue color (dark blue) */
    static const Color MidnightBlueColor(44,62,80);
    /** FlatUI Alizarin color (red) */
    static const Color     AlizarinColor(231,76,60);
    /** FlatUI Turquoise color */
    static const Color    TurquoiseColor(26,188,156);
    /** FlatUI Emerald color (green) */
    static const Color      EmeraldColor(0x2e,0xcc,0x71);
    
} }

#endif
