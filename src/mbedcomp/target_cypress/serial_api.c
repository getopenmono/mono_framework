// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#include <serial_api.h>
#include "serial_usb_api.h"
#include "PinNames.h"

#ifdef __cplusplus
extern "C" {
#endif
//#include <USBUART.h>
//#include <USBUART_cdc.h>
#include <project.h>
#ifdef __cplusplus
}
#endif

const int serial_ports_num = 1;

int stdio_uart_inited = 0;
serial_t stdio_uart;

struct serial_port_s serial_ports[2] =
{
    {
        1, USBTX, USBRX,
        &USBUART_Start, &USBUART_Stop,
        &USBUART_GetConfiguration,
        &USBUART_IsConfigurationChanged,
        &USBUART_CDC_Init,
        &USBUART_PutData, &USBUART_PutChar,
        &USBUART_GetCount,
        &USBUART_CDCIsReady, &USBUART_DataIsReady,
        &USBUART_GetData, &USBUART_GetAll,
        &USBUART_GetChar,
        &USBUART_GetLineControl, 0, 0, 0
    },
    {
        0, STDIO_UART_TX, STDIO_UART_RX,
        NULL, NULL,
        NULL, //get conf
        NULL,
        NULL, // cdc init
        NULL, NULL,
        NULL, // get count
        NULL, NULL,
        NULL, NULL,
        NULL, // get char
        NULL, 0, 0, 1 // is IO stream fake port
    }
};

void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    for (int i=0; i<serial_ports_num; i++)
    {
        struct serial_port_s *port = &serial_ports[i];
        if (port->Tx == tx && port->Rx == rx)
        {
            obj->portNum = i;
            obj->port = port;
            return;
        }
    }

}



void serial_free(serial_t *obj)
{

}

void serial_baud(serial_t *obj, int baudrate)
{

}


void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{

}

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{

}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{

}

int  serial_getc(serial_t *obj)
{
    if (!serial_usbuart_init_cdc(obj))
        return 0;

    return obj->port->USB_CDC_GetChar();
}

void serial_putc(serial_t *obj, int c)
{
    if (!serial_usbuart_init_cdc(obj))
        return;

    int timeout = 1;
    while (!obj->port->USB_CDC_CDCIsReady() && timeout>0) {
        timeout--;
        CyDelay(1);
    }

    return obj->port->USB_CDC_PutChar(c);
}

int  serial_readable(serial_t *obj)
{
    if (!serial_usbuart_init_cdc(obj))
        return 0;

    return obj->port->USB_CDC_GetCount();

}

int  serial_writable(serial_t *obj)
{
    if (!serial_usbuart_init_cdc(obj))
        return 0;

    return obj->port->USB_CDC_CDCIsReady();

}

void serial_clear(serial_t *obj)
{

}

void serial_break_set(serial_t *obj)
{

}

void serial_break_clear(serial_t *obj)
{

}

void serial_pinout_tx(PinName tx)
{

}

void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow)
{

}
