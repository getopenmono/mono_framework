

#ifndef tabbar_view_h
#define tabbar_view_h

#include <view.h>
#include <icon_button.h>
#include <list>
#include <mbed.h>

using namespace mono::geo;
using mono::display::Color;

namespace mono { namespace ui {
    
    class TabBarView : public View
    {
    protected:
        
        Color background, borderColor;
        std::list<IconButtonView*> tabButtons;
        IconButtonView *activeButton;
        int btnCount;
        mbed::FunctionPointerArg1<void, IconButtonView &> btnHandler;
        
        void init();
        void buttonPushedHandler(IconButtonView &);
        
    public:
        
        static const int tabBarHeight = 35;
        
        // MARK: Constructors
        
        TabBarView();
        
        TabBarView(const Point &position);
        
        TabBarView(const Point &pos,
                   IconButtonView &btn1);
        
        TabBarView(const Point &pos,
                   IconButtonView &btn1,
                   IconButtonView &btn2);
        
        TabBarView(const Point &pos,
                   IconButtonView &btn1,
                   IconButtonView &btn2,
                   IconButtonView &btn3);
        
        // MARK: Accessor Methods
        
        void addButton(IconButtonView &btn);
        void removeButton(IconButtonView &btn);
        
        bool setActiveButton(IconButtonView &btn);
        IconButtonView* currentActiveButton() const;
        
        void setButtonPushCallback(void(*cfunc)(IconButtonView&));
        
        template <typename Context>
        void setButtonPushCallback(Context *cnxt, void(Context::*memptr)(IconButtonView&))
        {
            btnHandler.attach<Context>(cnxt, memptr);
        }
        
        std::list<IconButtonView*>::const_iterator buttonListBegin() const;
        std::list<IconButtonView*>::const_iterator buttonListEnd() const;
        
        // MARK: View Method Overloads
        
        void repositionButtonLayout();
        
        void show();
        
        void hide();
        
        void scheduleRepaint();
        
        void repaint();
    };
    
} }

#endif /* tabbar_view_h */
