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

using namespace mono;

TouchResponder *TouchResponder::FirstResponder = NULL;

TouchResponder::TouchResponder()
{
    nextResponder = NULL;
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
    if (FirstResponder == NULL)
        FirstResponder = this;
    else
    {
        TouchResponder *res = FirstResponder;
        
        while (res->nextResponder != NULL) {
            res = res->nextResponder;
        }
        
        res->nextResponder = this;
    }
}

void TouchResponder::Deactivate()
{
    if (FirstResponder == this)
    {
        FirstResponder = this->nextResponder;
    }
    else if (FirstResponder != NULL)
    {
        TouchResponder *res = FirstResponder;
        
        while (res->nextResponder != this && res->nextResponder != NULL) {
            res = res->nextResponder;
        }
        
        if (res->nextResponder == NULL)
            return; // Im not in responder chain
        
        
        res->nextResponder = this->nextResponder;
    }
}

void TouchResponder::RunResponderChainTouchBegin(mono::TouchEvent &event)
{
    
    if (TouchResponder::FirstResponder == NULL)
    {
        defaultSerial.printf("No first touch responder!\n\r");
        return;
    }
    
    TouchResponder *res = TouchResponder::FirstResponder;
    
    while (res != NULL) {
        res->RespondTouchBegin(event);
        
        if (event.handled)
            return;
        
        res = res->nextResponder;
    }
}

void TouchResponder::RunResponderChainTouchEnd(mono::TouchEvent &event)
{
    if (TouchResponder::FirstResponder == NULL)
    {
        return;
    }
    
    TouchResponder *res = TouchResponder::FirstResponder;
    
    while (res != NULL) {
        
        res->RespondTouchEnd(event);
        
        if (event.handled)
            return;
        
        res = res->nextResponder;
    }
}

void TouchResponder::RunResponderChainTouchMove(mono::TouchEvent &event)
{
    if (TouchResponder::FirstResponder == NULL)
    {
        return;
    }
    
    TouchResponder *res = TouchResponder::FirstResponder;
    
    while (res != NULL) {
        
        res->RespondTouchMove(event);
        
        res = res->nextResponder;
    }
}
