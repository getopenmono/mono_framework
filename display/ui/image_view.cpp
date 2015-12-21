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

ImageView::ImageView() : crop(0,0,0,0)
{
    image = NULL;
}

ImageView::ImageView(media::Image *img) : crop(0,0, img->Width(), img->Height())
{
    image = img;
    
    //crop within display canvas
    if (img->Width() > View::painter.CanvasWidth())
        crop.setWidth(View::painter.CanvasWidth());
    if (img->Height() > View::painter.CanvasHeight())
        crop.setHeight(View::painter.CanvasHeight());
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

void ImageView::setImage(media::Image *img)
{
    image = img;
    setCrop(geo::Rect(0,0,img->Width(),img->Height()));
    scheduleRepaint();
}

void ImageView::repaint()
{
    if (image == NULL)
        return;
    
    int h = viewRect.Height() < crop.Height() ? viewRect.Height() : crop.Height();
    int w = viewRect.Width() < crop.Width() ? viewRect.Width() : crop.Width();
    
    display::IDisplayController *ctrl = View::painter.DisplayController();
    uint16_t pixels[176];
    ctrl->setWindow(viewRect.X(), viewRect.Y(), w, h);
    
    //painter.setOrigin(viewRect.X(), viewRect.Y());
    //painter.setRotation(90);
    //float32_t *matrix = View::painter.CurrentMatrix();
    int iy = 0;
    
    for(int16_t y=crop.Y(); y<h; y++)
    {
        image->SeekToHLine(y);
        
        if (crop.X() > 0)
            image->SkipPixelData(crop.X());
        
        image->ReadPixelData(pixels, w);
        
        for (int16_t x=0; x<w-crop.X(); x++) {
            
            //int16_t x1 = x*matrix[0] + iy*matrix[1] + 1*matrix[2];
            //int16_t y1 = x*matrix[3] + iy*matrix[4] + 1*matrix[5];
            
            //if (x1 == x && iy == y1)
                ctrl->write(pixels[x]);
//            else
//            {
//                painter.setForegroundColor(pixels[x]);
//                painter.drawPixel(viewRect.X()+x, viewRect.Y()+iy);
//            }
            
            //View::painter.setForegroundColor(pixel);
            //View::painter.drawPixel(viewRect.X()+x, viewRect.Y()+y);
        }
        
        iy++;
    }
    
    image->Close();
    // set to identity
    //painter.ResetTransformation();
}