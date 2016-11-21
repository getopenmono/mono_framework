// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "spi_commands.h"

using namespace mono::redpine;

spiCommandC1 CommandC1::exportByteCommand()
{
    spiCommandC1 c1 = 0;
    c1 |= (this->CommandType << 6);
    c1 |= (this->ReadWrite << 5);
    c1 |= (this->RegisterMemoryAccess << 4);
    c1 |= (this->MemoryFrameAccess << 3);
    c1 |= (this->TransferLengthSelect << 2);
    c1 |= (this->TransferLength & 0x03);
    
    return c1;
}

spiCommandC2 CommandC2::exportByteCommand()
{
    spiCommandC2 c2 = 0;
    
    c2 |= this->DataGranularity << 6;
    c2 |= this->RegisterSelect & 0x3F;
    
    return c2;
}