
#ifndef icon_view_h
#define icon_view_h

#include <view.h>
#include <mono_icon.h>

using mono::display::Color;
using mono::display::MonoIcon;

namespace mono { namespace ui {
    
    class IconView : public View {
        
        const MonoIcon *icon;
        Color foreground;
        Color background;
        
    public:
        
        // Accessors
        
        void setForeground(Color c);
        void setBackground(Color c);
        
        Color Foreground() const;
        Color Background() const;
        
        IconView();
        IconView(const geo::Point &position, const MonoIcon &icon);
        
        void repaint();
    };
    
} }

#endif /* icon_view_hpp */
