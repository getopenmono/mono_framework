// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#include "text_render.h"
#include <view.h>

using namespace mono::display;

TextRender::TextRender(IDisplayController *displayCtrl)
{
    this->dispCtrl = displayCtrl;
    foregroundColor = ui::View::StandardTextColor;
    backgroundColor = ui::View::StandardBackgroundColor;
    hAlignment = ALIGN_LEFT;
    vAlignment = ALIGN_MIDDLE;
}

TextRender::TextRender(IDisplayController *displayCtrl, Color foreground, Color background)
{
    this->dispCtrl = displayCtrl;
    foregroundColor = foreground;
    backgroundColor = background;
    hAlignment = ALIGN_LEFT;
    vAlignment = ALIGN_MIDDLE;
}

TextRender::TextRender(const DisplayPainter &painter)
{
    dispCtrl = painter.DisplayController();
    foregroundColor = painter.ForegroundColor();
    backgroundColor = painter.BackgroundColor();
    hAlignment = ALIGN_LEFT;
    vAlignment = ALIGN_MIDDLE;
}

void TextRender::drawInRect(geo::Rect rect, String text, const MonoFont &fontFace)
{
    if (dispCtrl == 0)
        return;

    dispCtrl->setWindow(rect.X(), rect.Y(), rect.Width(), rect.Height());
    int cnt = 0;
    char c = text[cnt];
    mono::geo::Point offset = rect.Point();

    while (c != '\0' && offset.X()+fontFace.glyphWidth <= rect.X2())
    {
        if (c == '\n')
        {
            offset.appendY(fontFace.glyphHeight);
            switch (hAlignment) {
                case ALIGN_LEFT:
                    offset.setX(rect.X());
                    break;
                case ALIGN_CENTER:
                {
                    int remainder = text.Length() - cnt+1;
                    if (rect.Width() > remainder*fontFace.glyphWidth)
                        offset.setX( rect.X() + (rect.Width() - remainder*fontFace.glyphWidth) / 2 );
                    break;
                }
                case ALIGN_RIGHT:
                {
                    int remainder = text.Length() - cnt+1;
                    if (rect.Width() > remainder*fontFace.glyphWidth)
                        offset.setX( rect.X() + (rect.Width() - remainder*fontFace.glyphWidth) );
                    break;
                }
                default:
                    break;
            }
        }
        else if (c == ' ')
        {
            offset.appendX(fontFace.glyphWidth - (fontFace.glyphWidth >> 2));
        }
        else
        {
            this->drawChar(offset, c, fontFace, rect);
            offset.appendX(fontFace.glyphWidth);
        }

        c = text[++cnt];
    }
}

mono::geo::Size TextRender::renderDimension(String text, const MonoFont &fontFace)
{
    char *ptr = text.stringData;
    int newLines = 1;
    int currentLine = 0, longestLine = 0;

    while (*ptr != '\0') {

        if (*ptr == '\n')
        {
            newLines++;
            currentLine = 0;
        }
        currentLine++;

        if (currentLine > longestLine)
            longestLine = currentLine;

        ptr++;
    }
    geo::Size dimensions(longestLine*fontFace.glyphWidth,newLines*fontFace.glyphHeight);
    return dimensions;
}

