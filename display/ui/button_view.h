// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef button_view_h
#define button_view_h

#include "responder_view.h"
#include "text_label_view.h"
#include <FunctionPointer.h>

using namespace mono::display;

namespace mono { namespace ui {
    
    class ButtonView : public ResponderView
    {
    protected:
        
        mbed::FunctionPointer clickHandler;
        
        virtual void TouchBegin(TouchEvent &event);
        virtual void TouchMove(TouchEvent &event);
        virtual void TouchEnd(TouchEvent &event);
        
        TextLabelView textLabel;
        
        bool isPressedDown;
        
        Color borderColor;
        Color borderColorPressed;
        Color background;
        
        void initButton();
        
    public:
        
        ButtonView();
        
        
        ButtonView(geo::Rect rect, String text);
        ButtonView(geo::Rect rect, const char *text);

        /// MARK: Accessors
        void setText(String txt);
        void setFont(MonoFont const &newFont);

        // colors
        /**
         * @brief Sets the border and text color
         * This method will not schedule repaint!
         */
        void setBorder(Color c);

        /**
         * @brief Sets the active / pressed color (border & text)
         * This method will not schedule repaint!
         */
        void setHighlight(Color c);

        /**
         * @brief Sets the background color
         * This method will not schedule repaint!
         */
        void setBackground(Color c);

        /**
         * @brief Get a reference to the internal TextLabel object
         */
        const TextLabelView& TextLabel() const;

        /// MARK: Callbacks
        
        /**
         * @brief Attach a member function as the button click handler
         * 
         * Provide the callback member function you ewant to be called when
         * the button is clicked.
         * 
         * **NOTE:** THere can only be one callback function
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
         */
        void setClickCallback(void(*memPtr)(void))
        {
            clickHandler.attach(memPtr);
        }

        virtual void repaint();
        
    };
    
    
} }

#endif /* button_view_h */
