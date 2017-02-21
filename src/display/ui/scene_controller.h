
#ifndef scene_controller_h
#define scene_controller_h

#include <view.h>
#include <list>
#include <FunctionPointer.h>

namespace mono { namespace ui {

    // should inherit from a view interfcae - since not really a view
    class SceneController : public View
    {
    protected:
        
        std::list<View*> childviewList;
        display::Color backColor;
        mbed::FunctionPointerArg1<void, const SceneController&> showHandler, hideHandler;
        mbed::FunctionPointer dismissHandler;
        
    public:
        
        SceneController();
        
        SceneController(const geo::Rect &rect);
        
        virtual void addView(const View &child);
        
        virtual void removeView(const View &child);
        
        virtual void show();
        
        virtual void hide();
        
        void requestDismiss();
        
        virtual void repaint();
        
        display::Color BackgroundColor() const;
        void setBackground(display::Color color);
        
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
