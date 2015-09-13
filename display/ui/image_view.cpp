//
//  image.cpp
//  sdcardTest
//
//  Created by Kristoffer Andersen on 11/09/15.
//  Copyright (c) 2015 Monolit ApS. All rights reserved.
//

#include "image_view.h"
#include <consoles.h>

using namespace mono::ui;

ImageView::ImageView(media::Image *img) : crop(0,0, img->Width(), img->Height())
{
    image = img;
}

void ImageView::setCrop(geo::Rect crp)
{
    crop = crp;
}

void ImageView::setCrop(geo::Size crp)
{
    crop.setSize(crp);
}

void ImageView::setCrop(geo::Point crp)
{
    crop.setPoint(crp);
}

void ImageView::repaint()
{
    int h = viewRect.Height() < crop.Height() ? viewRect.Height() : crop.Height();
    int w = viewRect.Width() < crop.Width() ? viewRect.Width() : crop.Width();
    for(int16_t y=crop.Y(); y<h; y++)
    {
        image->SeekToHLine(y);
        
        if (crop.X() > 0)
            image->SkipPixelData(crop.X());
        
        for (int16_t x=crop.X(); x<w; x++) {
            
            uint16_t pixel;
            image->ReadPixelData(&pixel, 1);
            View::painter.setForegroundColor(pixel);
            View::painter.drawPixel(viewRect.X()+x, viewRect.Y()+y);
        }
    }
}