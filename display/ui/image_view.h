// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef __sdcardTest__image__
#define __sdcardTest__image__

#include "view.h"
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
         * Construct an empty image view, where no image is displayed
         * To display an image you need to call @ref setImage later.
         */
        ImageView();
        
        /**
         * Construct an UI image from an image file
         * 
         * At the moment only BMP images are supported!
         * *Remember to initialize the mbed class `SDFileSystem` object before calling this
         * consructor!*
         */
        ImageView(media::Image *img);
        
        /**
         * Set a new image source object
         * *Note: This method also resets the current cropping rectangle!*
         */
        void setImage(media::Image *img);
        
        void setCrop(geo::Rect crp);
        void setCrop(geo::Size crp);
        void setCrop(geo::Point crp);


        const geo::Rect &Crop() const;

        void repaint();
    };
    
} }

#endif /* defined(__sdcardTest__image__) */
