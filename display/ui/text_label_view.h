//
//  text_label_view.h
//  mono
//
//  Created by Kristoffer Andersen on 14/09/15.
//  Copyright (c) 2015 Monolit ApS. All rights reserved.
//

#ifndef __mono__text_label_view__
#define __mono__text_label_view__

#include "view.h"
#include "mn_string.h"

namespace mono { namespace ui {
    
    class TextLabelView : public View {
    public:
        
        enum TextAlignment
        {
            ALIGN_LEFT,
            ALIGN_CENTER,
            ALIGN_RIGHT
        };
        
    protected:
        String text;
        
        uint8_t textSize;
        display::Color textColor;
        display::Color bgColor;
        TextAlignment alignment;
        
    public:
        
        TextLabelView(String txt = String());
        TextLabelView(const char *txt);
        TextLabelView(geo::Rect rct, String txt);
        TextLabelView(geo::Rect rct, const char *txt);
        
        // Getters
        uint8_t TextSize() const;
        display::Color TextColor() const;
        TextAlignment Alignment() const;
        
        uint16_t TextPixelWidth() const;
        uint16_t TextPixelHeight() const;
        
        // Setters
        void setTextSize(uint8_t newSize);
        void setTextColor(display::Color col);
        void setBackgroundColor(display::Color col);
        void setAlignment(TextAlignment align);
        
        void setText(char *text, bool resizeViewWidth = false);
        void setText(const char *txt, bool resizeViewWidth = false);
        void setText(String text, bool resizeViewWidth = false);
        
        void repaint();
    };
    
} }

#endif /* defined(__marqueeApp__text_label_view__) */
