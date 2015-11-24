//
//  network_request.cpp
//  wifiTest
//
//  Created by Kristoffer Lyder Andersen on 17/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//
#include <mbed.h>
#include "network_request.h"

using namespace mono::network;

void INetworkRequest::setState(INetworkRequest::States newState)
{
    if (newState == state)
        return;
    
    StateChangeEvent evnt;
    evnt.Context = this;
    evnt.NewState = newState;
    evnt.OldState = state;
    
    this->state = newState;
    
    if (stateChangeHandler) {
        stateChangeHandler.call(&evnt);
    }
}

void INetworkRequest::triggerCompletionHandler()
{
    setState(COMPLETED_STATE);
    
    if (completionHandler == false)
        return;
    
    CompletionEvent evnt;
    evnt.Context = this;
    
    completionHandler.call(&evnt);
}

void INetworkRequest::triggerDirectErrorHandler()
{
    setState(ERROR_STATE);
    
    if (errorHandler == false)
        return;
    
    ErrorEvent evnt;
    evnt.Context = this;
    evnt.ErrorCode = lastErrorCode;
    
    errorHandler.call(&evnt);
}

void INetworkRequest::triggerQueuedErrorHandler()
{
    cbTimer = Timer::callOnce<INetworkRequest>(0, this, &INetworkRequest::triggerDirectErrorHandler);
}

/// PUBLIC METHODS

INetworkRequest::States INetworkRequest::State() const
{
    return state;
}

bool INetworkRequest::IsCompleted() const
{
    return state == COMPLETED_STATE;
}

bool INetworkRequest::HasFailed() const
{
    return state == ERROR_STATE;
}

INetworkRequest::~INetworkRequest()
{
    if (cbTimer) {
        debug("deleting the pending callback trigger timer\n\r");
        delete cbTimer;
    }
}