void TextRender::drawChar(geo::Point position, char character, const MonoFont &font, geo::Rect const &boundingRect)
{
    int glyphHeight = font.glyphHeight;
    int glyphWidth = font.glyphWidth;
    int totalWidth = font.bitmapWidth;

    if((position.X() >= boundingRect.X2())    || // Clip right
       (position.Y() >= boundingRect.Y2())   || // Clip bottom
       ((position.X() + glyphWidth - 1) < 0)         || // Clip left
       ((position.Y() + glyphHeight - 1) < 0))           // Clip top
        return;

    uint8_t maxPxVal = font.bitrate*font.bitrate-1;
    uint8_t pxScale = 255/maxPxVal;
    uint8_t pixelPerByte = 8/font.bitrate;
    geo::Rect bRct = geo::Rect(boundingRect.X(), boundingRect.Y(), boundingRect.Width(), boundingRect.Height()-1);

    for (int yi=0; yi<glyphHeight; yi++) {

        uint16_t lineOffset = (character-font.characterOffset)*glyphWidth+yi*totalWidth;

        geo::Point pos = geo::Point(position.X(),position.Y()+yi);
        if (!bRct.contains(pos))
            break;

        dispCtrl->setCursor(position.X(), position.Y()+yi);

        for (int xi=0; xi<glyphWidth; xi++) {

            uint32_t pixelOffset = lineOffset+xi;
            uint32_t byteOffset = pixelOffset / pixelPerByte;
            uint8_t shiftOffset = (pixelPerByte - 1) - (pixelOffset % pixelPerByte);
            uint8_t rawByte = font.bitmap[byteOffset];
            uint8_t twoBitIntensity = (rawByte >> (shiftOffset*font.bitrate)) & (font.bitrate == 4 ? 0xF : 0x3);

            uint16_t intensity = twoBitIntensity * pxScale;

            writePixel((uint8_t)(255-intensity));
        }
    }
}

void TextRender::drawInRect(const geo::Rect &rect, String text, const GFXfont &fontFace, bool lineLayout)
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

    int lineHeight;
    if (lineLayout)
        lineHeight = fontFace.yAdvance;
    else
        lineHeight = dim.Height() - calcUnderBaseline(text, fontFace);
    
    while (c != '\0')
    {
        if (c == '\n')
        {
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
            this->drawChar(offset, fontFace, glyph, rect, lineHeight);
            offset.appendX(glyph->xAdvance);
        }
        
        c = text[++cnt];
        glyph = &fontFace.glyph[c - fontFace.first];
    }
}

void TextRender::drawChar(const geo::Point &position, const GFXfont &gfxFont, const GFXglyph *glyph, geo::Rect const &bounds, int lineHeight)
{
    uint8_t  *bitmap = (uint8_t *)gfxFont.bitmap;
    
    uint16_t bo = glyph->bitmapOffset;
    uint8_t  w  = glyph->width,
             h  = glyph->height;
    int8_t   xo = glyph->xOffset,
             yo = glyph->yOffset;
    
    mono::geo::Rect glyphBounds(position.X() + xo,
                                position.Y() + lineHeight + yo,
                                w, h);
    
    if (!bounds.contains(glyphBounds, true))
        return;
    
    uint8_t  xx, yy, bits, bit = 0;
    bool rePosCursor = false;
    
    for(yy=0; yy<h; yy++) {
        dispCtrl->setCursor(position.X() + xo, position.Y() + yy + lineHeight + glyph->yOffset);
        
        for(xx=0; xx<w; xx++) {
            if(!(bit++ & 7)) {
                bits = bitmap[bo++];
            }
            if(bits & 0x80) {
                //writePixel(255);
                //dispCtrl->write(foregroundColor);
                if (rePosCursor)
                    dispCtrl->setCursor(position.X() + xo + xx,
                                        position.Y() + yy + lineHeight + glyph->yOffset);
                dispCtrl->write(foregroundColor);
                // (x+xo+xx, y+yo+yy, color);
            }
            else
                rePosCursor = true;
            
            
            bits <<= 1;
        }
    }
}

