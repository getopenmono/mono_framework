// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "text_label_view.h"
#include <string.h>
#include <ptmono15.h>
#include <text_render.h>

using namespace mono::ui;

__attribute__((weak)) const MonoFont *TextLabelView::StandardTextFont = &mono::display::PT_Mono_15;
__attribute__((weak)) const GFXfont *TextLabelView::StandardGfxFont = 0;

TextLabelView::TextLabelView(String txt) :
    textColor(StandardTextColor),
    bgColor(StandardBackgroundColor)
{
    this->incrementalRepaint = false;
    currentFont = TextLabelView::StandardTextFont;
    this->text = txt;
    this->setTextSize(2);

    viewRect.setSize( geo::Size(TextPixelWidth(), TextPixelHeight()) );
    prevTextRct = viewRect;
    currentGfxFont = StandardGfxFont;
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
    prevTextRct = viewRect;
    currentGfxFont = StandardGfxFont;
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
    prevTextRct = viewRect;
    currentGfxFont = StandardGfxFont;
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
    prevTextRct = viewRect;
    currentGfxFont = StandardGfxFont;
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
        if (currentFont)
            return tr.renderDimension(text, *currentFont).Width();
        else if (currentGfxFont)
            return tr.renderDimension(text, *currentGfxFont).Width();
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
            return tr.renderDimension(text, *currentGfxFont).Height();
        else
            return 0;
    }
}

const MonoFont* TextLabelView::Font() const
{
    return currentFont;
}

const GFXfont* TextLabelView::GfxFont() const
{
    return currentGfxFont;
}

mono::geo::Size TextLabelView::TextDimension() const
{
    display::TextRender tr(painter);
    if (currentFont)
        return tr.renderDimension(text, *currentFont);
    else if (currentGfxFont)
        return tr.renderDimension(text, *currentGfxFont, true);
    else
        return geo::Size();
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
    currentGfxFont = 0;
    
    scheduleRepaint();
}

void TextLabelView::setFont(GFXfont const &font)
{
    currentGfxFont = &font;
    currentFont = 0;
    
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
    geo::Rect txtRct;

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
    else if (currentFont != 0 || currentGfxFont != 0)
    {
        if ((currentGfxFont && alignment == ALIGN_CENTER && text != prevText) ||
            !incrementalRepaint ||
            prevText.Length() == 0 ||
            prevText.Length() != text.Length())
        {
            display::TextRender tr(painter);
            painter.drawFillRect(prevTextRct, true);

            txtRct = geo::Rect(offset.X(),offset.Y(), viewRect.X2()-offset.X(), viewRect.Y2()-offset.Y());

            if (currentGfxFont)
            {
                geo::Size dim = tr.renderDimension(text, *currentGfxFont);
                geo::Rect txtBounds(offset.X(), offset.Y(), dim.Width(), dim.Height());

                //painter.setForegroundColor(display::BlueColor);
                //painter.drawRect(txtBounds);
                //painter.setForegroundColor(textColor);

                tr.drawInRect(txtRct, text, *currentGfxFont);
                prevTextRct = txtBounds;
            }
            else if (currentFont)
            {
                tr.drawInRect(txtRct, text, *currentFont);
            }
        }
        else
        {
            display::TextRender tr(painter);

            uint32_t charIdx = 0;
            uint32_t glyphXOffset = 0;
            uint32_t glyphWidth;
            if (currentFont)
                glyphWidth = currentFont->glyphWidth;
            
            uint32_t glyphHeight;
            
            if (currentFont)
                glyphHeight = currentFont->glyphHeight;
            else if (currentGfxFont)
                glyphHeight = currentGfxFont->yAdvance;
            else
                return;
            
            uint32_t newlines = 0;
            uint32_t lineIdx = 0;

            while(charIdx < text.Length() && charIdx < prevText.Length())
            {
                
                if (text[charIdx] == '\n')
                {
                    newlines++;
                    lineIdx = 0;
                }

                GFXglyph gl;
                if (currentGfxFont)
                {
                    gl = currentGfxFont->glyph[text[charIdx] - currentGfxFont->first];
                    glyphWidth = gl.width;
                }
                
                if (text[charIdx] != prevText[charIdx])
                {
                    uint32_t xOff = offset.X() + glyphXOffset;
                    uint32_t yOff = offset.Y()+newlines*glyphHeight;
                    
                    //uint32_t prevGlyphWidth;
                    if (currentGfxFont)
                    {
                        String remainder(text()+charIdx);
                        String oldRemainder(prevText()+charIdx);
                        
                        geo::Size oldDim = tr.renderDimension(oldRemainder, *currentGfxFont);
                        tr.setForeground(bgColor);
                        tr.drawInRect(geo::Rect(xOff, yOff, oldDim.Width(), oldDim.Height()),
                                      oldRemainder,
                                      *currentGfxFont);

                        tr.setForeground(textColor);
                        txtRct = geo::Rect(xOff, yOff, viewRect.X2() - xOff, viewRect.Y2() - yOff);
                        tr.drawInRect(txtRct, remainder, *currentGfxFont);
                        geo::Size txtBnds = tr.renderDimension(remainder, *currentGfxFont);
                        prevTextRct = geo::Rect(xOff, yOff, txtBnds.Width(), txtBnds.Height());
                        charIdx = text.Length();
                        break;
                        
                        
//                        GFXglyph prevGl = currentGfxFont->glyph[prevText[charIdx] - currentGfxFont->first];
//                        
//                        prevGlyphWidth = prevGl.width + prevGl.xOffset;;
//                        tr.setForeground(bgColor);
//                        tr.drawInRect(geo::Rect(xOff, yOff, prevGlyphWidth, glyphHeight),
//                                      String::Format("%c",prevText[charIdx]),
//                                      *currentGfxFont);
//                        
////                        painter.drawFillRect(xOff+prevGl.xOffset,
////                                             yOff + glyphHeight + prevGl.yOffset,
////                                             prevGlyphWidth, prevGl.height, true);
//                        tr.setForeground(textColor);
//                        glyphWidth += gl.xOffset;
                    }
                    else
                        painter.drawFillRect(xOff, yOff, glyphWidth, glyphHeight, true);
                    
                    geo::Rect glypsRct(xOff, yOff, glyphWidth, glyphHeight);
                    String glyph = String::Format("%c",text[charIdx]);
                    
                    if (currentFont)
                        tr.drawInRect(glypsRct, glyph, *currentFont);
                }

                if (currentGfxFont)
                    glyphXOffset += gl.xAdvance;
                else
                    glyphXOffset += glyphWidth;
                charIdx++;
                lineIdx++;
            }

            if (charIdx < text.Length())
            {
                uint32_t xOff = offset.X()+lineIdx*glyphWidth;
                uint32_t yOff = offset.Y()+newlines*glyphHeight;
                txtRct = geo::Rect(xOff, yOff, viewRect.X2() - xOff, viewRect.Y2() - yOff);

                String remainder(text()+charIdx);
                
                if (currentFont)
                    tr.drawInRect(txtRct, remainder, *currentFont);
                else if (currentGfxFont)
                    tr.drawInRect(txtRct, remainder, *currentGfxFont);
                    
            }
        }

        // previous text is always the last painted text
        this->prevText = this->text;
    }
    
}
