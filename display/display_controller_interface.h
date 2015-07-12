//
//  display_controller_interface.h
//  displaySimTest
//
//  Created by Kristoffer Lyder Andersen on 27/06/15.
//
//

#ifndef displaySimTest_display_controller_interface_h
#define displaySimTest_display_controller_interface_h

#include <stdint.h>
#include <callback_handle.h>
#include "color.h"

namespace mono { namespace display
{
    /**
     * This interface a simple set of function that display interface must 
     * implement.
     * 
     * Mono dislpay system build upon this interface, and all drawing is done
     * through these functions.
     * 
     * You must override all the defined methods in this interface. The 
     * interface does not define or depend on a communication protocol, like
     * parrallel or SPI.
     *
     * @brief Abstract Interface for display controllers like ILITEK chip etc.
     * @author Kristoffer Andersen
     */
    class IDisplayController
    {
    protected:
        CallbackHandle<void*> refreshCallback;
    public:
        
        /**
         * Setup the display controller object, init vaiables and the screen 
         * size. The width is the horizontal measure, when mono is held in 
         * portrait mode.
         *
         * This constructor should not transfer any data or initialize the 
         * display. You must do that in the @ref init method.
         *
         * @param width The display width (horizontal) in pixels
         * @param height The display height (vertical) in pixels
         * @returns The display controller instance.
         */
        IDisplayController(int width, int height) {}
        
        /**
         * Initalizing the hardware display controller means setting up the
         * power, gamma control and sync options. The display should be ready to
         * draw graphics when this method returns.
         *
         * It is not nessasary to do any draw commands from this method, other
         * classes will take care of clearing the screen to black, etc.
         *
         * @brief Initializes the hardware display controller.
         */
        virtual void init() = 0;
        
        /**
         * The display controller must support windowing in software or in 
         * hardware. The window is the acrtive painting area of the screen, 
         * where you can paint. Upon drawing a single pixel the dispay cursor
         * increments inside the window. This means you can sometime skip calls
         * to the @ref setCursor method.
         *
         * @brief Changes the current active window to a new rectangle
         * @param x X-coordinate of the new windows upper left corner
         * @param y Y-coordinate of the new windows upper left corner
         * @param width The window width
         * @param height The window height
         */
        virtual void setWindow(int x, int y, int width, int height) = 0;
        
        virtual uint16_t ScreenWidth() const = 0;
        virtual uint16_t ScreenHeight() const = 0;
        
        /**
         * Set the callback function, that is called whenever the display has
         * just repainted itself.
         * This means it is time to repaint any dirty views, that needs to be
         * updated.
         *
         * *IMPORTANT*: You should re-paint graphics from within this callback,
         * since it might run inside a hardware interrupt. It is better to just
         * schedule the repaint from here.
         *
         * @brief Set the callback for display refreshes
         * @param obj The owner object of the callback method (the `this` context)
         * @param memPtr A pointer to the owner objects callback member function
         */
        template <typename Owner>
        void setRefreshCallback(Owner *obj, void(Owner::*memPtr)(void))
        {
            refreshCallback.attach<Owner>(obj, memPtr);
        }
        
        /**
         * Sets the current drawing cursor to a new position (must be within the
         * cuurently active window).
         *
         * When setting the cursor you must use absolute coordinates (screen 
         * coordinates), not coordinates inside the active window.
         *
         * @brief Set the drawing cursor to a new absolute position
         * @param x The new X position (screen coordinates)
         * @param y The new X position (screen coordinates)
         */
        virtual void setCursor(int x, int y) = 0;
        
        virtual int getCursorX() = 0;
        virtual int getCursorY() = 0;
        
        /**
         * Write a pixel color to the display, at the cursor position.
         * This method will automatically increment the cursor position.
         *
         * If the increment happens automatically in hardware, the controller
         * implementation must keep its own cursor in sync.
         *
         * @brief Draw a pixel with the given color, at cursor position.
         * @param pixelColor The 16-bit 5-6-5 RGB color to draw
         */
        virtual void write(Color pixelColor) = 0;
        
        
        
        
        virtual uint16_t read() = 0;
    };
} }



#endif
