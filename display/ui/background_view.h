// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt
#ifndef __mono__background_view__
#define __mono__background_view__

#include "view.h"

namespace mono { namespace ui {

    class BackgroundView : public View {

    protected:

        display::Color bgColor;

    public:

        BackgroundView(display::Color color = StandardBackgroundColor);

        void setBackgroundColor(display::Color color);
        void repaint();
    };

} }

#endif // __mono__background_view__
