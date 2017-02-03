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

        /** Function interface that for character drawing routines */
        typedef void(*charDrawFunction)(geo::Point &position, char character, const GFXfont &font, geo::Rect const &boundRect, int lineHeight);

        /** @brief The horizontal text alignment, when rendering inside rects */
        enum HorizontalAlignment
        {
            ALIGN_LEFT,     /**< Left justify text inside containing Rect */
            ALIGN_CENTER,   /**< Justify at center of containing Rect */
            ALIGN_RIGHT     /**< Right justify text inside containing Rect */
        };

        /** @brief The horizontal text alignment, when rendering inside rects */
        enum VerticalAlignmentType
        {
            ALIGN_TOP,      /**< Place text at the top of containing Rect */
            ALIGN_MIDDLE,   /**< Place text at the vertical center of containing Rect */
            ALIGN_BOTTOM    /**< Place text at the bottom of containing Rect*/
        };

    protected:

        IDisplayController *dispCtrl;

        Color foregroundColor, backgroundColor;

        HorizontalAlignment hAlignment;
        VerticalAlignmentType vAlignment;

        /**
         * @brief Render a single MonoFont character
         *
         * @deprecated Use the GfxFont method instead
         */
        void drawChar(geo::Point position, char character, const MonoFont &font, geo::Rect const &boundingRect);

        /**
         * @brief Calculated the maximum offset under the text baseline
         * 
         * This method returns the maximum offset under the baseline for some
         * text. Some characters (glyphs) has parts under the baseline, examples 
         * are lower-case _g_, _p_, _q_ and _j_.
         *
         * To align all glyphs in a text line, the character with the largest
         * undershoot is found and returned here.
         *
         * @param text The text content to process
         * @patam font The font face to use
         * @return The maximum offset below the text baseline
         */
        int calcUnderBaseline(String text, const GFXfont &font);

        /** Blend and emit a single pixel to the DisplayController. */
        void writePixel(uint8_t intensity, bool bg = false);

        /**
         * @brief Returns the pixel width of the text provided
         * 
         * This method is used in multiline text rendering, to get the width
         * of a stand-alone line inside a multiline text blob. Unlike @ref
         * renderDimensions that gets the width of the longest line, this method
         * get the length of the current line.
         *
         * @param font The font face to use
         * @param The C string text to get the current line from (until newline or end)
         * @return The pixel width of the line
         */
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
         * @brief Layout the string in a rect, using af callback to handle characters
         * 
         * This method will parse a text string and trigger a callback for each 
         * character, providing the position and allowed dimension of each 
         * character.
         *
         * @param rect The rectangle to render in
         * @param text The text string to render
         * @param fontFace A pointer the Adafruit GFX font to use
         * @param self A pointer to the callback method context object
         * @param memptr A pointer to the callback method
         * @param lineLayout Default: `true`, Render text as a multiline layout
         */
        template <typename Context>
        void layoutInRect(geo::Rect rect,
                          const String text,
                          const GFXfont &fontFace,
                          Context *self,
                          void(Context::*memptr)(const geo::Point &position, const GFXfont &font, const GFXglyph *gfxGlyph, geo::Rect const &boundingRect, const int lineHeight),
                          bool lineLayout = true)
        {
            if (dispCtrl == 0)
                return;
            if (text.Length() == 0)
                return;

            dispCtrl->setWindow(rect.X(), rect.Y(), rect.Width(), rect.Height());
            int cnt = 0;
            char c = text[cnt];

            geo::Rect offset = renderInRect(rect, text, fontFace, lineLayout);
            geo::Rect dim = offset;

            GFXglyph *glyph = &fontFace.glyph[c - fontFace.first];

            bool firstCharInLine = true;
            int lineHeight;
            if (lineLayout)
                lineHeight = fontFace.yAdvance;
            else
                lineHeight = dim.Height() - calcUnderBaseline(text, fontFace);

            while (c != '\0')
            {
                if (c == '\n')
                {
                    firstCharInLine = true;
                    offset.appendY(fontFace.yAdvance);
                    // +1 to skip newline char
                    uint32_t textWidth = remainingTextlineWidth(fontFace, text.stringData+cnt+1);
                    offset.setX(rect.X());

                    if (rect.Width() - textWidth > 0)
                    {
                        switch (hAlignment) {
                            case ALIGN_CENTER:
                                offset.setX(rect.X() + (rect.Width() - textWidth)/2 );
                                break;
                            case ALIGN_RIGHT:
                                offset.setX(rect.X() + rect.Width() - textWidth );
                                break;
                            default:
                                break;
                        }
                    }
                }
                else if (c == ' ')
                {
                    offset.appendX(glyph->xAdvance);
                }
                else if (offset.X()+glyph->width <= rect.X2())
                {
                    if (firstCharInLine) {
                        offset.appendX( -glyph->xOffset);
                    }

                    void (Context::**m)(const geo::Point &, const GFXfont &, const GFXglyph *, geo::Rect const &, const int) = reinterpret_cast<void(Context::**)(const geo::Point &, const GFXfont &, const GFXglyph *, geo::Rect const &, const int)>(&memptr);

                    (self->**m)(offset, fontFace, glyph, rect, lineHeight);
                    offset.appendX(glyph->xAdvance);
                }
                else
                {
                }
                
                firstCharInLine = false;
                c = text[++cnt];
                glyph = &fontFace.glyph[c - fontFace.first];
            }
        }

        /**
         * @brief Renders a text string in a provided Rectangle
         * 
         * THis method paints / renders the text in bounding rectangle. The text
         * is always rendered with origin in the rectangles top left corner. If
         * the provided @ref Rect is not large enough, the text is clipped!
         *
         * @deprecated Use the GfxFont variant instead
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
         * @deprecated Use the GfxFont variant instead
         * @param text The text to calculate the dimensions of
         * @param fontFace The font to use
         */
        geo::Size renderDimension(String text, const MonoFont &fontFace);

        /**
         * @brief Render a single Adafruit GfxFont character
         *
         * @param position The point where the glyph is rendered
         * @param font The Adafruit GfxFont to use
         * @param gfxGlyph The specific glyph to render
         * @param boundingRect The Rect that limits the render canvas (no paints beyond this Rect)
         * @param lineHeight The glyph height offset, to align glyphs on the same baseline
         */
        void drawChar(const geo::Point &position, const GFXfont &font, const GFXglyph *gfxGlyph, geo::Rect const &boundingRect, const int lineHeight);

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
