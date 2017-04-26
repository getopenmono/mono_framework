
#ifndef scene_controller_h
#define scene_controller_h

#include <view_alike.h>
#include <rect.h>
#include <point.h>
#include <size.h>
#include <color.h>
#include <list>
#include <FunctionPointer.h>

namespace mono { namespace ui {

    // should inherit from a view interfcae - since not really a view
    class SceneController : public IViewALike
    {
    protected:
        
        std::list<IViewALike*> childviewList;
        display::Color backColor;
        geo::Rect viewRect;
        bool visible;
        mbed::FunctionPointerArg1<void, const SceneController&> showHandler, hideHandler;
        mbed::FunctionPointer dismissHandler;
        
    public:
        
        SceneController();
        
        SceneController(const geo::Rect &rect);
        
        // MARK: Scene methods
        
        virtual void addView(const IViewALike &child);
        
        virtual void removeView(const IViewALike &child);
        
        void requestDismiss();
        
        display::Color BackgroundColor() const;
        
        void setBackground(display::Color color);
        
        // MARK: ViewAlike states
        
        virtual bool Visible() const;
        
        virtual void show();
        
        virtual void hide();
        
        virtual void scheduleRepaint();
        
        virtual void setRect(geo::Rect const &rct);
        
        virtual const geo::Rect& ViewRect() const;
        
        virtual geo::Point& Position();
        
        virtual geo::Size& Size();
        
        // MARK: Scene callbacks
        
        template <typename Context>
        void setShowCallback(Context *cnxt, void(Context::*memptr)(const SceneController&))
        {
            showHandler.attach(cnxt, memptr);
        }
        
        template <typename Context>
        void setHideCallback(Context *cnxt, void(Context::*memptr)(const SceneController&))
        {
            hideHandler.attach(cnxt, memptr);
        }
        
        template <typename Context>
        void setDimissCallback(Context *cnxt, void(Context::*memptr)(void))
        {
            dismissHandler.attach<Context>(cnxt, memptr);
        }
    };
} }

#endif /* scene_controller_h */
