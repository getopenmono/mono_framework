// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

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

    viewRect.setSize(crop);
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

    //copy the image crop/select rect
    geo::Rect dispRect = crop;
    //zero the offset
    dispRect.setPoint(geo::Point(0,0));
    //crop by viewRect
    dispRect = dispRect.crop(viewRect);

    display::IDisplayController *ctrl = View::painter.DisplayController();
    uint16_t pixels[176];
    ctrl->setWindow(viewRect.X(), viewRect.Y(), dispRect.Width(), dispRect.Height());

    int iy = 0;

    for(int16_t y=crop.Y(); y<crop.Y()+dispRect.Height(); y++)
    {
        image->SeekToHLine(y);

        if (crop.X() > 0)
            image->SkipPixelData(crop.X());

        image->ReadPixelData(pixels, dispRect.Width());

        for (int16_t x=0; x<dispRect.Width(); x++) {
                ctrl->write(pixels[x]);
        }

        iy++;
    }
}