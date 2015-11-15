//
//  touch_responder.cpp
//  mono framework
//
//  Created by Kristoffer Lyder Andersen on 05/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#include "touch_responder.h"
#include <stdio.h>
#include "consoles.h"

#include <mbed_debug.h>

using namespace mono;

GenericQueue<TouchResponder> TouchResponder::ResponderChain;

TouchResponder::TouchResponder()
{
    Activate();
}

void TouchResponder::RespondTouchBegin(mono::TouchEvent &event)
{
}

void TouchResponder::RespondTouchMove(mono::TouchEvent &event)
{
}

void TouchResponder::RespondTouchEnd(mono::TouchEvent &event)
{
}

void TouchResponder::Activate()
{
    ResponderChain.Enqueue(this);
}

void TouchResponder::Deactivate()
{
    ResponderChain.Remove(this);
}

void TouchResponder::RunResponderChainTouchBegin(mono::TouchEvent &event)
{
    if (ResponderChain.Peek() == NULL)
    {
        defaultSerial.printf("No first touch responder!\n\r");
        return;
    }
    
    TouchResponder *res = ResponderChain.Peek();
    
    while (res != NULL) {
        res->RespondTouchBegin(event);
        
        if (event.handled)
            return;
        
        res = ResponderChain.Next(res);
    }
}

void TouchResponder::RunResponderChainTouchEnd(mono::TouchEvent &event)
{
    if (ResponderChain.Peek() == NULL)
    {
        return;
    }
    
    TouchResponder *res = ResponderChain.Peek();
    
    while (res != NULL) {
        
        res->RespondTouchEnd(event);
        
        if (event.handled)
            return;
        
        res = ResponderChain.Next(res);
    }
}

void TouchResponder::RunResponderChainTouchMove(mono::TouchEvent &event)
{
    if (ResponderChain.Peek() == NULL)
    {
        return;
    }
    
    TouchResponder *res = ResponderChain.Peek();
    
    while (res != NULL) {
        
        res->RespondTouchMove(event);
        
        res = ResponderChain.Next(res);
    }
}

TouchResponder* TouchResponder::FirstResponder()
{
    return ResponderChain.Peek();
}
