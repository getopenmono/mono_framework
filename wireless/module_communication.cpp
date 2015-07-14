//
//  module_communication.cpp
//  Mono::Redpine
//
//  Created by Kristoffer Andersen on 27/05/15.
//
//

#include "module_communication.h"
#include <mono.h>
#include <mbed.h>
#include <consoles.h>

using namespace mono::redpine;


bool ModuleCommunication::bufferIsMgmtFrame(DataReceiveBuffer &buffer)
{
    if ((buffer.buffer[1] & 0x40) == 0x40)
        return true;
    else
        return false;
}


bool ModuleCommunication::bufferIsDataFrame(DataReceiveBuffer &buffer)
{
    if ((buffer.buffer[1] & 0x50) == 0x50)
        return true;
    else
        return false;
}


SPIReceiveDataBuffer::SPIReceiveDataBuffer(int size) : DataReceiveBuffer()
{
    this->length = size;
    this->ownsMemory = true;
    
    mono::warning("SPIReceiveDataBuffer: allocating buffer on HEAP!");
    this->buffer = (uint8_t*) malloc(size);
}

SPIReceiveDataBuffer::SPIReceiveDataBuffer(frameDescriptorHeader &frmHead)
{
    this->length = (frmHead.totalBytes + 3) & (~3);
    this->ownsMemory = true;
    
    mono::warning("SPIReceiveDataBuffer: allocating buffer on HEAP!");
    this->buffer = (uint8_t*) malloc(this->length);
    this->bytesToRead = this->length;
}

SPIReceiveDataBuffer& SPIReceiveDataBuffer::operator<<(mbed::SPI *spi)
{
    if (length < bytesToRead)
    {
        mono::error("SPIReceiveDataBuffer read error, buffer is too small!");
        return *this;
    }
    
    spi->format(32);
    
    uint32_t buf = 0;
    int *writePtr = (int*) this->buffer;
    while (this->bytesToRead > 0)
    {
        buf = spi->write(0);
        
        //set LSByte on index 0, and MSByte on index 3
        *writePtr = (buf & 0xFF)<<24 | (buf & 0xFF00)<<8 | (buf & 0xFF0000)>>8 | (buf&0xFF000000)>>24;
        
        bytesToRead -= 4;
        writePtr++; // increment 4 bytes
    }
    
    spi->format(8);
    
    return *this;
}

SPIReceiveDataBuffer::~SPIReceiveDataBuffer()
{
    if (ownsMemory)
        free(this->buffer);
}

ModuleSPICommunication::ModuleSPICommunication(mbed::SPI &spi, PinName interruptPin)
{
    this->spi = &spi;
}

// PROTECTED AUXILLARY METHODS

CommandStatus ModuleSPICommunication::sendC1C2(spiCommandC1 c1, spiCommandC2 c2)
{
    int retval = BUSY_RESPONSE;
    int reTries = 0;
    while (retval == BUSY_RESPONSE && reTries < 10) {
        
        retval = spi->write(c1);
        
        if (retval != 0)
            mono::warning("Unexpected response for C1 command, not zero!");
        
        retval = spi->write(c2);
        
        if (retval == BUSY_RESPONSE)
            mbed::wait_ms(100);
        reTries++;
    }
    
    return (CommandStatus) retval;
}

CommandStatus ModuleSPICommunication::sendC1C2(CommandC1 c1, CommandC2 c2)
{
    return sendC1C2(c1.exportByteCommand(), c2.exportByteCommand());
}


bool ModuleSPICommunication::waitForStartToken(bool thirtyTwoBitMode)
{
    // set 32-bit mode if selected
    if (thirtyTwoBitMode)
        spi->format(32);
    
    int retval = 0;
    while (retval != START_TOKEN)
    {
        retval = spi->write(0x00);
        
        // 32-bit transfer command status code is big-endian
        // our target is little-endian, compensate for that!
        if (thirtyTwoBitMode && retval == START_TOKEN << 24)
        {
            retval = START_TOKEN;
        }
        
        if (retval != START_TOKEN)
            mbed::wait_ms(50);
    }
    
    // set mode back to 8 bit
    if (thirtyTwoBitMode)
        spi->format(8);
    
    //  we receive start token?
    if (retval == START_TOKEN)
        return true;
    else
        mono::error("Failed to receive start token");
    
    return false;
}

