// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef displaySimTest_color_h
#define displaySimTest_color_h

#include <stdint.h>
#include <mn_string.h>


namespace mono { namespace display {

    /**
     * @brief A RGB color representation, generic for display devices
     *
     * This class implements a 16-bit RGB 5-6-5 color model. It support methods
     * for calculating color blendings and more.
     *
     * The class definition also define a set of global constant predefined
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



        // MARK: Constructors

        /**
         * @brief Construct a black color
         */
        Color();

        /**
         * @brief Construct a color from an existing 5-6-5 encoded value
         *
         * This constructor takes a an integer an casts it to a 5-6-5 RGB color,
         * from from the integers LSB's.
         * @param col The 5-6-5 RGB color value
         */
        Color(const int col);

        /**
         * @brief Construct a color from individual RGB components
         *
         * You provide 8-bit RGB components to create the color.
         *
         * @param R The red component, 0 to 255
         * @param G The red component, 0 to 255
         * @param B The red component, 0 to 255
         */
        Color(uint8_t R, uint8_t G, uint8_t B);
        
        Color(const Color &color);

        // MARK: Getters

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

        /**
         * @brief Blend this color with another using a provided intensity
         *
         * Alpha blending is done when you need to combine two colors, say you
         * wish to realize a gradient. If you blend black and white the intensity
         * will define all the grayscales in between.
         *
         * Use this method to realize an alpha channel that controls how
         * transparent a certain color is. The alpha channel controls the
         * intensity, and you must provide the color that lies *behind* the
         * alpha blended color.
         *
         * @param intensity The intensity of this color, 0 to 255.
         * @param other The color to blend with
         * @return The new blended color
         */
        Color alphaBlend(uint8_t intensity, Color const &other) const;

        uint8_t* BytePointer();

        /**
         * @brief Get a human readable string representatio of the color
         *
         * Returns a string of the form: (RR, GG, BB)
         *
         * @return a color string
         */
        mono::String toString() const;

        // MARK: Operator overloads

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
