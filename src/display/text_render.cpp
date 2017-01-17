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
}

TextRender::TextRender(IDisplayController *displayCtrl, Color foreground, Color background)
{
    this->dispCtrl = displayCtrl;
    foregroundColor = foreground;
    backgroundColor = background;
}

TextRender::TextRender(const DisplayPainter &painter)
{
    dispCtrl = painter.DisplayController();
    foregroundColor = painter.ForegroundColor();
    backgroundColor = painter.BackgroundColor();
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
            offset.setX(rect.X());
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

void TextRender::drawInRect(geo::Rect rect, String text, const GFXfont &fontFace)
{
    if (dispCtrl == 0)
        return;
    if (text.Length() == 0)
        return;
    
    dispCtrl->setWindow(rect.X(), rect.Y(), rect.Width(), rect.Height());
    int cnt = 0;
    char c = text[cnt];
    mono::geo::Point offset = rect.Point();
    GFXglyph *glyph = &fontFace.glyph[c - fontFace.first];
    
    if (glyph->xOffset < 0)
        offset.setX(offset.X()+glyph->xOffset);
    
    while (c != '\0' && offset.X()+glyph->width < rect.X2())
    {
        if (c == '\n')
        {
            offset.appendY(fontFace.yAdvance);
            offset.setX(rect.X());
        }
        else if (c == ' ')
        {
            offset.appendX(glyph->xAdvance);
        }
        else
        {
            this->drawChar(offset, fontFace, glyph, rect);
            offset.appendX(glyph->xAdvance);
        }
        
        c = text[++cnt];
        glyph = &fontFace.glyph[c - fontFace.first];
    }
}

void TextRender::drawChar(geo::Point position, const GFXfont &gfxFont, const GFXglyph *glyph, geo::Rect const &bounds)
{
    uint8_t  *bitmap = (uint8_t *)gfxFont.bitmap;
    
    uint16_t bo = glyph->bitmapOffset;
    uint8_t  w  = glyph->width,
             h  = glyph->height;
    int8_t   xo = glyph->xOffset,
             yo = glyph->yOffset;
    
    mono::geo::Rect glyphBounds(position.X() + xo,
                                position.Y() + glyph->height + yo,
                                w, h);
    
    if (!bounds.contains(glyphBounds, true))
        return;
    
    uint8_t  xx, yy, bits, bit = 0;
    bool rePosCursor = false;
    
    for(yy=0; yy<h; yy++) {
        
        dispCtrl->setCursor(position.X() + xo, position.Y() + yy + (gfxFont.yAdvance + yo));
        
        for(xx=0; xx<w; xx++) {
            if(!(bit++ & 7)) {
                bits = bitmap[bo++];
            }
            if(bits & 0x80) {
                //writePixel(255);
                //dispCtrl->write(foregroundColor);
                if (rePosCursor)
                    dispCtrl->setCursor(position.X() + xo + xx,
                                        position.Y() + yy + (gfxFont.yAdvance + yo));
                dispCtrl->write(foregroundColor);
                // (x+xo+xx, y+yo+yy, color);
            }
            else
                rePosCursor = true;
            
            
            bits <<= 1;
        }
    }
}

mono::geo::Size TextRender::renderDimension(String text, const GFXfont &fontFace, bool characterHeight)
{
    char *ptr = text.stringData;
    int newLines = 1;
    int currentLine = 0, longestLine = 0;
    int lastXOffset = 0;
    int mostUnderBaseLine = 0;
    int highestPoint = fontFace.yAdvance;
    int lowestPoint = 0;

    while (*ptr != '\0') {

        if (*ptr == '\n')
        {
            newLines++;
            mostUnderBaseLine = 0; // theres a new line, discard prev. lines undershoot
            currentLine = 0;
        }

        GFXglyph glyph = fontFace.glyph[*ptr - fontFace.first];
        currentLine += glyph.xAdvance;
        lastXOffset = glyph.xOffset;

        if (highestPoint > -glyph.yOffset)
            highestPoint = -glyph.yOffset;

        if (lowestPoint < -glyph.yOffset + glyph.height)
            lowestPoint = -glyph.yOffset + glyph.height;

        if (glyph.height + glyph.yOffset > mostUnderBaseLine)
            mostUnderBaseLine = glyph.height + glyph.yOffset;
        
        if (currentLine > longestLine)
            longestLine = currentLine;

        ptr++;
    }

    if (lastXOffset < 0)
        lastXOffset = -lastXOffset;

    int height;
    if (characterHeight)
        height = lowestPoint - highestPoint;
    else
        height = newLines*fontFace.yAdvance+mostUnderBaseLine;

    geo::Size dimensions(longestLine + lastXOffset, height);
    return dimensions;
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

/// MARK: Accessors

void TextRender::setForeground(Color fg)
{
    foregroundColor = fg;
}

void TextRender::setBackground(Color bg)
{
    backgroundColor = bg;
}