bool ModuleSPICommunication::readFrameDescriptorHeader(frameDescriptorHeader *buffer)
{
    CommandC1 cmd1;
    cmd1.CommandType = CommandC1::READ_WRITE;
    cmd1.ReadWrite = false;
    cmd1.RegisterMemoryAccess = true;
    cmd1.MemoryFrameAccess = true;
    cmd1.TransferLengthSelect = true;
    cmd1.TransferLength = CommandC1::FOUR_BYTES; // ignored
    
    CommandC2 cmd2;
    cmd2.DataGranularity = CommandC2::THIRTYTWO_BITMODE;
    cmd2.RegisterSelect = 0; // ignored
    
    if (sendC1C2(cmd1, cmd2) != CMD_SUCCESS)
    {
        mono::error("Failed to fetch frame length header!");
        return false;
    }
    
    spiCommandC3 c3 = 0x04; // read 4 bytes of frame
    spiCommandC4 c4 = 0x00;
    
    int retval = spi->write(c3);
    retval = spi->write(c4);
    
    retval = waitForStartToken();
    
    if (retval != true)
    {
        mono::error("Failed to fetch frame length header");
        return false;
    }
    
    //receive frame head 4-bytes
    spi->format(32);
    
    uint8_t readBuf[4];
//    readBuf[0] = spi->write(0);
//    readBuf[1] = spi->write(0);
//    readBuf[2] = spi->write(0);
//    readBuf[3] = spi->write(0);
    
    *((int*)readBuf) = spi->write(0);
    
    spi->format(8);
    
    // convert number to the current architecture endian
    // (module send as little-endian)
    buffer->dummyBytes = readBuf[1] | readBuf[0] << 8;
    buffer->totalBytes = readBuf[3] | readBuf[2] << 8;
    
    return true;
}

bool ModuleSPICommunication::readFrameBody(frameDescriptorHeader &frameHeader, SPIReceiveDataBuffer &buffer)
{
    // this is big-endian
    uint32_t frameLength = frameHeader.dummyBytes + frameHeader.totalBytes;
    
    //align 4-byte granularity, by AND with 0b111111100
    int readLength = (frameLength + 3) & (~3);
    
    // Setup init command structure
    CommandC1 c1;
    c1.CommandType = CommandC1::READ_WRITE;
    c1.ReadWrite = false;
    c1.RegisterMemoryAccess = true;
    c1.MemoryFrameAccess = true;
    c1.TransferLengthSelect = true;
    c1.TransferLength = CommandC1::ONE_BYTE; // ignored
    
    CommandC2 c2;
    c2.DataGranularity = CommandC2::THIRTYTWO_BITMODE;
    c2.RegisterSelect = 0; // ignored
    
    // send the commands to read a frame
    if (sendC1C2(c1,c2) != CMD_SUCCESS)
    {
        mono::error("Failed to send frameRead body c1 and c2 command");
        return false;
    }
    
    spiCommandC3 c3 = readLength & 0xFF; // lower byte
    spiCommandC4 c4 = (readLength & 0xFF00) >> 8; // upper byte
    
    // send read-length
    int status = spi->write(c3);
    status = spi->write(c4);
    
    // wait for module to respond
    if (waitForStartToken() != true)
    {
        mono::error("Failed to recv START_TOKEN for frameRead body.");
        return false;
    }
    
    
    // read and discard any dummy bytes
    // dummy bytes are not 32-bit aligned
    if (frameHeader.dummyBytes > 0) {
        for (int i=0; i<frameHeader.dummyBytes; i++) {
            spi->write(0);
        }
    }
    
    //raed the real data bytes
    if (buffer.length < readLength - frameHeader.dummyBytes)
    {
        mono::error("Module frame read failed: Receive buffer too small!");
        return false;
    }
    
    spi->format(32);
    
    buffer.bytesToRead = readLength - frameHeader.dummyBytes;
    buffer << spi;
    
    spi->format(8);
    
    return true;
}

int ModuleSPICommunication::spiWrite(uint8_t *data, int byteLength, bool thirtyTwoBitMode)
{
    if (thirtyTwoBitMode)
    {
        spi->format(32);
        
        if (byteLength != (byteLength & ~3))
            mono::Warn << "SPI Write: the data buffer is not 4-byte aligned, but transfer mode is 32-bit!";
    }
    
    int spiRead;
    
    if (thirtyTwoBitMode)
    {
        for (int i=0; i<byteLength; i+=4)
        {
            spiRead = spi->write( *( (int*)(data+i) ) );
        }
    }
    else
    {
        for (int i=0; i<byteLength; i++)
        {
            spiRead = spi->write( data[i] );
        }
    }
    
    if (thirtyTwoBitMode)
    {
        spiRead = (spiRead >> 24) & 0xFF; // reverse reponse endian for 32-bit
        spi->format(8);
    }
    
    return spiRead;
}


