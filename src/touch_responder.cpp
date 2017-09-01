// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "touch_responder.h"
#include <stdio.h>
#include "consoles.h"
#include <deprecated.h>

#include <mbed_debug.h>

using namespace mono;

GenericQueue<TouchResponder> TouchResponder::ResponderChain;

TouchResponder::TouchResponder()
{
    activate();
}

void TouchResponder::respondTouchBegin(mono::TouchEvent &event)
{
    static bool breakReponderLoop = false;
    if (breakReponderLoop)
        return;

    breakReponderLoop = true;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    RespondTouchBegin(event);
#pragma GCC diagnostic pop
    breakReponderLoop = false;
}

void TouchResponder::respondTouchMove(mono::TouchEvent &event)
{
    static bool breakReponderLoop = false;
    if (breakReponderLoop)
        return;

    breakReponderLoop = true;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    RespondTouchMove(event);
#pragma GCC diagnostic pop
    breakReponderLoop = false;
}

void TouchResponder::respondTouchEnd(mono::TouchEvent &event)
{
    static bool breakReponderLoop = false;
    if (breakReponderLoop)
        return;

    breakReponderLoop = true;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    RespondTouchEnd(event);
#pragma GCC diagnostic pop
    breakReponderLoop = false;
}

void TouchResponder::activate()
{
    ResponderChain.enqueue(this);
}

void TouchResponder::deactivate()
{
    ResponderChain.remove(this);
}

void TouchResponder::runResponderChainTouchBegin(mono::TouchEvent &event)
{
    if (ResponderChain.peek() == NULL)
    {
        defaultSerial.printf("No first touch responder!\r\n");
        return;
    }

    TouchResponder *res = ResponderChain.peek();

    while (res != NULL) {
        res->respondTouchBegin(event);

        if (event.handled)
            return;

        res = ResponderChain.next(res);
    }
}

void TouchResponder::runResponderChainTouchEnd(mono::TouchEvent &event)
{
    if (ResponderChain.peek() == NULL)
    {
        return;
    }

    TouchResponder *res = ResponderChain.peek();

    while (res != NULL) {

        res->respondTouchEnd(event);

        if (event.handled)
            return;

        res = ResponderChain.next(res);
    }
}

void TouchResponder::runResponderChainTouchMove(mono::TouchEvent &event)
{
    if (ResponderChain.peek() == NULL)
    {
        return;
    }

    TouchResponder *res = ResponderChain.peek();

    while (res != NULL) {

        res->respondTouchMove(event);

        res = ResponderChain.next(res);
    }
}

TouchResponder* TouchResponder::FirstResponder()
{
    return ResponderChain.peek();
}
