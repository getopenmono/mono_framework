//
//  network_request.h
//  wifiTest
//
//  Created by Kristoffer Lyder Andersen on 17/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#ifndef network_request_h
#define network_request_h

#include "mono.h"
#include <FunctionPointer.h>

namespace mono { namespace network {
    
    /**
     * Interface class that defines a generic network request. Network request
     * have a notion of 3 states:
     *
     * 1. setup
     * 2. in progress
     * 3. completed
     * 4. error
     *
     */
    class INetworkRequest
    {
    public:
        
        enum Errors
        {
            NO_ERROR = 0,
            NETWORK_NOT_READY_ERROR,
            COMMUNICATION_ERROR,
            INVALID_STATE_ERROR,
            DNS_RESOLUTION_FAILED_ERROR,
            URL_PARSE_ERROR
        };
        
        enum States
        {
            SETUP_STATE,
            IN_PROGRESS_STATE,
            COMPLETED_STATE,
            ERROR_STATE
        };
        
        class ErrorEvent
        {
        public:
            INetworkRequest *Context;
            int ErrorCode;
        };
        
        class CompletionEvent
        {
        public:
            INetworkRequest *Context;
        };
        
        class StateChangeEvent
        {
        public:
            INetworkRequest *Context;
            States OldState;
            States NewState;
        };
        
    protected:
        /**
         * @brief Handler called upon successfull completion of request
         */
        mbed::FunctionPointerArg1<void, CompletionEvent*> completionHandler;
        
        /**
         * @brief Handler called upon any error (transition into ERROR_STATE)
         */
        mbed::FunctionPointerArg1<void, ErrorEvent*> errorHandler;
        
        /** handler called whenever the states changes */
        mbed::FunctionPointerArg1<void, StateChangeEvent*> stateChangeHandler;
        
        /** the current state of the request */
        States state;
        
        /** holds the latest error code, 0 = no error */
        int lastErrorCode;
        
        
        /** queued callback timer object */
        Timer *cbTimer;
        
        /** set new states and trigger stateCHange callback if state changed */
        void setState(States newState);
        
        /** calls the completion handler */
        void triggerCompletionHandler();
        
        /** call the error handler, directly */
        void triggerDirectErrorHandler();
        
        /** call the error callback, from the run queue */
        void triggerQueuedErrorHandler();
        
        /** A new network request in SETUP state */
        INetworkRequest() : state(SETUP_STATE), cbTimer(NULL) {};
        
    public:
        
        virtual ~INetworkRequest();
        
        /**
         * @brief Get the current state of the underlying network request
         *
         * @returns The current state of the request
         */
        States State() const;
        
        /**
         * @brief Check to see if the underlying network request has finished and
         * are in the `COMPLETE_STATE`
         *
         * @returns `true` if the request is completed
         */
        bool IsCompleted() const;
        
        /**
         * @brief Check to see if the underlying network has failed, and are in
         * `ERROR_STATE`
         *
         * @returns `true` if the request has failed
         */
        bool HasFailed() const;
        
        /**
         * To receive a notification callback when the underlying network request
         * finishes successfully, use this method to install a callback function.
         *
         * @brief Set the completion handler callback function
         * @param cnxt A pointer to the callback context (the `this` pointer)
         * @param memPtr A pointer to the context class' member function
         */
        template <typename Owner>
        void setCompletionCallback(Owner *cnxt, void(Owner::*memPtr)(CompletionEvent*))
        {
            this->completionHandler.attach(cnxt, memPtr);
        }
        
        /**
         * To receive a notification callback if this request fails, set a 
         * callback function here.
         *
         * @brief Set an error handler callback function
         * @param cnxt A pointer to the callback context (the `this` pointer)
         * @param memPtr A pointer to the context class' member function
         */
        template <typename Owner>
        void setErrorCallback(Owner *cnxt, void(Owner::*memPtr)(ErrorEvent*))
        {
            this->errorHandler.attach(cnxt, memPtr);
        }
        
        /**
         * To receive notifications of state changes to the underlying network
         * request, install a callback function with this method.
         *
         * @brief Set a state change observer callback
         * @param cnxt A pointer to the callback context (the `this` pointer)
         * @param memPtr A pointer to the context class' member function
         */
        template <typename Owner>
        void setStateChangeEventCallback(Owner *cnxt, void(Owner::*memPtr)(StateChangeEvent*))
        {
            this->stateChangeHandler.attach(cnxt, memPtr);
        }
    };
    
} }

#endif /* network_request_h */
