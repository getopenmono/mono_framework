// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef scene_controller_h
#define scene_controller_h

#include <view_alike.h>
#include <rect.h>
#include <point.h>
#include <size.h>
#include <color.h>
#include <list>
#include <FunctionPointer.h>
#include <background_view.h>

namespace mono { namespace ui {

    /**
     * @brief A logical grouping of views, that together comprise a scene
     *
     * A Scene is a group of views that together represent a UI. The scene
     * enables you to @ref hide, @ref show and repaint all the views at once.
     *
     * You should use the scene to represent a complete UI that takes up a full
     * screen. Your application can have multiple scenes, that you can switch
     * between to navigate in your app.
     *
     * ## Adding views to a scene
     *
     * When you create a scene (constructing the object instance), you need to
     * add the views to it. You do this assigning the relevant view by calling
     * the @ref addView method, for every view.
     *
     * When a view is added to a scene, the scene are now in charge of
     * controlling the views state. It is the scene that handles repaints and
     * visibility states.
     *
     * Like views, scenes are constructed in `hidden` state.
     *
     * ## Changing scenes
     *
     * A scene is shown on the display by calling it @ref show method. This
     * triggers the callback set be @ref setShowCallback, and enables you
     * to provide your scene's views with the data to be displayed.
     *
     * When you wish to hide or navigate to another view, you must call @ref
     * requestDismiss. This will trigger the callback set by
     * @ref setDimissCallback, that must do the actual @ref hide call. Also,
     * this method must call show on the new scene to display.
     *
     * ## Scene background color
     *
     * Scenes have a background color, that is painted before any of the views
     * in the scene. By default the background color is @ref View::StandardBackgroundColor
     * , but you can change that with the @ref setBackground method.
     *
     * ## Scene dimensions
     *
     * A scene has display bounaries, just like views. However, these are not
     * enforced by the scene itself. They exist to allow you to enforce
     * restrictions on dimensions.
     */
    class SceneController : public IViewALike
    {
    protected:

        std::list<IViewALike*> childviewList;
        geo::Rect viewRect;
        bool visible;
        mbed::FunctionPointerArg1<void, const SceneController&> showHandler, hideHandler;
        mbed::FunctionPointer dismissHandler;
        BackgroundView background;

    public:

        /**
         * @brief Construct a scene that takes up the entire display
         *
         */
        SceneController();

        /**
         * @brief Construct a scene with specific position and dimensions
         *
         * @param rect The scenes visible Rect
         */
        SceneController(const geo::Rect &rect);

        // MARK: Scene methods

        /**
         * @brief Add a view to the scene
         *
         * @param child THe view to add to the scene
         */
        virtual void addView(const IViewALike &child);

        /**
         * @brief Remove a view from the scene
         * @param child The view to remove from the scene
         */
        virtual void removeView(const IViewALike &child);

        /**
         * @brief Call the scene dismiss callback handler
         *
         * @see setDismissCallback
         */
        void requestDismiss();

        /**
         * @brief Get the scenes background color
         */
        display::Color BackgroundColor() const;

        /**
         * @brief Set the scenes background color
         */
        void setBackground(display::Color color);

        // MARK: ViewAlike states

        /**
         * @brief Get the scene's visibility state
         */
        virtual bool Visible() const;

        /**
         * @brief Show the scene.
         *
         * This will trigger the @ref setShowCallback and call `show` on all
         * the scene's views.
         */
        virtual void show();

        /**
         * @brief Hide the scene
         *
         * This will call the scene's hide handler as set by @ref setHideCallback
         * and then call `hide` on all the scene's views.
         *
         * **Note:** When you need to change between scenes, you should use
         * @ref requestDismiss - not `hide`
         */
        virtual void hide();

        /**
         * @brief Schedule repaint of the scene's views
         */
        virtual void scheduleRepaint();

        /**
         * @brief Sets a new Rect for the scene's
         */
        virtual void setRect(geo::Rect rct);

        /**
         * @brief Get the scene's view rect area and offset
         */
        virtual const geo::Rect& ViewRect() const;

        /**
         * @brief Get the scene position (upper left corner)
         */
        virtual geo::Point& Position();

        /**
         * @brief Get the scene's dimension
         */
        virtual geo::Size& Size();

        // MARK: Scene callbacks

        /**
         * @brief Sets the callback handler for the `show` event
         *
         * You can provide a callback to be called when the scene is about to be
         * shown. Use this callback function to do any pre-setup of the scenes
         * internal views.
         *
         * @param cnxt The callback context, normally the `this` pointer
         * @param memptr The callback method on the context class.
         */
        template <typename Context>
        void setShowCallback(Context *cnxt, void(Context::*memptr)(const SceneController&))
        {
            showHandler.attach(cnxt, memptr);
        }

        /**
         * @brief Sets the callback handler for the `hide` event
         *
         * You can provide a callback to be called when the scene is about to be
         * hidden. Use this callback function to do any post-teardown of the
         * scenes internal views.
         *
         * @param cnxt The callback context, normally the `this` pointer
         * @param memptr The callback method on the context class
         */
        template <typename Context>
        void setHideCallback(Context *cnxt, void(Context::*memptr)(const SceneController&))
        {
            hideHandler.attach(cnxt, memptr);
        }

        /**
         * @brief Sets the callback handler for the `dismiss` event
         *
         * You can provide a callback to be called when the scene is about to be
         * dismissed. You can use this callback to hide this scene and setup
         * the new view to be displayed.
         *
         * @param cnxt The callback context, normally the `this` pointer
         * @param memptr The callback method on the context class
         */
        template <typename Context>
        void setDismissCallback(Context *cnxt, void(Context::*memptr)(void))
        {
            dismissHandler.attach<Context>(cnxt, memptr);
        }
    };
} }

#endif /* scene_controller_h */
