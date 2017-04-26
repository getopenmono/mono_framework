// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef view_alike_h
#define view_alike_h

#include <rect.h>

namespace mono { namespace ui {
    
    /**
     * @brief A viewable interface, something that can act a like a view
     *
     * A *ViewALike* class is something that can be shown, hidden and painted.
     * Or at least give the impression of being painted. A *ViewALike* does not
     * necessarily need to draw to the screen, but can just expose something
     * similar to that.
     *
     * A *ViewALike* also have a dimension and position in a cartesian
     * coordinate system. This represents the area where the content is 
     * displayed.
     */
    class IViewALike {
    public:
        
        // MARK: Visibility states
        
        /** @brief Get the current shown / hidden state */
        virtual bool Visible() const = 0;
        
        /** @brief Change the *ViewALike* state to shown */
        virtual void show() = 0;
        
        /** @brief Change the *ViewALike* state to hidden */
        virtual void hide() = 0;
        
        // MARK: Repainting
        
        /**
         * @brief Schedule the repaint of this ViewALike
         *
         * Any implementing subclass must take action to re-display it content
         * on the screen. (Only if the object is visible)
         *
         * **Note:** This method should not directly call any repaints, but only
         * schedule repainting, to be done later.
         *
         * @see Visible
         * @see show
         */
        virtual void scheduleRepaint() = 0;
        
        // MARK: Rect areas
        
        /**
         * @brief Set the ViewALike's position and dimension
         *
         * You provide a @ref Rect that defines a new offset to origin, and
         * overall dimensions of the viewable area.
         *
         * Note that it is a subclass reponsibility to obey these boundaries
         * in its repainting routines. A rouge subclass might ignore this
         * display rectangle completely.
         *
         * @param rect The new display rectangle
         */
        virtual void setRect(geo::Rect rect) = 0;
        
        /**
         * @brief Get the current position of the *ViewALike*'s upper left
         * corner.
         *
         * @returns A reference to the current position
         */
        virtual geo::Point &Position() = 0;
        
        /**
         * @brief Get the *ViewALike* current size rectangle
         * @returns A reference to the view's size rectangle
         */
        virtual geo::Size &Size() = 0;
        
        
        /**
         * @brief Get the *ViewALike* *view rect*
         *
         * This method returns a reference to the views current view rect.
         */
        virtual const geo::Rect &ViewRect() const = 0;
    };
    
} }

#endif /* view_alike_h */
