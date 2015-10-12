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
#include <rect.h>

namespace mono {
    class IApplicationContext;
    namespace ui {
    
        class Animator; // forward declaration
        
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
     *
     * @TODO Something on dependence of AppContext and Appctrl design pattern
     */
    class View
    {
        friend mono::IApplicationContext;
        friend Animator;
    public:
        
        /**
         * Define the 4 dirrerent orientations the of display. The display controller
         * apply the orientation transformation to real display. For the UI Views
         * the coordinate system remains the same, it just changes width and height.
         * The orico is always the top left corner (defined relative to gravity),
         * no matter the physical orientation of mono's display.
         */
        enum Orientation
        {
            PORTRAIT = 0,       /**< Expected standard orientation of mono, where the thick edge is at the bottom */
            PORTRAIT_BOTTOMUP=1,/**< Upside-down of @ref PORTRAIT, where the thick edge is at the top */
            LANDSCAPE_RIGHT = 2,/**< @ref PORTRAIT rotated 90 degrees clock-wise */
            LANDSCAPE_LEFT = 3  /**< @ref PORTRAIT rotated 90 degrees counter clock-wise */
        };
        
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
         * The rect defines the position and size of this view on the screen.
         * This defines where the view rectangles upper left corner is situated,
         * and the width and height in pixels.
         */
        geo::Rect viewRect;
        
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
        virtual void setPosition(geo::Point pos);
        
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
        virtual void setSize(geo::Size siz);
        
        /**
         * Set the views position and size, by providing a rectangle object.
         *
         * *Note* that this method does not repaint the view, you must do that
         * explicitly.
         *
         * @param rect The view rectangle, containing size and position
         */
        virtual void setRect(geo::Rect rect);
        
        /**
         * @brief Get the current position of the views upper left corner
         * @returns A reference to the current position
         */
        virtual geo::Point &Position();
        
        /**
         * @brief Get the views current size rectangle
         * @returns A reference to the views size rectangle
         */
        virtual geo::Size &Size();
        
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
        
        /**
         * Returns the horizontal (X-axis) width of the display canvas, in pixels.
         * The width is always defined as perpendicular to gravity
         */
        static uint16_t DisplayWidth();
        
        /**
         * Returns the vertical (Y-axis) height of the display canvas, in pixels.
         * The height axis is meant to be parallel to the gravitational axis.
         */
        static uint16_t DisplayHeight();
        
        static geo::Size DisplaySize();
        
        /**
         * Returns the current physical display orientation of the display
         * The orientation is controlled by the @ref IDisplayController
         */
        static Orientation DisplayOrientation();
    };
    
    
} }

#endif /* defined(__displaySimTest__view__) */
