// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef abstract_button_view_h
#define abstract_button_view_h

#include "responder_view.h"

namespace mono { namespace ui {

    class AbstractButtonView : public ResponderView
    {
    protected:
        mbed::FunctionPointer clickHandler;

        virtual void touchBegin(TouchEvent &event);
        virtual void touchMove(TouchEvent &event);
        virtual void touchEnd(TouchEvent &event);

        bool isPressedDown;

        /**
         * @brief Cannot construct abstract buttons
         *
         */
        AbstractButtonView();

        /**
         * @brief Construct a button with a defined viewRect
         * 
         */
        AbstractButtonView(geo::Rect rct);

    public:


        /**
         * @brief Attach a member function as the button click handler
         *
         * Provide the callback member function you ewant to be called when
         * the button is clicked.
         *
         * **NOTE:** THere can only be one callback function
         *
         * @param obj A pointer to the object where the callback method exists
         * @param memPtr A pointer to the callback method itself
         */
        template <typename Owner>
        void setClickCallback(Owner *obj, void(Owner::*memPtr)(void))
        {
            clickHandler.attach<Owner>(obj, memPtr);
        }

        /**
         * @brief Attach a C function pointeras the button click handler
         *
         * Provide a pointer to the callback C function, you ewant to be called
         * when the button is clicked.
         *
         * **NOTE:** THere can only be one callback function.
         *
         * @param memPtr A pointer to the C function callback
         */
        void setClickCallback(void(*memPtr)(void))
        {
            clickHandler.attach(memPtr);
        }



    };

} }


#endif /* abstract_button_view_h */
