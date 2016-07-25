// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#include "bmp_image.h"

using namespace mono::media;

BMPImage::BMPImage()
{
    filePath = NULL;
    fPointer = NULL;
    fileOpen = false;
    imageValid = false;
}

BMPImage::BMPImage(const char *path)
{
    filePath = path;
    fPointer = NULL;
    fileOpen = true;
    imageValid = false;
    fPointer = fopen(filePath, "rb");

    if (fPointer == NULL)
    {
        printf("BMPImage: No such file: %s\r\n",path);
        return;
    }

    readHeaderData();

    if (fileHeader.bfType != 0x4D42)
    {
        imageValid = false;
        printf("BMPIMage: File %s is not a BMP image!\r\n", path);
    }
    else
    {
        imageValid = true;
    }

    Close();
}

int BMPImage::ReadPixelData(void *target, int bytesToRead)
{
    if (filePath == NULL)
        return -1;

    if (!fileOpen)
    {
        fPointer = fopen(filePath, "rb");
        fileOpen = true;
    }

    size_t bytesRead = fread(target, bytesToRead*2, 1, fPointer);
    return (int)(bytesRead / PixelByteSize());
}

int BMPImage::SkipPixelData(int pixelsToSkip)
{
    if (filePath == NULL)
        return -1;

    if (!fileOpen)
    {
        fPointer = fopen(filePath, "rb");
        fileOpen = true;
    }

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
    if (filePath == NULL)
        return;

    if (!fileOpen)
    {
        fPointer = fopen(filePath, "rb");
        fileOpen = true;
    }

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
    if (filePath == NULL || fPointer == NULL)
        return;

    fread(&fileHeader, sizeof(struct BMPImage::BmpFileHeader), 1, fPointer);
    fread(&infoHeader, sizeof(struct BMPImage::BmpInfoHeader), 1, fPointer);
    widthMult4 = ((PixelByteSize()*8*infoHeader.biWidth+31)/32)*PixelByteSize();
}

BMPImage::~BMPImage()
{
    if (fPointer != NULL)
        Close();
}