// PUBLIC METHODS

bool ModuleSPICommunication::initializeInterface()
{
    
    
    // Setup init command structure
    CommandC1 cmd;
    cmd.CommandType = CommandC1::INITIALIZATION;
    cmd.ReadWrite = 0;
    cmd.RegisterMemoryAccess = 1;
    cmd.MemoryFrameAccess = 0;
    cmd.TransferLengthSelect = 1;
    cmd.TransferLength = CommandC1::ONE_BYTE;
    
    // convert to command byte
    spiCommandC1 initCmd = cmd.exportByteCommand();
    
    int retval = spi->write(initCmd);
    retval = spi->write(0x00);
    
    if (retval == CMD_SUCCESS)
        return true;
    else if (retval == CMD_FAILURE)
    {
        mono::error("Failed to init SPI interface, module returned failure.");
        return false;
    }
    
    mono::error("Failed to init SPI interface, unknown module response!");
    return false;
}

uint8_t ModuleSPICommunication::readRegister(SpiRegisters reg)
{
    
    CommandC1 cmd1;
    cmd1.CommandType = CommandC1::READ_WRITE;
    cmd1.ReadWrite = false;
    cmd1.RegisterMemoryAccess = false;
    cmd1.MemoryFrameAccess = 0; // ignored
    cmd1.TransferLengthSelect = false;
    cmd1.TransferLength = CommandC1::ONE_BYTE;
    
    CommandC2 cmd2;
    cmd2.DataGranularity = CommandC2::EIGHT_BITMODE;
    cmd2.RegisterSelect = reg;
    
    int retval = sendC1C2(cmd1, cmd2);
    
    if (retval != CMD_SUCCESS)
    {
        mono::error("Failed to sent ReadRegister command to module!");
        return 0;
    }
    
    retval = waitForStartToken();
    
    //  we receive start token?
    if (retval == true)
        retval =  spi->write(0x00);
    else
        mono::error("Register read failed");
    
    return retval;
}

bool ModuleSPICommunication::pollInputQueue()
{
    uint8_t regval = readRegister(SPI_HOST_INTR);
    if ((regval & 0x08) == 0x08)
    {
        return true;
    }
    else
        return false;
}

bool ModuleSPICommunication::readManagementFrame(ManagementFrame &frame)
{
    // get the size of the incoming frame
    frameDescriptorHeader frmHead;
    bool success = readFrameDescriptorHeader(&frmHead);
    
    if (!success)
    {
        mono::error("Failed to read FrameDescriptor Header from input");
        return false;
    }
    
    //alloc memory for incoming frame
    SPIReceiveDataBuffer buffer(frmHead);
    
    success = readFrameBody(frmHead, buffer);
    
    if (!success)
    {
        mono::error("Failed to  read frame body from input");
        return false;
    }
    
    if (!bufferIsMgmtFrame(buffer))
    {
        mono::error("Frame is not a management frame!");
        return false;
    }
    
    mgmtFrameRaw *rawFrame = (mgmtFrameRaw*) buffer.buffer;
    frame = ManagementFrame(rawFrame);
    
    return true;
}

bool ModuleSPICommunication::readManagementFrameResponse(ManagementFrame &request)
{
    // get the size of the incoming frame
    frameDescriptorHeader frmHead;
    bool success = readFrameDescriptorHeader(&frmHead);
    
    if (!success)
    {
        mono::error("Failed to read FrameDescriptor Header from input\n");
        return false;
    }
    
    //alloc memory for incoming frame
    SPIReceiveDataBuffer buffer(frmHead);
    
    success = readFrameBody(frmHead, buffer);
    
    if (!success)
    {
        mono::error("Failed to read frame body from input\n");
        return false;
    }
    
    if (!bufferIsMgmtFrame(buffer))
    {
        mono::error("Frame is not a management frame!\n");
        return false;
    }
    
    mgmtFrameRaw *rawFrame = (mgmtFrameRaw*) buffer.buffer;
    
    //check that the frame is a correct response to the request
    if (rawFrame->CommandId != request.commandId)
    {
        mono::Error << "Read frame response. Wrong resp command Id. Was " << rawFrame->CommandId << " expected " << request.commandId << "\n";
        return false;
    }
    
    if (rawFrame->status != 0)
    {
        mono::Warn << "Frame response for command " << rawFrame->CommandId << " wa not 0, but " << rawFrame->status << "\n";
    }
    
    // check for payload
    if (request.responsePayload && (rawFrame->LengthType & 0xFFF) > 0)
    {
        mono::Debug << "Parsing response frame payload data...\n";
        request.responsePayloadHandler(((uint8_t*)rawFrame)+16);
    }
    else if (request.responsePayload)
    {
        mono::Error << "command frame request expected a response payload, but response is empty!\n";
        return false;
    }
    
    request.length = rawFrame->LengthType & 0xFFF;
    request.direction = ModuleFrame::RX_FRAME;
    
    return true;
}

