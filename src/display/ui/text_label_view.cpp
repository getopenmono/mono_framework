// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "text_label_view.h"
#include <string.h>
#define PROGMEM
#include <Fonts/FreeSans9pt7b.h>
#include <text_render.h>
#include <mbed_debug.h>

using namespace mono::ui;

__attribute__((weak)) const MonoFont *TextLabelView::StandardTextFont = 0;
__attribute__((weak)) const GFXfont *TextLabelView::StandardGfxFont = &FreeSans9pt7b;

TextLabelView::TextLabelView(String txt) :
    textColor(StandardTextColor),
    bgColor(StandardBackgroundColor)
{
    this->incrementalRepaint = false;
    currentFont = TextLabelView::StandardTextFont;
    this->text = txt;
    this->textMultiline = isTextMultiline();
    this->setTextSize(2);

    viewRect.setSize( geo::Size(TextPixelWidth(), TextPixelHeight()) );
    prevTextRct = viewRect;
    currentGfxFont = StandardGfxFont;

    alignment = ALIGN_LEFT;
    vAlignment = ALIGN_MIDDLE;

    incrementTextRender = 0;
    incrementCharOffset = 0;
    incrementCharPosition = 0;
}

TextLabelView::TextLabelView(const char *txt) :
    textColor(StandardTextColor),
    bgColor(StandardBackgroundColor)
{
    this->incrementalRepaint = false;
    currentFont = TextLabelView::StandardTextFont;
    this->text = txt;
    this->textMultiline = isTextMultiline();
    this->setTextSize(2);

    viewRect.setSize( geo::Size(TextPixelWidth(), TextPixelHeight()) );
    prevTextRct = viewRect;
    currentGfxFont = StandardGfxFont;

    alignment = ALIGN_LEFT;
    vAlignment = ALIGN_MIDDLE;

    incrementTextRender = 0;
    incrementCharOffset = 0;
    incrementCharPosition = 0;
}

TextLabelView::TextLabelView(geo::Rect rct, String txt) :
    View(rct),
    textColor(StandardTextColor),
    bgColor(StandardBackgroundColor)
{
    this->incrementalRepaint = false;
    currentFont = TextLabelView::StandardTextFont;
    this->text = txt;
    this->textMultiline = isTextMultiline();
    this->setTextSize(2);
    prevTextRct = viewRect;
    currentGfxFont = StandardGfxFont;

    alignment = ALIGN_LEFT;
    vAlignment = ALIGN_MIDDLE;

    incrementTextRender = 0;
    incrementCharOffset = 0;
    incrementCharPosition = 0;
}

TextLabelView::TextLabelView(geo::Rect rct, const char *txt) :
    View(rct),
    textColor(StandardTextColor),
    bgColor(StandardBackgroundColor)
{
    this->incrementalRepaint = false;
    currentFont = TextLabelView::StandardTextFont;
    this->text = txt;
    this->textMultiline = isTextMultiline();
    this->setTextSize(2);
    prevTextRct = viewRect;
    currentGfxFont = StandardGfxFont;

    alignment = ALIGN_LEFT;
    vAlignment = ALIGN_MIDDLE;

    incrementTextRender = 0;
    incrementCharOffset = 0;
    incrementCharPosition = 0;
}

/// MARK: Getters

