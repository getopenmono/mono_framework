//
//  image.h
//  sdcardTest
//
//  Created by Kristoffer Andersen on 11/09/15.
//  Copyright (c) 2015 Monolit ApS. All rights reserved.
//

#ifndef __sdcardTest__image__
#define __sdcardTest__image__

#include "view.h"
#include <FileSystemLike.h>

namespace mono { namespace ui {
    
    
    class BMPData
    {
    public:
        struct __attribute__ ((__packed__)) fileHeader {
            uint16_t bfType;
            uint32_t bfSize;
            int16_t  bfReserved1;
            int16_t  bfReserved2;
            uint32_t bfOffBits;
        };
        
        struct __attribute__ ((__packed__)) infoHeader {
            uint32_t biSize;
            int32_t  biWidth;
            int32_t  biHeight;
            int16_t  biPlanes;
            int16_t  biBitCount;
            int32_t  biCompression;
            int32_t  biSizeImage;
            int32_t  biXPelsPerMeter;
            int32_t  biYPelsPerMeter;
            int32_t  biClrUsed;
            int32_t  biClrImportant;
        };
        
    };
    
    class Image : public View
    {
    protected:
        FILE *imgFile;
        struct BMPData::fileHeader head1;
        struct BMPData::infoHeader head2;
        uint16_t widthMult4;
        
    public:
        /**
         * Construct an UI image from an image file
         * 
         * At the moment only BMP images are supported!
         * *Remember to initialize the SDFileSystem object before calling this
         * consructor!*
         */
        Image(const char *filePath);
        
        
        void repaint();
    };
    
} }

#endif /* defined(__sdcardTest__image__) */
