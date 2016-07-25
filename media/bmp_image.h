// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef __displaySimTest__bmp_image__
#define __displaySimTest__bmp_image__

#include "image.h"
#include <stdint.h>

namespace mono { namespace media {

    class BMPImage : public Image
    {
    protected:

        struct __attribute__ ((__packed__)) BmpFileHeader {
            uint16_t bfType;
            uint32_t bfSize;
            int16_t  bfReserved1;
            int16_t  bfReserved2;
            uint32_t bfOffBits;
        };

        struct __attribute__ ((__packed__)) BmpInfoHeader {
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

        struct BmpFileHeader fileHeader;
        struct BmpInfoHeader infoHeader;

        uint16_t widthMult4;

        void readHeaderData();

        const char *filePath;
        FILE *fPointer;
        bool imageValid;

    public:

        BMPImage();

        BMPImage(const char *path);

        ~BMPImage();

        int ReadPixelData(void *target, int bytesToRead);
        int SkipPixelData(int pixelsToSkip);

        int Width();
        int Height();

        int PixelByteSize();

        void SeekToHLine(int vertPos);

        bool IsValid ();
    };

} }

#endif /* defined(__displaySimTest__bmp_image__) */
