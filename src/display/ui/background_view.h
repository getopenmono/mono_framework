// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt
#ifndef __mono__background_view__
#define __mono__background_view__

#include "view.h"

namespace mono { namespace ui {

    /**
     * @brief A full screen solid colored background.
     * 
     * Use this view to paint the background any color you like. This view is by
     * default the same size as Mono's display. It can be used a solid colored
     * background for your GUI.
     *
     * To save memory you should only have one background view per app.
     *
     * ## Painting on top of the background
     *
     * To ensure that the background is *behind* all other UI widgets on the screen,
     * it has to be rendered first. Then all other views will apepear on top of
     * the background. To achieve this, it is important to keep track of your
     * repaint order. All paint calls must begin with a @ref scheduleRepaint to
     * the background view.
     *
     * ## Example
     * @code
     * // Construct the view (should alwaus be a class member)
     * mono::ui::BackgroundView bg;
     *
     * // set an exiting new background color
     * bg.setBackgroundColor(mono::display::RedColor);
     *
     * //show the background
     * bg.show();
     *
     * // show all my other views, after the call to bg.show()
     * @endcode
     */
    class BackgroundView : public View {

    protected:

        display::Color bgColor;

    public:

        /**
         * @brief Construct a Background view on the entire screen
         * 
         * This contructor takes a optional color to use for the background. If
         * no argument is provided the color default to the 
         * @ref View::StandardBackgroundColor
         *
         * Also be default the view dimension is the entire display, meaning it
         * has a bounding rect (@ref vireRect) that is (0,0,176,220).
         *
         * *Note*: Remember to all @ref show to make the view visible.
         *
         * @param color An optional bckground color
         */
        BackgroundView(display::Color color = StandardBackgroundColor);

        /**
         * @brief Construct a BackgroundView with a specific position and size
         *
         * If you need at solid color (as background) in a confined space on
         * the screen, then you can use this constructor.
         *
         * @param rect The position and size of the view
         * @param color An optional color, default is @ref StandardBackgroundColor
         */
        BackgroundView(geo::Rect const &rect, display::Color color = StandardBackgroundColor);
        
        /**
         * @brief Sets a new background color on the view.
         * 
         * Set a new background color on the view. If the view is shared between
         * multiple UI scenes, you can use this method to change the background
         * color.
         *
         * When changing the background color the view is not automatically 
         * repainted. You must call @ref scheduleRepaint yourself.
         */
        void setBackgroundColor(display::Color color);

        /**
         * @brief Gets the current background color
         */
        display::Color Color() const;

        void repaint();
    };

} }

#endif // __mono__background_view__
