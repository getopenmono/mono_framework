// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef icon_view_h
#define icon_view_h

#include <view.h>
#include <mono_icon.h>

using mono::display::Color;
using mono::display::MonoIcon;

namespace mono { namespace ui {

    /**
     * @brief A view that display a bitmap icon
     *
     * This UI widget display the bitmap contents of the Mono's icon format. To
     * display the icon you just provide a reference to the global variable for
     * the icon, and a position offset.
     *
     * ## Mono Icons
     *
     * The icon format is a bitmasps of color blending values. This means every
     * pixel value represents a blend between to colors, the fore- and background.
     * You set these colors specifically or uses the default system values. The
     * technique allows icon to be semi-transparent over a background.
     *
     * The default values for foreground color are the @ref View static variable
     * @ref StandardBorderColor. SImialr the default background is
     * @ref StandardBackgroundColor
     *
     * ## System  icons
     *
     * The framework contains a number of default icons each in two sizes: 16x16
     * and 24x24 pixels. These are distributed as part of the official SDK
     * releases. These icons include:
     *
     * * `alarmBell16`: defined in *alarm-bell-16.h*
     * * `alarmBell24`: defined in *alarm-bell-24.h*
     * * `alarmClock16`: defined in *alarm-clock-16.h*
     * * `alarmClock24`: defined in *alarm-clock-24.h*
     * * `clock16`: defined in *clock-16.h*
     * * `clock24`: defined in *clock-24.h*
     * * `cloudComputing16`: defined in *cloud-computing-16.h*
     * * `cloudComputing24`: defined in *cloud-computing-24.h*
     * * `mute16`: defined in *mute-16.h*
     * * `mute24`: defined in *mute-24.h*
     * * `pauseSymbol16`: defined in *pause-symbol-16.h*
     * * `pauseSymbol24`: defined in *pause-symbol-24.h*
     * * `playButton16`: defined in *play-button-16.h*
     * * `playButton24`: defined in *play-button-24.h*
     * * `reload16`: defined in *reload-16.h*
     * * `reload24`: defined in *reload-24.h*
     * * `settings16`: defined in *settings-16.h*
     * * `settings24`: defined in *settings-24.h*
     * * `speaker16`: defined in *speaker-16.h*
     * * `speaker24`: defined in *speaker-24.h*
     * * `thermometer16`: defined in *thermometer-16.h*
     * * `thermometer24`: defined in *thermometer-24.h*
     * * `upload16`: defined in *upload-16.h*
     * * `upload24`: defined in *upload-24.h*
     * * `wifiMediumSignalSymbol16`: defined in *wifi-medium-signal-symbol-16.h*
     * * `wifiMediumSignalSymbol24`: defined in *wifi-medium-signal-symbol-24.h*
     *
     * To see the full list of available system icons, you should see the source
     * code foler: `/include/display/icons` or on
     * [Github](https://github.com/getopenmono/mono_framework): `/resources/icons`
     *
     * ## Example
     *
     * Setup an IconView with the speaker 16x16 icon:
     *
     * @code
     * #include <speaker-16.h>
     *
     * IconView icn(geo::Point(20,20), speaker);
     * icn.show();
     * @endcode
     *
     * ## Creating your own icons
     *
     * The tool we use to create icon objects (simple header files) are available
     * on GitHub and converts bitmap images (like JPG and PNG) into icons files.
     *
     * [img2icon on Github](https://github.com/getopenmono/img2icon)
     *
     * @see mono::display::MonoIcon
     */
    class IconView : public View {

        const MonoIcon *icon;
        Color foreground;
        Color background;

    public:

        /**
         * @brief Contruct an empty icon view, with no icon and zero dimensions
         */
        IconView();

        /**
         * @brief Construct an icon view with an icon and position offset
         *
         * You provide the offset position (not the @ref Rect) to the view and
         * the global variable for the icon object.
         *
         * The views rect (its dimensions) is calculated fro the icon you
         * provide.
         *
         * **Remember:** to include the header file for the icon you wish to use
         *
         * @param position The offset where the view is positioned
         * @param icon The reference to the global icon object
         */
        IconView(const geo::Point &position, const MonoIcon &icon);

        // MARK: Accessors

        /**
         * @brief Set the foreground color for the icon
         * @param c The color to use as foreground
         */
        void setForeground(Color c);

        /**
         * @brief Set the background blending color for the icon
         * @param c The color to use as background
         */
        void setBackground(Color c);

        /**
         * @brief Replace the existing icon with a new one
         * @param ocon replacement
         */
        void setIcon(const MonoIcon *icon);

        /**
         * @brief Get the current foreground color
         */
        Color Foreground() const;

        /**
         * @brief Get the current background blening color
         */
        Color Background() const;

        void repaint();
    };

} }

#endif /* icon_view_hpp */
