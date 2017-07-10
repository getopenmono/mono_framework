// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "touch_responder.h"
#include <stdio.h>
#include "consoles.h"

#include <mbed_debug.h>

using namespace mono;

GenericQueue<TouchResponder> TouchResponder::ResponderChain;

TouchResponder::TouchResponder()
{
    activate();
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

void TouchResponder::activate()
{
    ResponderChain.enqueue(this);
}

void TouchResponder::deactivate()
{
    ResponderChain.remove(this);
}

void TouchResponder::RunResponderChainTouchBegin(mono::TouchEvent &event)
{
    if (ResponderChain.peek() == NULL)
    {
        defaultSerial.printf("No first touch responder!\r\n");
        return;
    }

    TouchResponder *res = ResponderChain.peek();

    while (res != NULL) {
        res->RespondTouchBegin(event);

        if (event.handled)
            return;

        res = ResponderChain.next(res);
    }
}

void TouchResponder::RunResponderChainTouchEnd(mono::TouchEvent &event)
{
    if (ResponderChain.peek() == NULL)
    {
        return;
    }

    TouchResponder *res = ResponderChain.peek();

    while (res != NULL) {

        res->RespondTouchEnd(event);

        if (event.handled)
            return;

        res = ResponderChain.next(res);
    }
}

void TouchResponder::RunResponderChainTouchMove(mono::TouchEvent &event)
{
    if (ResponderChain.peek() == NULL)
    {
        return;
    }

    TouchResponder *res = ResponderChain.peek();

    while (res != NULL) {

        res->RespondTouchMove(event);

        res = ResponderChain.next(res);
    }
}

TouchResponder* TouchResponder::FirstResponder()
{
    return ResponderChain.peek();
}
