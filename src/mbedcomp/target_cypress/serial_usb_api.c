// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#include "serial_usb_api.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <project.h>
#ifdef __cplusplus
}
#endif


extern struct serial_port_s serial_ports[2];

char serial_usbuart_is_powered = 0;
static uint32_t serial_usbuart_first_enumeration_timeout_ms = 300;
static uint32_t serial_usbuart_enumeration_delay_ms = 200;

char serial_usbuart_try_enumeration(struct serial_port_s *port)
{
    if (port->USB_GetConfiguration())
    {
        port->cdcInited = 1;
        port->USB_CDC_Init();
        return 1;
    }
    else
    {
        port->cdcInited = 0;
        port->lastEnumTry = us_ticker_read();
        return 0;
    }
}

char serial_usbuart_init_cdc(serial_t *obj)
{
    //enumerate done?
    if (obj->port->cdcInited)
        return 1;

    if (!serial_usbuart_is_powered)
    {
        return 0;
    }

    //usb has power, start it if needed
    if (!obj->port->usbStarted)
    {
        CyGlobalIntEnable;
        obj->port->USB_Start(0, USBUART_5V_OPERATION);
        obj->port->usbStarted = 1;
    }

    // not enumerated (first time)
    if (obj->port->lastEnumTry == 0)
    {
        uint32_t timeOut = serial_usbuart_first_enumeration_timeout_ms << 10;
        uint32_t stopTime = us_ticker_read() + timeOut;
        while(obj->port->USB_GetConfiguration() == 0 && us_ticker_read() < stopTime);

        return serial_usbuart_try_enumeration(obj->port);
    }
    else
    {
        // later enumeration tries
        uint32_t now = us_ticker_read();
        uint32_t diff = (now - obj->port->lastEnumTry) >> 10; // divide by 1024
        if (diff > serial_usbuart_enumeration_delay_ms)
        {
            return serial_usbuart_try_enumeration(obj->port);
        }
        else
            return 0;
    }

}

char serial_usbuart_is_started()
{
    return serial_ports[0].usbStarted;
}

void serial_usbuart_stopped()
{
    serial_ports[0].usbStarted = 0;
    serial_ports[0].cdcInited = 0;
}

char serial_usbuart_dtr(serial_t *obj)
{
    if (!serial_usbuart_init_cdc(obj))
        return 0;

    return obj->port->USB_CDC_GetLineControl() & USBUART_LINE_CONTROL_DTR;
}
