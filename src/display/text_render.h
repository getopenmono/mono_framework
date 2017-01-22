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
     * text, you need to provide a pointer to wither the @ref MonoFont structure,
     * or a Adafruit compatible @ref GFXfont structure. One these is used as the
     * rendered font.
     *
     * ### Monospaced MonoFonts
     * 
     * Mono has its own monospace-only font format @ref MonoFont
     *
     * This renderer has a palette like @ref DisplayPainter, and uses it the 
     * blend the semi-transparent pixels in the font anti-aliasing. The font 
     * bitmap defines pixel intensities, that are the foreground opacity.
     *
     * The MonoFont defines the text size and the anti-aliasing quality. Some fonts
     * has 2 bit pixels, others have 4 bit pixels.
     *
     * ### Adafruit GFXfonts
     *
     * TextRender can also work with Adafruits proportionally spaced fonts, that
     * include many different styles: Sans, serif, plain, italic, bold and 
     * combinations of these.
     *
     * This format is still bitmap based, but it does not used semi-transparent
     * pixels to achieve anti-aliasing. The bitmaps are one bit per pixel.
     *
     * Because coherent glyphs might overlap each others bounding rects, this
     * format does not draw text backgrounds. Since this will overwrite 
     * overlapping areas of text glyphs.
     *
     * #### Line Layouts
     *
     * The text rendering has two modes: line layout or not. Line layout means
     * a text line always has a fixed height: the line separation height. This
     * is critically when rendering multiline text layouts. But single line text
     * will appear off-centered in a vertically centered box. The reason is the
     * line height is much higher than the visual center of the text glyphs.
     *
     * If you need to render a single line of text at the center of a box, you
     * want to not use line layout mode. In this mode, text dimension height are
     * the distance from the texts hightest to lowest point.
     *
     */
    class TextRender
    {
    public:

        /** @brief The horizontal text alignment, when rendering inside rects */
        enum HorizontalAlignment
        {
            ALIGN_LEFT,
            ALIGN_CENTER,
            ALIGN_RIGHT
        };

        /** @brief The horizontal text alignment, when rendering inside rects */
        enum VerticalAlignmentType
        {
            ALIGN_TOP,
            ALIGN_MIDDLE,
            ALIGN_BOTTOM
        };

    protected:

        IDisplayController *dispCtrl;

        Color foregroundColor, backgroundColor;

        HorizontalAlignment hAlignment;
        VerticalAlignmentType vAlignment;

        /** Render a single character */
        void drawChar(geo::Point position, char character, const MonoFont &font, geo::Rect const &boundingRect);
        void drawChar(const geo::Point &position, const GFXfont &font, const GFXglyph *gfxGlyph, geo::Rect const &boundingRect, int lineHeight);
        
        int calcUnderBaseline(String text, const GFXfont &font);

        /** Blend and emit a single pixel to the DisplayController. */
        void writePixel(uint8_t intensity, bool bg = false);

        uint32_t remainingTextlineWidth(const GFXfont &font, const char *text);

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
         *
         * @param text The text to calculate the dimensions of
         * @param fontFace The font to use
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
         * @param lineLayout Default: `true`, Render text as a multiline layout
         */
        void drawInRect(const geo::Rect &rect, String text, const GFXfont &fontFace, bool lineLayout = true);

        /**
         * @brief Return the resulting dimension / size of some rendered text
         *
         * The final width and height of a rendered text, with the defined font
         * face.
         *
         * @param text The text to calculate the dimensions of
         * @param fontFace The font to use
         * @param lineLayout Default: `true`, use line layout or not
         */
        geo::Size renderDimension(String text, const GFXfont &fontFace, bool lineLayout = true);

        /**
         * @brief Return the calculated offset of the text in the drawing Rect
         *
         * The offset is returned in absolute coords, _not_ the relative coords
         * inside the drawing rect. The offset matters for text that is aligned
         * other than left and top.
         *
         * @param rect The drawing Rect, that the text should be rendered inside
         * @param text The text
         * @param fontFace The GFXfont to use
         * @param lineLayout Default: `true`, sets if lineLayout is used
         */
        geo::Rect renderInRect(const geo::Rect &rect, String text, const GFXfont &fontFace, bool lineLayout = true);

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
         * @brief Sets the texts justification within the drawing rect
         * @param align The alignment
         */
        void setAlignment(HorizontalAlignment align);


        /**
         * @brief Set the texts vertical alignment withnin the drawing rect
         * @param vAlign The vertical alignment
         */
        void setAlignment(VerticalAlignmentType vAlign);

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

        /** @brief Get the horizontal text alignment */
        HorizontalAlignment Alignment() const;

        /** @brief Get the vertical text alignment */
        VerticalAlignmentType VerticalAlignment() const;
    };

} }

#endif /* text_render_h */
