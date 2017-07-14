// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef button_view_h
#define button_view_h

#include "responder_view.h"
#include "text_label_view.h"
#include <FunctionPointer.h>

using namespace mono::display;

namespace mono { namespace ui {

    /**
     * @brief A Push Button UI Widget
     *
     * This is a common state-less push button. It is basically a bordered text
     * label. This button reacts to touch input (pushes) and can call a function
     * when it is pushed.
     *
     * You provide the button with a callback function, that gets called when
     * the button is pushed. A valid button push is a touch that begins *and ends*
     * within the button boundaries. If a touch begins inside the buttons
     * boudaries, but ends outside - the button click callback is not triggered.
     *
     *
     * You define the button dimensions by the @ref Rect you provide in the
     * constructor. Note that the resistive touch panel is not that precise,
     * you should not create buttons smaller than 40x35 pixels. Also note that
     * buttons do not automatically scale, when you set their text content.
     *
     * ## Example
     * @code
     * // Create the button (should normally be defined as a class member)
     * mono::ui::ButtonView btn(mono::geo::Rect(10,10,100,35), "Push Here")
     *
     * // Setup a click handler
     * btn.setClickCallback<MyClass>(this, &MyClass::MyClickHandler);
     *
     * // show the button on the screen
     * btn.show();
     * @endcode
     */
    class ButtonView : public ResponderView
    {
    protected:

        mbed::FunctionPointer clickHandler;

        virtual void touchBegin(TouchEvent &event);
        virtual void touchMove(TouchEvent &event);
        virtual void touchEnd(TouchEvent &event);

        TextLabelView textLabel;

        bool isPressedDown;

        Color borderColor, textColor;
        Color borderColorPressed;
        Color background;

        void initButton();

    public:

        /**
         * @brief Construct an empty button
         *
         * The button will have zero dimensions and no text.
         */
        ButtonView();


        /**
         * @brief Construct a button with dimensions and text
         *
         * Creates a button with the provided dimensions and text to display. You
         * still need to setup a callback and call @ref show.
         *
         * @param rect The view rect, where the button is displayed
         * @param text The text that is showed on the button
         */
        ButtonView(geo::Rect rect, String text);

        /**
         * @brief Construct a button with dimensions and text
         *
         * Creates a button with the provided dimensions and text to display. You
         * still need to setup a callback and call @ref show.
         *
         * @param rect The view rect, where the button is displayed
         * @param text The text that is showed on the button
         */
        ButtonView(geo::Rect rect, const char *text);



        /**
         * @brief Set the text content
         *
         * Sets the text that is displayed on the button. Note that the width
         * and height of the button is not changed. You must change the buttons
         * @ref viewRect if your text is larger than the buttons dimensions.
         *
         * When you set new text content, the button is automatically repainted
         *
         * @param txt The new text content
         */
        void setText(String txt);

        /**
         * @brief Change the button fontface (font family and size)
         *
         * You can change the buttons font to use a larger (or smaller) font.
         *
         * @deprecated Use the GfxFont variant
         */
        void setFont(MonoFont const &newFont) __DEPRECATED("Use the method of same name, but provide a GfxFont","setFont");

        /**
         * @brief Change the buttons fontface (font family and size)
         *
         * You can change the buttons font to use a larger (or smaller) font.
         *
         * @param newFont The font face to render the text content with
         */
        void setFont(GFXfont const &newFont);

        /**
         * @brief Sets the border color
         *
         * This method will not schedule repaint! You must @ref scheduleRepaint
         * manually.
         *
         * @param c The new border color
         */
        void setBorder(Color c);

        /**
         * @brief Sets the text color
         *
         * This method will not schedule repaint! You must @ref scheduleRepaint
         * manually.
         *
         * @param c The new text color
         */
        void setText(Color c);

        /**
         * @brief Sets the highlight color (border & text)
         *
         * The highlight color is the color used to represented a button that is
         * currently being pushed. This means that a touch event has started
         * inside its boundaries. The highlight color is applied to both border
         * and text.
         *
         * This method will not schedule repaint! You must @ref scheduleRepaint
         * manually.
         *
         * @param c The new highlight color
         */
        void setHighlight(Color c);

        /**
         * @brief Sets the background color
         *
         * The background color of the fill color inside the button bounding
         * rectangle.
         *
         * This method will not schedule repaint! You must @ref scheduleRepaint
         * manually.
         *
         * @param c The new border and text color
         */
        void setBackground(Color c);

        void setRect(geo::Rect r);

        /**
         * @brief Get a reference to the internal TextLabel object
         *
         * You get a `const` reference to the button internal
         * @ref TextLabelView
         */
        const TextLabelView& TextLabel() const;


        /**
         * @brief Attach a member function as the button click handler
         *
         * Provide the callback member function you ewant to be called when
         * the button is clicked.
         *
         * **NOTE:** THere can only be one callback function
         *
         * @param obj A pointer to the object where the callback method exists
         * @param memPtr A pointer to the callback method itself
         */
        template <typename Owner>
        void setClickCallback(Owner *obj, void(Owner::*memPtr)(void))
        {
            clickHandler.attach<Owner>(obj, memPtr);
        }

        /**
         * @brief Attach a C function pointeras the button click handler
         *
         * Provide a pointer to the callback C function, you ewant to be called
         * when the button is clicked.
         *
         * **NOTE:** THere can only be one callback function.
         *
         * @param memPtr A pointer to the C function callback
         */
        void setClickCallback(void(*memPtr)(void))
        {
            clickHandler.attach(memPtr);
        }


        virtual void repaint();

    };


} }

#endif /* button_view_h */
