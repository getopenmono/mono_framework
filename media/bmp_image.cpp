//
//  bmp_image.cpp
//  displaySimTest
//
//  Created by Kristoffer Lyder Andersen on 13/09/15.
//
//

#include "bmp_image.h"
#include <mbed.h>

using namespace mono::media;

BMPImage::BMPImage(const char *path)
{
    filePath = path;
    fPointer = NULL;
    fileOpen = true;
    fPointer = fopen(filePath, "rb");
    readHeaderData();
    Close();
}

int BMPImage::ReadPixelData(void *target, int bytesToRead)
{
    
    if (!fileOpen)
    {
        fPointer = fopen(filePath, "rb");
        fileOpen = true;
    }
    
    return fread(target, bytesToRead*2, 1, fPointer);
}

int BMPImage::SkipPixelData(int pixelsToSkip)
{
    if (!fileOpen)
    {
        fPointer = fopen(filePath, "rb");
        fileOpen = true;
    }
    
    return fseek(fPointer, pixelsToSkip*2, SEEK_CUR);
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
    
    if (!fileOpen)
    {
        fPointer = fopen(filePath, "rb");
        fileOpen = true;
    }
    
    if (infoHeader.biHeight < 0)
        fseek(fPointer, fileHeader.bfOffBits+(vertPos*widthMult4)*2, SEEK_SET);
    else
        fseek(fPointer, fileHeader.bfOffBits+((-infoHeader.biHeight-vertPos)*widthMult4)*2, SEEK_SET);
}

void BMPImage::readHeaderData()
{
    fread(&fileHeader, sizeof(struct BMPImage::BmpFileHeader), 1, fPointer);
    fread(&infoHeader, sizeof(struct BMPImage::BmpInfoHeader), 1, fPointer);
    widthMult4 = infoHeader.biWidth + (infoHeader.biWidth%4);
}