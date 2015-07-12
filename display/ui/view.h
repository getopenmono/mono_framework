//
//  view.h
//  displaySimTest
//
//  Created by Kristoffer Andersen on 10/07/15.
//
//

#ifndef __displaySimTest__view__
#define __displaySimTest__view__

#include <display_painter.h>
#include <point.h>
#include <size.h>

namespace mono { namespace ui {
    
    /**
     * Abstract View class/interface. All UI view/widgets that paint to the 
     * screen must inherit from this class. Views handle repaint queues, touch
     * inputs and painting to the display buffer automatically.
     *
     * All views has a width and height, along with an absolute x,y coordinate.
     * The coordinates define the upper left corner of the view rectangle.
     *
     * As a design pattern, we chose that Views must not contain any state. They
     * only draw data to the display. Therefore view might contain or have 
     * references to object holding the actual state information.
     *
     * A TextLabel view
     */
    class View
    {
    protected:
        
        /**
         * Global View painter object. Once the first View is included in the
         * code, this painter is initialized on the stack. Every view object uses
         * this painter to paint itself.
         *
         * The painter is initialized with the display controller of the 
         * application context. If you want views to draw themselv on another
         * display, you musy subclass or change the current display controller
         * of the @ref mono::ApplicationContext object.
         */
        static display::DisplayPainter painter;
        
        /**
         * The position of this view on the screen.
         * This defines where the view rectangles upper left corner is situated.
         */
        geo::Point position;
        
        /**
         * The view dimensions: width and height i pixels.
         */
        geo::Size size;
        
        /**
         * @brief A pointer to the next view in the re-paint queue.
         * 
         * Then you call the @ref scheduleRepaint method, views use a chain of
         * `nextDirtyView` pointers to realize a queue. This eliminates the need
         * for a heap based variable length linked list, that implements a queue
         * structure.
         *
         * The static member @ref firstDiryView is the top of the queue, and all
         * views in the queue use this member to point to the next dirty view.
         *
         * The queue can be traversed by starting with @ref firstDirtyView and
         * then following the `nextDirtyView` pointer, until a `NULL` is encountered.
         */
        View *nextDirtyView;
        
        /**
         * The top of the scheduled repaint queue.
         * See the describing of the repaint queue implementation in 
         * @ref nextDirtyView
         */
        static View *firstDirtyView;
        
    public:
        
        View();
        
        /**
         * Changes the views position on the screen.
         * Note that changing the position does not implicitly redraw the view.
         * This means you will need to update the screen the affected areas
         * to make the change visible.
         *
         * @brief Change the views position on the screens coordinate system
         * @param pos The new position of the view
         */
        void setPosition(geo::Point pos);
        
        /**
         * Changes the views dimensions. The affect of size changes might depend
         * on the specefic view subclass. Some views might use their size to
         * calculate their internal layout - other might only support fixed sizes.
         *
         * Note that changing the size here does not redraw the view. The screen
         * needs to be redrawn the affected places, to make the size change
         * visible.
         *
         * @brief Change the size (width, height) of the view.
         * @param siz The new size of the view
         */
        void setSize(geo::Size siz);
        
        /**
         * Re-paint the view content. This method should be called then the view
         * content has changed.
         * You can call this method directly, but it might cause graphics artifacts
         * because the display is not double buffered. Instead you should 
         * schedule a repaint by calling the @ref scheduleRepaint() method. This
         * method will schedule the repaint, right after the next display update.
         *
         * The display system will not schedule any repaints automatically. The
         * view does not contain any state information, so you or other classes
         * utilizing view must schedule repaints.
         *
         * In subclasses of View, this method _must_ be overwritten.
         *
         * @brief Repaint the view content, using the @ref View::painter
         */
        virtual void repaint() = 0;
        
        /**
         * This method add the view to the display systems re-paint queue. The 
         * queue is executed right after a display refresh. This helps prevent
         * graphical artifacts, when running on a single display buffer system.
         *
         * Because view has no state information, they do not know when to 
         * repaint themselves. You or classes using views, must manually call
         * this repaint method when its time to repaint the view.
         *
         * @brief Schedule this view for repaint at next display refresh
         */
        void scheduleRepaint();
        
        /**
         * This class method will run through the scheduled re-paints queue and
         * call the @ref repaint method on all of them.
         *
         * This method is called automatically be the display system, you do not
         * need to call it yourself.
         */
        static void repaintScheduledViews();
        
    };
    
    
} }

#endif /* defined(__displaySimTest__view__) */