uint8_t TextLabelView::TextSize() const
{
    debug("TextLabelView::TextSuze is Deprecated!\r\n");
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

TextLabelView::VerticalTextAlignment TextLabelView::VerticalAlignment() const
{
    return vAlignment;
}

uint16_t TextLabelView::TextPixelWidth() const
{
    if (textSize == 1)
        return text.Length()*5*TextSize()+TextSize()+1;
    else
    {
        display::TextRender tr(painter);
        if (currentFont)
            return tr.renderDimension(text, *currentFont).Width();
        else if (currentGfxFont)
            return tr.renderDimension(text, *currentGfxFont, textMultiline).Width();
        else
            return 0;
    }
}

uint16_t TextLabelView::TextPixelHeight() const
{
    if (textSize == 1)
        return 7*TextSize()+1;
    else
    {
        display::TextRender tr(painter);
        if (currentFont)
            return tr.renderDimension(text, *currentFont).Height();
        else if (currentGfxFont)
            return tr.renderDimension(text, *currentGfxFont, textMultiline).Height();
        else
            return 0;
    }
}

const MonoFont* TextLabelView::Font() const
{
    debug("TextLabelView::Font() is deprecated. Use GfxFont()!\r\n");
    return currentFont;
}

const GFXfont* TextLabelView::GfxFont() const
{
    return currentGfxFont;
}

mono::geo::Rect TextLabelView::TextDimension() const
{
    display::TextRender tr(painter);
    tr.setAlignment((display::TextRender::HorizontalAlignment) alignment);
    tr.setAlignment((display::TextRender::VerticalAlignmentType) vAlignment);
    
    if (currentFont)
        return geo::Rect(geo::Point(0,0), tr.renderDimension(text, *currentFont));
    else if (currentGfxFont)
        return tr.renderInRect(viewRect, text, *currentGfxFont, textMultiline);
    else
        return geo::Rect();
}

/// MARK: Setters
void TextLabelView::setTextSize(uint8_t newSize)
{
    debug("TextLabelView::setSize is deprecated!\r\n");
    textSize = newSize;
}

void TextLabelView::setTextColor(display::Color col)
{
    debug("TextLabelView::setTextColor is deprecated! Use setText(Color)\r\n");
    setText(col);
}

void TextLabelView::setBackgroundColor(display::Color col)
{
    debug("TextLabelView::setBackgroundColor is deprecated! Use setBackground(Color)\r\n");
    setBackground(col);
}

void TextLabelView::setText(display::Color col)
{
    textColor = col;
    incrementalRepaint = false;
}

/** Set the color behind the text */
void TextLabelView::setBackground(display::Color col)
{
    bgColor = col;
}

void TextLabelView::setAlignment(TextAlignment align)
{
    alignment = align;
    incrementalRepaint = false;
}

void TextLabelView::setAlignment(VerticalTextAlignment vAlign)
{
    vAlignment = vAlign;
    incrementalRepaint = false;
}

void TextLabelView::setText(const char *text)
{
    this->setText(text, false);
}


void TextLabelView::setText(mono::String text)
{
    this->setText(text, false);
}

void TextLabelView::setText(const char *txt, bool resizeViewWidth)
{
    this->setText(String(txt), resizeViewWidth);
}

void TextLabelView::setText(mono::String text, bool resizeViewWidth)
{
    this->text = String(text);
    this->textMultiline = isTextMultiline();

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
    debug("TextLabelView::setFont(): MonoFont's are deprecated, use GfxFonts!\r\n");
    currentFont = &newFont;
    currentGfxFont = 0;
    incrementalRepaint = false;
    
    scheduleRepaint();
}

void TextLabelView::setFont(GFXfont const &font)
{
    currentGfxFont = &font;
    currentFont = 0;
    incrementalRepaint = false;
    
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
    
    geo::Rect txtRct;

    painter.setBackgroundColor(bgColor);
    painter.setForegroundColor(TextColor());

    if (canUseIncrementalRepaint())
    {
        if (currentGfxFont)
            repaintGfxIncremental(txtRct);
        else if (currentFont)
            repaintLegacyIncremental(txtRct);
    }
    else
    {
        if (textSize == 1 || currentFont)
            repaintLegacy(txtRct);
        else if (currentGfxFont)
            repaintGfx(txtRct);
    }

    // previous text is always the last painted text
    this->prevText = this->text;
}

bool TextLabelView::canUseIncrementalRepaint() const
{
    // old text render does not use incrementals
    if (textSize == 1)
        return false;
    // centered proportial fonts cannot incremental repaint
    if (currentGfxFont && alignment == ALIGN_CENTER && text != prevText)
        return false;
    if (currentGfxFont && alignment == ALIGN_RIGHT && text != prevText)
        return false;
    if (prevText.Length() == 0)
        return false;
    if (prevText.Length() != text.Length())
        return false;

    return incrementalRepaint;
}

void TextLabelView::repaintGfx(geo::Rect &)
{
    display::TextRender tr(painter);
    tr.setAlignment((display::TextRender::HorizontalAlignment)alignment);
    tr.setAlignment((display::TextRender::VerticalAlignmentType) vAlignment);

    if (prevText != text)
        painter.drawFillRect(prevTextRct, true);

    tr.drawInRect(viewRect, text, *currentGfxFont, textMultiline);
    prevTextRct = TextDimension();
}

void TextLabelView::repaintLegacy(geo::Rect &)
{
    if (textSize == 1)
    {
        mono::geo::Point offset = this->viewRect.Point();
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
    else
    {
        display::TextRender tr(painter);
        tr.setAlignment((display::TextRender::HorizontalAlignment)alignment);
        tr.setAlignment((display::TextRender::VerticalAlignmentType) vAlignment);

        if (prevText != text)
            painter.drawFillRect(prevTextRct, true);

        tr.drawInRect(viewRect, text, *currentFont);
    }
}

void TextLabelView::drawIncrementalChar(const geo::Point &position, const GFXfont &font, const GFXglyph *gfxGlyph, const geo::Rect &boundingRect, const int lineHeight)
{
    // skip draw - since char is already present on screen
    if (incrementCharPosition < incrementCharOffset) {
        incrementCharPosition++;
        return;
    }

    //draw the character
    if (incrementTextRender)
        incrementTextRender->drawChar(position, font, gfxGlyph, boundingRect, lineHeight);

    incrementCharPosition++;
}

void TextLabelView::repaintGfxIncremental(mono::geo::Rect &)
{
    display::TextRender tr(painter);
    tr.setAlignment((display::TextRender::HorizontalAlignment)alignment);
    tr.setAlignment((display::TextRender::VerticalAlignmentType) vAlignment);

    uint32_t charIdx = 0;

    while(charIdx < text.Length() && charIdx < prevText.Length())
    {
        // is current char different - repaint
        if (text[charIdx] != prevText[charIdx])
        {
            incrementCharPosition = 0;
            incrementCharOffset = charIdx;
            tr.setForeground(bgColor);
            incrementTextRender = &tr;
            tr.layoutInRect<TextLabelView>(viewRect, prevText, *currentGfxFont, this, &TextLabelView::drawIncrementalChar, textMultiline);

            tr.setForeground(textColor);
            incrementCharPosition = 0;
            tr.layoutInRect(viewRect, text, *currentGfxFont, this, &TextLabelView::drawIncrementalChar, textMultiline);

            return; // no now - every following chars are repainted
        }

        charIdx++;
    }
}

void TextLabelView::repaintLegacyIncremental(geo::Rect &txtRct)
{
    display::TextRender tr(painter);

    uint32_t charIdx = 0;
    uint32_t glyphXOffset = 0;
    uint32_t glyphWidth = currentFont->glyphWidth;
    uint32_t glyphHeight = currentFont->glyphHeight;

    uint32_t newlines = 0;
    uint32_t lineIdx = 0;

    while(charIdx < text.Length() && charIdx < prevText.Length())
    {
        if (text[charIdx] == '\n')
        {
            newlines++;
            lineIdx = 0;
        }

        if (text[charIdx] != prevText[charIdx])
        {
            uint32_t xOff = viewRect.X() + glyphXOffset;
            uint32_t yOff = viewRect.Y()+newlines*glyphHeight;

            painter.drawFillRect(xOff, yOff, glyphWidth, glyphHeight, true);
        }

        glyphXOffset += glyphWidth;
        charIdx++;
        lineIdx++;
    }

    if (charIdx < text.Length())
    {
        uint32_t xOff = viewRect.X()+lineIdx*glyphWidth;
        uint32_t yOff = viewRect.Y()+newlines*glyphHeight;
        txtRct = geo::Rect(xOff, yOff, viewRect.X2() - xOff, viewRect.Y2() - yOff);

        String remainder(text()+charIdx);
        tr.drawInRect(txtRct, remainder, *currentFont);
    }

}

bool TextLabelView::isTextMultiline() const
{
    if (text.Length() == 0)
        return false;

    int len = text.Length();
    for (int i=0; i<len; i++) {
        char c = text[i];
        if (c == '\n')
            return true;
    }

    return false;
}
