// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "text_label_view.h"
#include <string.h>
#include <ptmono15.h>
#include <text_render.h>

using namespace mono::ui;

__attribute__((weak)) const MonoFont *TextLabelView::StandardTextFont = &mono::display::PT_Mono_15;

TextLabelView::TextLabelView(String txt) :
    textColor(StandardTextColor),
    bgColor(StandardBackgroundColor)
{
    this->incrementalRepaint = false;
    currentFont = TextLabelView::StandardTextFont;
    this->text = txt;
    this->setTextSize(2);

    viewRect.setSize( geo::Size(TextPixelWidth(), TextPixelHeight()) );
}

TextLabelView::TextLabelView(const char *txt) :
    textColor(StandardTextColor),
    bgColor(StandardBackgroundColor)
{
    this->incrementalRepaint = false;
    currentFont = TextLabelView::StandardTextFont;
    this->text = txt;
    this->setTextSize(2);

    viewRect.setSize( geo::Size(TextPixelWidth(), TextPixelHeight()) );
}

TextLabelView::TextLabelView(geo::Rect rct, String txt) :
    View(rct),
    textColor(StandardTextColor),
    bgColor(StandardBackgroundColor)
{
    this->incrementalRepaint = false;
    currentFont = TextLabelView::StandardTextFont;
    this->text = txt;
    this->setTextSize(2);
}

TextLabelView::TextLabelView(geo::Rect rct, const char *txt) :
    View(rct),
    textColor(StandardTextColor),
    bgColor(StandardBackgroundColor)
{
    this->incrementalRepaint = false;
    currentFont = TextLabelView::StandardTextFont;
    this->text = txt;
    this->setTextSize(2);
}

/// MARK: Getters
uint8_t TextLabelView::TextSize() const
{
    return textSize;
}

mono::display::Color TextLabelView::TextColor() const
{
    return textColor;
}

TextLabelView::TextAlignment TextLabelView::Alignment() const
{
    return alignment;
}

uint16_t TextLabelView::TextPixelWidth() const
{
    if (textSize == 1)
        return text.Length()*5*TextSize()+TextSize()+1;
    else
    {
        display::TextRender tr(painter);
        return tr.renderDimension(text, *currentFont).Width();
    }
}

uint16_t TextLabelView::TextPixelHeight() const
{
    if (textSize == 1)
        return 7*TextSize()+1;
    else
    {
        display::TextRender tr(painter);
        return tr.renderDimension(text, *currentFont).Height();
    }
}

const MonoFont& TextLabelView::Font() const
{
    return *currentFont;
}

/// MARK: Setters
void TextLabelView::setTextSize(uint8_t newSize)
{
    textSize = newSize;
}

void TextLabelView::setTextColor(display::Color col)
{
    setText(col);
}

void TextLabelView::setBackgroundColor(display::Color col)
{
    setBackground(col);
}

void TextLabelView::setText(display::Color col)
{
    textColor = col;
}

/** Set the color behind the text */
void TextLabelView::setBackground(display::Color col)
{
    bgColor = col;
}

void TextLabelView::setAlignment(TextAlignment align)
{
    alignment = align;
}

void TextLabelView::setText(char *text, bool resizeViewWidth)
{
    this->setText(String(text), resizeViewWidth);
}

void TextLabelView::setText(const char *txt, bool resizeViewWidth)
{
    this->setText(String(txt), resizeViewWidth);
}

void TextLabelView::setText(mono::String text, bool resizeViewWidth)
{
    this->text = String(text);
    if (resizeViewWidth)
    {
        viewRect.setWidth(TextPixelWidth());
    }

    if (isDirty)
        return;

    scheduleRepaint();
    incrementalRepaint = true;
}

void TextLabelView::setFont(const MonoFont &newFont)
{
    currentFont = &newFont;

    scheduleRepaint();
}

/// MARK: Aux Functions

void TextLabelView::scheduleRepaint()
{
    this->incrementalRepaint = false;

    if (isDirty || !visible)
        return;

    this->isDirty = true;
    dirtyQueue.Enqueue((View*) this);
}

void TextLabelView::repaint()
{
    mono::geo::Point offset = this->viewRect.Point();

    switch (alignment) {
        case ALIGN_RIGHT:
            offset.setX(viewRect.X() + viewRect.Width() - TextPixelWidth());
            break;
        case ALIGN_CENTER:
            if (TextPixelWidth() > viewRect.Width())
                break;
            offset.setX( viewRect.X() + ((viewRect.Width() / 2) - (TextPixelWidth() / 2)) );
            break;
        case ALIGN_LEFT:
        default:
            break;
    }

    painter.setBackgroundColor(bgColor);
    painter.setForegroundColor(TextColor());

    if (textSize == 1)
    {

        View::painter.drawFillRect(this->viewRect.X(), viewRect.Y(), viewRect.Width(), viewRect.Height(), true);

        int cnt = 0;
        char c = text[cnt];

        View::painter.setTextSize(TextSize());


        while (c != '\0' && c != '\n' && offset.X()+TextSize()*5 < viewRect.X2()) {
            View::painter.drawChar(offset.X(), offset.Y(), c);
            offset.appendX(TextSize()*5+TextSize()-1);
            c = text[++cnt];
        }
    }
    else if (currentFont != 0)
    {
        if (!incrementalRepaint || prevText.Length() == 0 || prevText.Length() != text.Length())
        {
            View::painter.drawFillRect(this->viewRect.X(), viewRect.Y(), viewRect.Width(), viewRect.Height(), true);

            geo::Rect textRect(offset.X(),offset.Y(),   viewRect.X2()-offset.X(), viewRect.Y2()-offset.Y());

            display::TextRender tr(painter);
            tr.drawInRect(textRect, text, *currentFont);
        }
        else
        {
            display::TextRender tr(painter);

            uint32_t glyphIdx = 0;
            uint32_t glyphWidth = this->currentFont->glyphWidth;
            uint32_t glyphHeight = this->currentFont->glyphHeight;
            uint32_t newlines = 0;
            uint32_t lineIdx = 0;
            while(glyphIdx < text.Length() && glyphIdx < prevText.Length())
            {
                if (text[glyphIdx] == '\n')
                {
                    newlines++;
                    lineIdx = 0;
                }

                if (text[glyphIdx] != prevText[glyphIdx])
                {
                    uint32_t xOff = offset.X()+lineIdx*glyphWidth;
                    uint32_t yOff = offset.Y()+newlines*glyphHeight;
                    geo::Rect glypsRct(xOff, yOff, viewRect.X2()-xOff, viewRect.Y2()-yOff);

                    painter.drawFillRect(glypsRct, true);

                    tr.drawInRect(glypsRct, String::Format("%c",text[glyphIdx]), *currentFont);
                }

                glyphIdx++;
                lineIdx++;
            }

            if (glyphIdx < text.Length())
            {
                uint32_t xOff = offset.X()+lineIdx*glyphWidth;
                uint32_t yOff = offset.Y()+newlines*glyphHeight;
                geo::Rect txtRct(xOff, yOff, viewRect.X2() - xOff, viewRect.Y2() - yOff);

                String remainder(text()+glyphIdx);
                tr.drawInRect(txtRct, remainder, *currentFont);
            }
        }

        // previous text is always the last painted text
        this->prevText = this->text;
    }
}
