// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef async_h
#define async_h

#include "mn_timer.h"

namespace mono {

    /**
     * @brief Call a C++ function/member asynchronously
     * 
     * This function call execute a member function (method) asynchronously. The
     * method call is scheduled on the run-loop and called later in time.
     * 
     * You provide the context (`this` pointer and its type) along with a
     * pointer to the member function.
     *
     * ## Example
     * @code
     * async<AppClass>(this, &MyClass::methodToCall);
     * @endcode
     *
     * This function uses the @ref Timer to schedule the function call on the 
     * run loop.
     *
     * ## Aborting
     *
     * You are able to abort the function call, as long the the function has not
     * executed. This `async` function returns a pointer to the underlying
     * @ref Timer object. You can use this pointer to *Stop* the @ref Timer.
     * *Note* that when the timer executes it is dealloced, to you pointer is
     * not be valid at that point. Normally you can only expect the pointer to 
     * be valid in the same stack frame, as it was created.
     *
     * @param cnxt The context pointer, normally `this`
     * @param method A pointer to the member function on the *Context* class.
     * @return A pointer to the @ref Timer object that will execute the async call
     */
    template <typename Context>
    Timer* async(Context *cnxt, void(Context::*method)(void))
    {
        return Timer::callOnce<Context>(0, cnxt, method);
    }

    /**
     * @brief Call a C function asynchronously
     *
     * This function call execute a C function asynchronously. The
     * function call is scheduled on the run-loop and called later in time.
     *
     * You provide the pointer to the member function.
     *
     * ## Example
     * @code
     * async(&MyClass::MyStaticFunction);
     * @endcode
     *
     * This function uses the @ref Timer to schedule the function call on the
     * run loop.
     *
     * ## Aborting
     *
     * You are able to abort the function call, as long the the function has not
     * executed. This `async` function returns a pointer to the underlying
     * @ref Timer object. You can use this pointer to *Stop* the @ref Timer.
     * *Note* that when the timer executes it is dealloced, to you pointer is
     * not be valid at that point. Normally you can only expect the pointer to
     * be valid in the same stack frame, as it was created.
     *
     * @param cFunction A pointer to the C function.
     * @return A pointer to the @ref Timer object that will execute the async call
     */
    inline Timer* async(void(*cFunction)(void))
    {
        return Timer::callOnce(0, cFunction);
    }
}

#endif /* async_h */
