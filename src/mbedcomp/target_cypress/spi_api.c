//
//  spi_api.c
//  spiTest
//
//  Created by Kristoffer Andersen on 26/05/15.
//
//

#include "spi_api.h"
#include <mbed_error.h>
#ifdef __cplusplus
extern "C" {
#endif
#include <project.h>
#ifdef __cplusplus
}
#endif

static const int spi_ports_num = 3;

struct spi_port_s spi_ports[3] =
{
    {
        RP_SPI_HARD_WIRE, RP_SPI_CS, 8, 1,
        &SPI_RP_Start, &SPI_RP_Stop,
        &SPI_RP_WriteTxData, &SPI_RP_ReadTxStatus,
        &SPI_RP_GetRxBufferSize, &SPI_RP_ReadRxData
    },
	{
        TFT_SPI_HARD_WIRE, TFT_SPI_CS, 8, 1,
        &SPI1_Start, &SPI1_Stop,
        (void(*)(uint8_t))&SPI1_WriteTxData, &SPI1_ReadTxStatus, // typecast to silence warnings
        &SPI1_GetRxBufferSize, (uint8_t(*)(void))&SPI1_ReadRxData // again here
    },
    {
        SD_SPI_HARD_WIRE, SD_SPI_CS, 8, 1,
        &SPI_SD_Start, &SPI_SD_Stop,
        &SPI_SD_WriteTxData, &SPI_SD_ReadTxStatus,
        &SPI_SD_GetRxBufferSize, SPI_SD_ReadRxData
    }
};


void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    for (int i=0; i<spi_ports_num; i++)
    {
        struct spi_port_s *port = &spi_ports[i];
        
        if (port->hardwire == mosi)
        {
            obj->spiPortNum = i;
            obj->port = port;
            if (!port->hardwareCS)
            {
                obj->cs = ssel;
            }
            
            CyGlobalIntEnable;
            port->SPI_Start();
            return;
        }
    }
    
    obj->cs = NC;
    obj->spiPortNum = -1;
    obj->port = NULL;
}

void spi_free         (spi_t *obj)
{
    if (obj->port == NULL)
        return;
    
    obj->port->SPI_Stop();
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    obj->bitmode = bits;
}

void spi_frequency(spi_t *obj, int hz)
{
    
}

void spi_setHighCS(spi_t *obj)
{
    if (obj->port != NULL && !obj->port->hardwareCS)
        CyPins_SetPin(obj->cs);
}

int  spi_master_write (spi_t *obj, int value)
{
    //set the CS pin
    //CyPins_ClearPin(obj->cs); // activate chip select
    int timeout = 1000;
    // only 8 bit transfer
    if (obj->port != NULL && obj->bitmode == 8)
    {
        obj->port->SPI_WriteTxData(value);
        int to = 0;
        while((obj->port->SPI_ReadTxStatus() & SPI_RP_STS_BYTE_COMPLETE) == 0 && timeout > to++)
        {
            CyDelayUs(1);
        }
        int retval = 0;
        if (obj->port->SPI_GetRxBufferSize() > 0)
            retval = obj->port->SPI_ReadRxData();
        //CyPins_SetPin(obj->cs);
        if (to < timeout)
            return retval;
        else
            return 0xFF;
    }
    else if (obj->port != NULL && obj->bitmode == 16)
    {
        int retval = 0;
        uint8_t *retarray = (uint8_t*) &retval;
        uint8_t *valarray = (uint8_t*) &value;
        for (int c=0; c<2; c++) {
            int to = 0;
            obj->port->SPI_WriteTxData(valarray[c]);
            
            while ((obj->port->SPI_ReadTxStatus() & SPI_RP_STS_BYTE_COMPLETE) == 0 && to++ < timeout) {
                CyDelayUs(1);
            }
            
            retarray[c] = obj->port->SPI_ReadRxData();
        }
        
        return retval;
    }
    else if (obj->port != NULL && obj->bitmode == 32)
    {
        int retval = 0;
        uint8_t *retarray = (uint8_t*) &retval;
        uint8_t *valarray = (uint8_t*) &value;
        for (int c=0; c<4; c++) {
            int to = 0;
            obj->port->SPI_WriteTxData(valarray[c]);
            
            while ((obj->port->SPI_ReadTxStatus() & SPI_RP_STS_BYTE_COMPLETE) == 0 && to++ < timeout) {
                CyDelayUs(1);
            }
            
            retarray[c] = obj->port->SPI_ReadRxData();
        }
        
        return retval;
    }
    else
    {
        error("Unsupported format: %i, valid formats are: 8,16 & 32.\n",obj->bitmode);
    }
        
    
    return 0xFF;
}


int  spi_slave_receive(spi_t *obj)
{
    return 0;
}

int  spi_slave_read   (spi_t *obj)
{
    return 0;
}

void spi_slave_write  (spi_t *obj, int value)
{
    
}

int  spi_busy         (spi_t *obj)
{
    return 0;
}
