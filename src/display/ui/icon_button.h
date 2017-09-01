
#ifndef icon_button_h
#define icon_button_h

#include <abstract_button_view.h>
#include <icon_view.h>
#include <mbed.h>

using namespace mono::geo;
using mono::display::MonoIcon;
using mono::display::Color;

namespace mono { namespace ui {
    
    
    class IconButtonView : public AbstractButtonView
    {
    protected:
        IconView iconView;
        Color borderColor, iconColor;
        Color borderColorPressed;
        Color background;
        static const int iconInset = 2;
        const MonoIcon *_icon;
        
        // TODO: these should be view or abstract button or general views
        bool drawsBorder;
        bool drawsBackground;
        mbed::FunctionPointerArg1<void,IconButtonView&> cnxtPushHandler;
        
        void repositionIconView();
        void init();
        
        void callPushHandler();
        
    public:
        
        IconButtonView();
        
        IconButtonView(const Rect &rct, const MonoIcon &icon);
        
        // MARK: Accessor methods
        
        /**
         * @brief Sets the border color
         *
         * This method will not schedule repaint! You must @ref scheduleRepaint
         * manually.
         *
         * @param c The new border color
         */
        void setBorder(Color c);
        
        /**
         * @brief Sets the icons foreground color
         *
         * This method will not schedule repaint! You must @ref scheduleRepaint
         * manually.
         *
         * @param c The new icon color
         */
        void setIconForeground(Color c);
        
        /**
         * @brief Sets the highlight color (border & icon)
         *
         * The highlight color is the color used to represented a button that is
         * currently being pushed. This means that a touch event has started
         * inside its boundaries. The highlight color is applied to both border
         * and icon foreground.
         *
         * This method will not schedule repaint! You must @ref scheduleRepaint
         * manually.
         *
         * @param c The new highlight color
         */
        void setHighlight(Color c);
        
        /**
         * @brief Sets the background color
         *
         * The background color of the fill color inside the button bounding
         * rectangle.
         *
         * This method will not schedule repaint! You must @ref scheduleRepaint
         * manually.
         *
         * @param c The new border and text color
         */
        void setBackground(Color c);
        
        
        void setIcon(const MonoIcon &icn);
        //const MonoIcon& icon() const;
        
        virtual void setRect(geo::Rect rect);
        
        /**
         * @brief Control where a border is drawed
         */
        void setDrawsBorder(bool draws);
        
        /**
         * @brief Control whether background is drawed
         */
        void setDrawsBackground(bool draws);
        
        void setPushCallback(void(*cfunc)(IconButtonView&));
        
        template <typename Context>
        void setPushCallback(Context *cnxt, void(Context::*memptr)(IconButtonView&))
        {
            cnxtPushHandler.attach<Context>(cnxt, memptr);
        }
        
        // MARK: View Method Overloads
        
        void repaint();
    };
    
} }

#endif /* icon_button_h */
