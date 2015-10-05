//
//  callback_handle.h
//  mono
//
//  Created by Kristoffer Lyder Andersen on 12/07/15.
//  Copyright (c) 2015 Monolit Aps. All rights reserved.
//

#ifndef __mono__callback_handle__
#define __mono__callback_handle__

#include <string.h>

namespace mono {
    
    /**
     * Callback handler class. This class can store a callback funtion, along
     * with the owning object. This eefectively realizes callbacks in C++.
     *
     * A callback is defined by the callee object (the `this` context) and a
     * member function on the callee object/class.
     *
     * The callback is templated both in terms of the callback arguments to
     * pass along the calback function, and in terms of the callee object type.
     *
     * @brief Class to store and handle callback member methods
     */
    template <typename CallbackData>
    class CallbackHandle {
        
        /** internal callback function prototype */
        void (*_reqMemCaller)(void *, char *, CallbackData);
        
        void *_object; /**< callee object pointer */
        
        char _member[sizeof(void*)]; /**< member function pointer storage */
        
        /**
         * Internal static method to contruct and store the callback type
         * information. A pointer to this function is stored in the 
         * @ref _req_memCaller member.
         */
        template <typename T>
        static void callInternal(void *obj, char *member, CallbackData data) {
            T* o = static_cast<T*>(obj);
            void (T::*m)(CallbackData);
            memcpy((char*)&m, member, sizeof(m));
            (o->*m)(data);
        }
        
    public:
        
        CallbackHandle() { _reqMemCaller = NULL; }
        
        /** Check to see if a handler function is attached */
        char exists() {
            return _reqMemCaller != NULL ? true : false;
        }
        
        /** Set the function callback to this handler */
        template <typename T>
        void attach(T *obj, void(T::*mptr)(CallbackData) ) {
            _object = static_cast<void*>(obj);
            memcpy(_member, (char*)&mptr, sizeof(mptr));
            _reqMemCaller = &CallbackHandle::callInternal<T>;
        }
        
        /** Remove any callback function from this handler */
        void clear() {
            _reqMemCaller = NULL;
        }
        
        /** Execute any existing function handler */
        virtual void call(CallbackData data) {
            _reqMemCaller(_object, _member, data);
        }
    };
    
}


#endif /* defined(__mono__callback_handle__) */
