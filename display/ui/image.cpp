//
//  image.cpp
//  sdcardTest
//
//  Created by Kristoffer Andersen on 11/09/15.
//  Copyright (c) 2015 Monolit ApS. All rights reserved.
//

#include "image.h"
#include <consoles.h>

using namespace mono::ui;

Image::Image(const char *filePath)
{
    imgFile = fopen(filePath, "r");
    
    fread(&head1, sizeof(struct BMPData::fileHeader), 1, imgFile);
    fread(&head2, sizeof(struct BMPData::infoHeader), 1, imgFile);
    
    mono::defaultSerial.printf("Image %i x %i, depth: %i\n\r",head2.biWidth,head2.biHeight, head2.biBitCount);
    
    if (head1.bfType != 0x4D42) {
        mono::defaultSerial.printf("not a bmp!\n");
        
    }
    
    if (head2.biHeight<0) {
        mono::defaultSerial.printf("top down BMP not supported\n");
        
    }
    
    // crop target, if bigger than source
//    if (sw>head2.biWidth)
//        sw = head2.biWidth;
//    if (sh>head2.biHeight)
//        sh = head2.biHeight;
    
    //row length must a multiple of 4, according to the BMP standard
    widthMult4 = head2.biWidth + (head2.biWidth%4);
    
    //uint8_t colorDepth = head2.biBitCount;
    
    //uint16_t *pixels16 = (uint16_t*)(bmp+head1.bfOffBits);
    //uint8_t *pixels8 = (uint8_t*)(bmp+head1.bfOffBits);
    //fseek(imgFile, head.bfOffBits, SEEK_SET);
    
}

void Image::repaint()
{
    for(int16_t y=0; y<this->size.Height(); y++)
    {
        if (head2.biHeight < 0)
            fseek(imgFile, head1.bfOffBits+((y)*widthMult4)*2, SEEK_SET);
        else
            fseek(imgFile, head1.bfOffBits+((-head2.biHeight-y)*widthMult4)*2, SEEK_SET);
        
        for (int16_t x=0; x<this->size.Width(); x++) {
            
            uint16_t pixel;
            fread(&pixel, 2, 1, imgFile);
            View::painter.setForegroundColor(pixel);
            View::painter.drawPixel(position.X()+x, position.Y()+y);
            //            uint16_t color;
            //            if (colorDepth == 16)
            //                color = *(pixels16 + (sy+sh-y-1)*w4 + sx+x );
            //            else if (colorDepth == 8) {
            //                color = *(pixels8 + (sy+sh-y-1)*w4 + sx+x );
            //                color = (color & 0xE0) << 8 | (color & 0x1C) << 6 | (color & 0x3) << 3;
            //            }
            //            else {
            //                println("Color depth not supported");
            //                return;
            //            }
            
            
        }
    }
}