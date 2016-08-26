// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef __sdcardTest__image__
#define __sdcardTest__image__

#include "view.h"
#include "../../media/image.h"

namespace mono { namespace ui {
    
    /**
     * The ImageView can render a bitmap image on the display. It needs a image
     * data source, that delivers the actual bitmap. (See @ref BMPImage)
     *
     * You provide the image data and a bounding rect where the ImageView is
     * painted.
     *
     * If you wish to use the class @ref BMPImage as a image source, you must
     * initialize the SD Card Filesystem first!
     *
     * ## Example
     * @code
     * // init the SD card before accessing the file system
     *
     * // Open and prepare the BMP file to display
     * mono::media::BMPImage img("/sd/my_pic.bmp");
     *
     * // create the view and provide the image source
     * mono::ui::ImageView imgView(&img);
     *
     * // tell the image to be showed
     * imgView.show();
     * @endcode
     *
     * It is your responsibility to make sure the source image data object is 
     * staying around, and do not get deallocated. Preferreably you should make
     * both the image source and the view class members.
     *
     * ## Cropping
     *
     * The image view can crop the source image, thereby only showing a selected
     * portion. The default crop is defined by the views bounding rect. Naturally
     * images larger than the view's rect will be cropped with repect to the
     * upper left corner.
     *
     * The default cropping @ref Rect is therefore *(0, 0, imageWidth, imageHeight)*
     *
     * @brief Displays a bitmap image on the display
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
         *
         * The image @ref viewRect is set to the full screen size. Use 
         * @ref setRect to adjust size and position.
         *
         * @param img The image data slurce to show
         */
        ImageView(media::Image *img);
        
        /**
         * @brief Set a new image source object
         * *Note: This method also resets the current cropping rectangle!*
         *
         * @param img A pointer to the new image to display
         */
        void setImage(media::Image *img);

        /**
         * @brief Set image cropping
         * 
         * Define switch portion for the image should be display inside the view
         * own bounding rectangle. By default as much as the original image as
         * possible, will be showed.
         *
         * By defining a cropping rectangle you can define an offset and size to
         * display from thesource image.
         *
         * The source image has the same coordinate system as the display. That
         * is (0,0) is the upper left corner.
         *
         * @param crp A cropping rectangle
         */
        void setCrop(geo::Rect crp);
        void setCrop(geo::Size crp);
        void setCrop(geo::Point crp);

        /**
         * @brief Get the current cropping rectangle
         * Get the current used cropping rectangle for the source image
         */
        const geo::Rect &Crop() const;

        void repaint();
    };
    
} }

#endif /* defined(__sdcardTest__image__) */
