// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#include "bmp_image.h"
#include <mbed_debug.h>
#include <errno.h>

using namespace mono::media;

BMPImage::BMPImage()
{
    filePath = NULL;
    fPointer = NULL;
    imageValid = false;
}

// MARK: Rule of 3

BMPImage::BMPImage(String path)
{
    filePath = path;
    fPointer = NULL;
    imageValid = false;
    fPointer = fopen(filePath(), "rb");

    if (!fPointer)
    {
        debug("BMPImage: No such file: %s\r\n",path());
        return;
    }

    readHeaderData();

    if (fileHeader.bfType != 0x4D42)
    {
        debug("BMPIMage: File %s is not a BMP image!\r\n", path());
    }
    else
    {
        imageValid = true;
    }
}

BMPImage::BMPImage(BMPImage const &img)
{
    filePath = img.filePath;
    imageValid = img.imageValid;
    fileHeader = img.fileHeader;
    infoHeader = img.infoHeader;
    widthMult4 = img.widthMult4;

    fPointer = fopen(img.filePath(), "rb");
    fpos_t pos;
    fgetpos(img.fPointer, &pos);
    fseek(fPointer, pos, SEEK_SET);
}


BMPImage& BMPImage::operator=(const BMPImage &img)
{
    filePath = img.filePath;
    imageValid = img.imageValid;
    fileHeader = img.fileHeader;
    infoHeader = img.infoHeader;
    widthMult4 = img.widthMult4;

    if (fPointer != NULL)
        fclose(fPointer);

    fPointer = fopen(img.filePath(), "rb");
    fpos_t pos;
    fgetpos(img.fPointer, &pos);
    fseek(fPointer, pos, SEEK_SET);

    return *this;
}

int BMPImage::ReadPixelData(void *target, int bytesToRead)
{
    if (!IsValid())
        return -1;

    size_t bytesRead = fread(target, PixelByteSize(), bytesToRead, fPointer);
    return (int)(bytesRead);
}

int BMPImage::SkipPixelData(int pixelsToSkip)
{
    if (!IsValid())
        return -1;
    return fseek(fPointer, pixelsToSkip*2, SEEK_CUR) == 0 ? pixelsToSkip : 0;
}

int BMPImage::Width()
{
    return infoHeader.biWidth;
}
int BMPImage::Height()
{
    return infoHeader.biHeight < 0 ? infoHeader.biHeight*(-1) : infoHeader.biHeight;
}

int BMPImage::PixelByteSize()
{
    return infoHeader.biBitCount >> 3; // divide by 8
}

void BMPImage::SeekToHLine(int vertPos)
{
    if (!IsValid())
        return;
    if (infoHeader.biHeight < 0)
    {
        size_t pos = fileHeader.bfOffBits+(vertPos*widthMult4)*PixelByteSize();
        fseek(fPointer, pos, SEEK_SET);
    }
    else
    {
        size_t pos = fileHeader.bfOffBits+((Height()-vertPos-1)*widthMult4)*PixelByteSize();
        fseek(fPointer, pos, SEEK_SET);
    }
}

void BMPImage::readHeaderData()
{
    fread(&fileHeader, sizeof(struct BMPImage::BmpFileHeader), 1, fPointer);
    fread(&infoHeader, sizeof(struct BMPImage::BmpInfoHeader), 1, fPointer);
    widthMult4 = ((PixelByteSize()*8*infoHeader.biWidth+31)/32)*PixelByteSize();
}

bool BMPImage::IsValid ()
{
    return imageValid;
}

BMPImage::~BMPImage()
{
    if (fPointer != NULL)
    {
        fclose(fPointer);
    }
}
