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
#include "../../media/image.h"

namespace mono { namespace ui {
    
    /**
     * 
     *
     *
     * @brief <# brief desc #>
     */
    class ImageView : public View
    {
    protected:
        /**
         * A pointer to the @ref Image object that is to be displayed in the 
         * screen.
         */
        media::Image *image;
        
        /**
         * Image crop rectangle. Setting this rect will crop the source image
         * (non destructive).
         */
        geo::Rect crop;
        
    public:
        /**
         * Construct an UI image from an image file
         * 
         * At the moment only BMP images are supported!
         * *Remember to initialize the SDFileSystem object before calling this
         * consructor!*
         */
        ImageView(media::Image *img);
        
        void setCrop(geo::Rect crp);
        void setCrop(geo::Size crp);
        void setCrop(geo::Point crp);
        
        void repaint();
    };
    
} }

#endif /* defined(__sdcardTest__image__) */