mono::geo::Size TextRender::renderDimension(String text, const GFXfont &fontFace, bool lineLayout)
{
    char *ptr = text.stringData;
    int newLines = 1;
    int currentLine = 0, longestLine = 0;
    int maxOverBase = 0;
    int maxUnderBase = 0;
    int lastAdvanceDiff = 0;
    int bestAdvanceDiff = 0;

    while (*ptr != '\0') {

        if (*ptr == '\n')
        {
            newLines++;
            maxUnderBase = 0; // theres a new line, discard prev. lines undershoot
            currentLine = 0;
            
            if (lastAdvanceDiff > bestAdvanceDiff)
                bestAdvanceDiff = lastAdvanceDiff;
            
            ptr++;
            continue;
        }

        GFXglyph glyph = fontFace.glyph[*ptr - fontFace.first];
        currentLine += glyph.xAdvance;
        lastAdvanceDiff = glyph.xOffset + glyph.width - glyph.xAdvance;

        if (maxOverBase < -glyph.yOffset)
            maxOverBase = -glyph.yOffset;
        if (maxUnderBase < glyph.height + glyph.yOffset)
            maxUnderBase = glyph.height + glyph.yOffset;
        
        if (currentLine + lastAdvanceDiff > longestLine)
            longestLine = currentLine + lastAdvanceDiff;

        ptr++;
    }

    int height, width;
    if (!lineLayout) {
        width = longestLine;
        height = maxOverBase + maxUnderBase;
    }
    else {
        if (lastAdvanceDiff > bestAdvanceDiff)
            bestAdvanceDiff = lastAdvanceDiff;
        
        width = longestLine;
        height = newLines*fontFace.yAdvance+maxUnderBase;
    }
    
    geo::Size dimensions(width, height);
    return dimensions;
}

mono::geo::Rect TextRender::renderInRect(const geo::Rect &rect, mono::String text, const GFXfont &fontFace, bool lineLayout)
{
    char c = text[0];
    geo::Size dim = this->renderDimension(text, fontFace, lineLayout);
    mono::geo::Point offset = rect.Point();
    GFXglyph *glyph = &fontFace.glyph[c - fontFace.first];
    
    if (glyph->xOffset < 0)
        offset.setX(offset.X()+glyph->xOffset);
    
    if (dim.Height() < rect.Height())
    {
        switch (vAlignment) {
            case ALIGN_MIDDLE:
                offset.setY(offset.Y() + (rect.Height() - dim.Height())/2 );
                break;
            case ALIGN_BOTTOM:
                offset.setY(offset.Y() + rect.Height() - dim.Height() );
                break;
            default:
                break;
        }
    }
    
    uint32_t textWidth = remainingTextlineWidth(fontFace, text.stringData);
    if (dim.Width() < rect.Width())
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
    
    return geo::Rect(offset, dim);
}

int TextRender::calcUnderBaseline(mono::String text, const GFXfont &font)
{
    char *ptr = text.stringData;
    int maxUndershoot = 0;
    
    while (*ptr != '\0' && *ptr != '\n') {
        GFXglyph glyph = font.glyph[*ptr - font.first];
        
        if (maxUndershoot < glyph.height + glyph.yOffset)
            maxUndershoot = glyph.height + glyph.yOffset;
        
        ptr++;
    }
    
    return maxUndershoot;
}

void TextRender::writePixel(uint8_t intensity, bool bg)
{
    Color c;
    if (bg)
        c = backgroundColor.alphaBlend(intensity, foregroundColor);
    else
        c = foregroundColor.alphaBlend(intensity, backgroundColor);

    dispCtrl->write(c);
}

uint32_t TextRender::remainingTextlineWidth(const GFXfont &font, const char *text)
{
    uint32_t w = 0;
    int lastAdvanceDiff;

    while (*text != 0 && *text != '\n') {
        GFXglyph glyph = font.glyph[*text - font.first];
        w += glyph.xAdvance;
        lastAdvanceDiff = glyph.xOffset + glyph.width - glyph.xAdvance;
        text++;
    }

    return w + lastAdvanceDiff;
}

/// MARK: Accessors

void TextRender::setForeground(Color fg)
{
    foregroundColor = fg;
}

void TextRender::setBackground(Color bg)
{
    backgroundColor = bg;
}

void TextRender::setAlignment(TextRender::HorizontalAlignment align)
{
    hAlignment = align;
}

void TextRender::setAlignment(TextRender::VerticalAlignmentType vAlign)
{
    vAlignment = vAlign;
}

Color TextRender::Foreground() const
{
    return foregroundColor;
}

Color TextRender::Background() const
{
    return backgroundColor;
}

TextRender::HorizontalAlignment TextRender::Alignment() const
{
    return hAlignment;
}

TextRender::VerticalAlignmentType TextRender::VerticalAlignment() const
{
    return vAlignment;
}
