// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef text_render_h
#define text_render_h

#include "display_controller_interface.h"
#include "display_painter.h"
#include "font_interface.h"
#include <rect.h>
#include <mn_string.h>
#include "gfxfont.h"

namespace mono { namespace display {

    /**
     * @brief Text Render class to paint Text paragraphs on a DisplayController
     * 
     * This is a Text glyph and paragraph render. It uses a bitmap based fonts
     * and typesets the text to provide.
     *
     * You need to provide the text render with either a @ref DisplayPainter or 
     * @ref IDisplayController that serves as a target for the text rendering.
     *
     * The TextRender does not include any Font definitions. When you render your
     * text, you need to provide a pointer to the @ref MonoFont structure, that
     * should by used as the rendered font.
     *
     * This renderer has a palette like @ref DisplayPainter, and uses it the 
     * blend the semi-transparent pixels in the font anti-aliasing. The font 
     * bitmap defines pixel intensities, that are the foreground opacity.
     *
     * The Font defines the text size and the anti-aliasing quality. Some fonts
     * has 2 bit pixels, others have 4 bit pixels.
     */
    class TextRender
    {
    protected:

        IDisplayController *dispCtrl;

        Color foregroundColor, backgroundColor;

        /** Render a single character */
        void drawChar(geo::Point position, char character, const MonoFont &font, geo::Rect const &boundingRect);
        void drawChar(geo::Point position, const GFXfont &font, const GFXglyph *gfxGlyph, geo::Rect const &boundingRect);

        /** Blend and emit a single pixel to the DisplayController. */
        void writePixel(uint8_t intensity, bool bg = false);

    public:

        /**
         * @brief Construct a TextRender that renders to a DisplayController
         * Text Colors default to View::Standard colors
         *
         * @param displayCtrl A pointer to the display controller that is the render target
         */
        TextRender(IDisplayController *displayCtrl = 0);

        /**
         * @brief Construct a TextRender that renders to a DisplayController
         * You provide explicit text colors
         *
         * @param displayCtrl A pointer to the display controller that is the render target
         * @param foreground The text color
         * @param background the background color
         */
        TextRender(IDisplayController *displayCtrl, Color foreground, Color background);

        /**
         * @brief Construct a TextRender that renders to the DisplayController 
         * provided by a DisplayPainter.
         * The painter current pencil colors are used for the text color
         *
         * @param painter The display painter to copy DisplayController and color palette from
         */
        TextRender(const DisplayPainter &painter);

        /**
         * @brief Renders a text string in a provided Rectangle
         * 
         * THis method paints / renders the text in bounding rectangle. The text
         * is always rendered with origin in the rectangles top left corner. If
         * the provided @ref Rect is not large enough, the text is clipped!
         *
         * @param rect The rectangle to render in
         * @param text The text string to render
         * @param fontFace A pointer the fontface to use
         */
        void drawInRect(geo::Rect rect, String text, const MonoFont &fontFace);

        /**
         * @brief Return the resulting dimension / size of some rendered text
         * 
         * The final width and height of a rendered text, with the defined font
         * face.
         */
        geo::Size renderDimension(String text, const MonoFont &fontFace);

        /**
         * @brief Renders a text string in a provided Rectangle
         *
         * THis method paints / renders the text in bounding rectangle. The text
         * is always rendered with origin in the rectangles top left corner. If
         * the provided @ref Rect is not large enough, the text is clipped!
         *
         * @param rect The rectangle to render in
         * @param text The text string to render
         * @param fontFace A pointer the Adafruit GFX font to use
         */
        void drawInRect(geo::Rect rect, String text, const GFXfont &fontFace);

        /**
         * @brief Return the resulting dimension / size of some rendered text
         *
         * The final width and height of a rendered text, with the defined font
         * face.
         */
        geo::Size renderDimension(String text, const GFXfont &fontFace);


        /// MARK: Accessors

        /**
         * @brief Set the text color
         * 
         */
        void setForeground(Color fg);

        /**
         * @brief Set the background color.
         * Transparent pixels will be blended with this color.
         */
        void setBackground(Color bg);

        /**
         * @brief Get the current text color
         * 
         */
        Color Foreground() const;

        /**
         * @brief Get the current text background color
         * 
         */
        Color Background() const;
    };

} }

#endif /* text_render_h */
