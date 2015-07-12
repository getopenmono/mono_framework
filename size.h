//
//  size.h
//  displaySimTest
//
//  Created by Kristoffer Lyder Andersen on 11/07/15.
//
//

#ifndef __displaySimTest__size__
#define __displaySimTest__size__

namespace mono { namespace geo {
    
    /**
     * Class to represent a rectangular size, meaning something that has a width
     * and a height.
     * A size class does not have any position in space, it only defines 
     * dimensions.
     */
    class Size
    {
    protected:
        int width, height;
        
    public:
        
        /**
         * Construct a NULL size. A size that has no width or height.
         */
        Size();
        
        /**
         * Construct a size with defined dimenstions
         */
        Size(int w, int h);
        
        /**
         * Construct a size based on another size
         */
        Size(const Size &s);
        
        Size& operator=(const Size &s);
        
        //Getters
        int Width();
        int Height();
    };
    
} }

#endif /* defined(__displaySimTest__size__) */
