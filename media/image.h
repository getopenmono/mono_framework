// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef displaySimTest_image_h
#define displaySimTest_image_h

#include <stdio.h>

namespace mono { namespace media {

    /**
     * Generic image container class, defining generic properties and methods
     * for all image formats.
     *
     * Subclasses of this class implement different image formats, that can be
     * decoded by mono framework. See @ref BMPImage
     *
     * The class is an abstract interface that defines 3 methods for reading the
     * raw pixel data from the image, these works like file I/O with a read/write
     * cursor.
     *
     * * @ref ReadPixelData Reads individual raw pixels (@ref Color) from the image
     * * @ref SkipPixelData Skip a number of pixels
     * * @ref SeekToHLine Jump to a specific horizontal line in the image (Y coordinate)
     */
    class Image
    {
    public:

        /**
         * Returns the width of the image in pixels
         *
         */
        virtual int Width() = 0;

        /**
         * Returns the height of the image in pixels
         */
        virtual int Height() = 0;

        /**
         * Returns the pixel size in bytes. This is the format pixels are returned
         * in, by the method @ref ReadPixelData
         *
         * @returns Pixel size in bytes
         */
        virtual int PixelByteSize() = 0;

        /**
         * Read raw uncompressed pixels from the image. The pixels are read from
         * the current position (as set by @ref SeekToHLine), and copied to the
         * `target` position.
         *
         * @param target Pointer to the array where pixels are copied to
         * @param pixelsToRead The number of pixels to read from the image
         * @returns The number of pixels that was actually read
         */
        virtual int ReadPixelData(void *target, int pixelsToRead) = 0;

        /**
         * Like @ref ReadPixelData but will just skip the pixels, not read them.
         * This method increments the image current position pointer, as it is set
         * by @ref SeekToHLine.
         *
         * @param pixelsToSkip The number of pixels to skip.
         * @returns The number of pixels thatwas skipped
         */
        virtual int SkipPixelData(int pixelsToSkip) = 0;

        /**
         * Changes the current position cursor of the image to a certain line in
         * the image.
         *
         * @param vertPos The horizontal line in the image, the Y-coordinate
         */
        virtual void SeekToHLine(int vertPos) = 0;

        /**
         * Returns `true` if image is usable.
         * @returns `true` if you can actually get a proper image, `false` otherwise
         */
        virtual bool IsValid() = 0;
    };

} }

#endif