bool ModuleSPICommunication::writeFrame(ManagementFrame *frame)
{
    //see if there is room in the module input buffer
    uint8_t regval = readRegister(SPI_HOST_INTR);
    
    if (regval != 0)
    {
        mono::Error << "Cannot write frame to module, input buffer is full!";
        return false;
    }
    
    mgmtFrameRaw rawFrame;
    frame->rawFrameFormat(&rawFrame);
    
    // Write the command / mgmt frame
    
    CommandC1 cmd1;
    cmd1.CommandType = CommandC1::READ_WRITE;
    cmd1.ReadWrite = true;
    cmd1.RegisterMemoryAccess = true;
    cmd1.MemoryFrameAccess = true;
    cmd1.TransferLengthSelect = true;
    cmd1.TransferLength = CommandC1::FOUR_BYTES; // ignored
    
    CommandC2 c2;
    c2.DataGranularity = CommandC2::EIGHT_BITMODE;
    c2.RegisterSelect = 0; // ignored
    
    int statusCode = sendC1C2(cmd1, c2);
    //int statusCode = sendC1C2(0x7c, 0x40);
    
    if (statusCode != CMD_SUCCESS)
    {
        mono::Error << "Failed to write frame to module, error status: " << statusCode;
        return false;
    }
    
    spiCommandC3 c3 = frame->size & 0xFF; // lower byte
    spiCommandC4 c4 = 0; // upper byte
    
    // send write-length
    statusCode = spi->write(c3);
    statusCode = spi->write(c4);
    
    if (statusCode != CMD_SUCCESS)
    {
        mono::Error << "Failed to send length of frame to module, got response: " << statusCode;
        return false;
    }
    
    // write the mgmt/cmd frame
    statusCode = spiWrite((uint8_t*)&rawFrame, sizeof(rawFrame));
    
    
    if (frame->payloadLength() <= 0)
    {
        return true;
    }
    
    // write the data frame with payload
    // the size of 300 comes from redpines own source code
    // but this size might be too large for embedded memory sizes
    uint8_t payloadBuffer[300];
    memset(payloadBuffer, 0, 300);
    
    if (frame->payloadLength() > 300)
    {
        mono::Error << "Frame payload data is too large! More than 300 bytes!";
        return false;
    }
    
    frame->dataPayload(payloadBuffer);
    
    bool success = writePayloadData(payloadBuffer, frame->payloadLength());
    
    if (!success)
    {
        return false;
    }
    
    return true;
}

bool ModuleSPICommunication::writePayloadData(uint8_t *data, uint16_t byteLength)
{
    if (byteLength != (byteLength & ~3))
    {
        mono::Error << "Data Frame payload data is not 4-byte aligned!";
        return false;
    }
    
    CommandC1 cmd1;
    cmd1.CommandType = CommandC1::READ_WRITE;
    cmd1.ReadWrite = true;
    cmd1.RegisterMemoryAccess = true;
    cmd1.MemoryFrameAccess = true;
    cmd1.TransferLengthSelect = true;
    cmd1.TransferLength = CommandC1::FOUR_BYTES; // ignored
    
    CommandC2 c2;
    c2.DataGranularity = CommandC2::EIGHT_BITMODE;
    c2.RegisterSelect = 0; // ignored
    
    int statusCode = sendC1C2(cmd1, c2);

    if (statusCode != CMD_SUCCESS)
    {
        mono::Error << "Failed to write data to module, error status: " << statusCode;
        return false;
    }
    
    spiCommandC3 c3 = byteLength & 0xFF; // lower byte
    spiCommandC4 c4 = (byteLength & 0xFF00) >> 8; // upper byte
    
    // send write-length
    statusCode = spi->write(c3);
    statusCode = spi->write(c4);
    
    if (statusCode != CMD_SUCCESS)
    {
        mono::Error << "Failed to send length of data to module, got response: " << statusCode;
        return false;
    }
    
    // write the data
    statusCode = spiWrite(data, byteLength);
    
    if (statusCode != CMD_SUCCESS)
    {
        mono::Error << "Failed to transfer data frame payload data, got response: " << statusCode;
        return false;
    }
    
    return true;
}














