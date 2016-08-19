// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef __mono__text_label_view__
#define __mono__text_label_view__

#include "view.h"
#include "mn_string.h"
#include <font_interface.h>

namespace mono { namespace ui {

    class ButtonView;
    class TouchCalibrateView;

    /**
     * @brief A Text Label displays text strings on the display
     *
     * Use this UI view whenever you need to display text on the screen.
     *
     * A text label renders text strings on the display. As all views the label
     * lives inside a defined rectangle (@ref viewRect), where the text is
     * rendered. If the rectangle is smaller than the length of the text
     * content, the content will cropped. If the rectangle is larger, then you
     * can align the text inside the rectangle (left, center or right).
     *
     * ### Example
     *
     * You can mix and match mono strings with standard C strings when
     * constructing TextLabels.
     *
     * **Create a label using a `static const` C string:**
     * @code
     *  TextLabelView lbl("This is a contant string");
     * @endcode
     *
     * Also you can use C strings allocated on the stack:
     * @code
     *  char text[4] = {'m', 'o', 'n', 'o'};
     *  TextLabelView lbl(text);
     * @endcode
     *
     * Above the TextLabel will take a copy of the input string, to ensure it
     * can be accessed asynchronously.
     *
     * ### Content
     *
     * The text view contains it content (a @ref String object), and therefore
     * has a state. You get and set the content to update the rendered text on
     * the display.When you set new text content the label automatically
     * re-renders itself. (By calling @ref scheduleRepaint)
     *
     * Because the view is rendered asynchronously, its text content must be
     * allocated on the heap. Therefore it uses the @ref String as text storage.
     * You can provide it with C strings, but these must allocated inside the
     * .rodata segment of your binary. (Meaning they are `static const`.)
     *
     * ### Text Format
     *
     * Currently there are only one font type. But the text color and font can
     * changed. You change these for parameters:
     *
     * * Text font (including size)
     * * Text color
     * * Text background color (the color behind the characters)
     *
     * ### Getting text dimensions
     *
     * To help you layout your views, you can query the `TextLabel` of the current
     * width and height of its contents. The methods @ref TextPixelWidth and
     * @ref TextPixelHeight, will return the texts dimensions in pixels
     * - regardless of view rectangle. Also, you can use these methods before
     * the view has been rendered.
     *
     */
    class TextLabelView : public View {
        friend ButtonView;
        friend TouchCalibrateView;
    public:

        /**
         * @brief Three ways of justifing text inside the TextLabel
         */
        enum TextAlignment
        {
            ALIGN_LEFT,     /**< Align text to the left */
            ALIGN_CENTER,   /**< Align text in the center */
            ALIGN_RIGHT     /**< Align text to the right */
        };

        /**
         * @brief This is the default font for all TextLabelView's
         *
         * This points to the default Textlabel font. You can overwrite this in
         * your own code to change the default appearence of all TextLabels.
         *
         * You can also overwrite it to use a less memory expensive
         * (lower quality) font face.
         */
        static const MonoFont *StandardTextFont;

    protected:

        String text;
        String prevText;

        const MonoFont *currentFont;
        uint8_t textSize;
        display::Color textColor;
        display::Color bgColor;
        TextAlignment alignment;

    public:

        // MARK: Public Constructors

        /**
         * @brief Construct a text label with defined content, but no dimensions
         *
         * Before you can render the label you still need to set the view
         * dimensions. This constructor take the String object as defined in the
         * mono framework.
         *
         * @param txt The labels text content (as a mono lightweight string)
         */
        TextLabelView(String txt = String());

        /**
         * @brief Construct a text label with defined content, but no dimensions
         *
         * Before you can render the label you still need to set the view
         * dimensions. This constructor takes a `static const` C string pointer
         * that must *not* exist on the stack! (It must live inside the `.rodata`
         * segment.
         *
         * @param txt A pointer to the static const C string (.rodata based)
         */
        TextLabelView(const char *txt);

        /**
         * @brief Construct a label in a defined rectangle and with a string
         *
         * You provide the position and size of the label, along with its text
         * content. You can call this constructor using a mono type string or a
         * stack based C string - and it is automatically converted to a mono
         * string:
         *
         * @code
         *  int celcius = 22;
         *
         *  // char array (string) on the stack
         *  char strArray[50];
         *
         *  // format the string content
         *  sprintf(strArray,"%i celcius", celcius);
         *
         *  // construct the label with our stack based string
         *  TextLabelView lbl(geo::Rect(0,0,100,100), strArray);
         * @endcode
         *
         */
        TextLabelView(geo::Rect rct, String txt);

        /**
         * @brief Construct a label in a defined rectangle and with a string
         *
         * You provide the position and size of the label, along with its text
         * content. You can call this constructor using `static const` C string:
         *
         * @code
         *  // construct the label with our stack based string
         *  TextLabelView lbl(geo::Rect(0,0,100,100), "I am a .rodata string!");
         * @endcode
         *
         */
        TextLabelView(geo::Rect rct, const char *txt);

        // MARK: Getters

        /**
         * @deprecated Use the @ref Font accessor instead
         *
         * The text size will be phased out in coming releases. You control text
         * by changing the font.
         */
        uint8_t TextSize() const;

        display::Color TextColor() const;
        TextAlignment Alignment() const;
        bool incrementalRepaint;

        uint16_t TextPixelWidth() const;
        uint16_t TextPixelHeight() const;

        const MonoFont& Font() const;

        // MARK: Setters

        /**
         * @deprecated Use the @ref setFont accessor instead
         *
         * We will phase out this attribute in the coming releases. To change
         * the font size you should rely on the font face.
         *
         * If you set this to 1 the old font (very bulky) font will be used. Any
         * other value will load the new default font.
         */
        void setTextSize(uint8_t newSize);

        /**
         * @deprecated Name will change. Use @ref setText
         */
        void setTextColor(display::Color col);

        /**
         * @deprecated Name will change. Use @ref setBackground
         */
        void setBackgroundColor(display::Color col);

        /** Set the text color */
        void setText(display::Color col);

        /** Set the color behind the text */
        void setBackground(display::Color col);

        /** Controls text justification: center, right, left */
        void setAlignment(TextAlignment align);

        void setText(char *text, bool resizeViewWidth = false);
        void setText(const char *txt, bool resizeViewWidth = false);
        void setText(String text, bool resizeViewWidth = false);

        /**
         * @brief Set a new font face on the label
         *
         * You can pass any @ref MonoFont to the label to change its appearence.
         * Fonts are header files that you must include youself. Each header file
         * defines a font in a specific size.
         *
         * The header file defines a gloabl `const` variable that you pass to
         * to this method.
         */
        void setFont(MonoFont const &newFont);

    public:

        void scheduleRepaint();

        void repaint();
    };

} }

#endif /* defined(__marqueeApp__text_label_view__) */
