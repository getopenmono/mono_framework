// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef on_off_button_view_h
#define on_off_button_view_h

#include <abstract_button_view.h>
#include <text_label_view.h>

namespace mono { namespace ui {

    class OnOffButtonView : public AbstractButtonView
    {
    protected:
        bool state;

        display::Color backgroundColor, borderColor, enabledColor, highlightColor;

        TextLabelView onLbl, offLbl;

        uint16_t buttonSeparation;

        virtual void TouchEnd(TouchEvent &event);

    public:

        /**
         *
         * 
         */
        OnOffButtonView();

        /**
         *
         * 
         */
        OnOffButtonView(geo::Rect rct);


        /**
         *
         * 
         */
        void setBackground(display::Color c);

        /**
         *
         * 
         */
        void setBorder(display::Color c);


        /**
         *
         * 
         */
        void setHighlight(display::Color c);



        display::Color BackgroundColor() const;

        display::Color HighlightColor() const;

        display::Color BorderColor() const;

        bool isActive() const;


        void repaint();
    };

} }

#endif /* on_off_button_view_h */
