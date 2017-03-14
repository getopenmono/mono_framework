// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "module_communication.h"
#include <mono.h>
#include <mbed.h>
#include <consoles.h>
#include <application_context_interface.h>

extern "C"
{
#include <project.h>
}

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


SPIReceiveDataBuffer::SPIReceiveDataBuffer(int size, uint16_t fwVer) : DataReceiveBuffer()
{
    this->length = size;
    this->ownsMemory = true;
    this->firmwareVersion = fwVer;

    //mono::warning("SPIReceiveDataBuffer: allocating buffer on HEAP!\r\n");
    this->buffer = (uint8_t*) malloc(size);
    if (buffer == NULL)
    {
        error("HEAP overflow!\r\n");
    }
}

SPIReceiveDataBuffer::SPIReceiveDataBuffer(frameDescriptorHeader &frmHead, uint16_t fwVer)
{
    this->firmwareVersion = fwVer;

//    if (firmwareVersion == 0xab15) {
//        this->length = (frmHead.totalBytes + 3) & (~3); // old protocol 1.5
//    }
//    else
        this->length = ((frmHead.totalBytes - 4) + 3) & (~3); // -4 needed for version 1.6

    this->ownsMemory = true;

    //mono::warning("SPIReceiveDataBuffer: allocating buffer on HEAP!\r\n");
    this->buffer = (uint8_t*) malloc(this->length);
    if (buffer == NULL)
    {
        error("HEAP overflow! Could not alloc: %i bytes\r\n",this->length);
    }

    this->bytesToRead = this->length;
}

SPIReceiveDataBuffer& SPIReceiveDataBuffer::operator<<(mbed::SPI *spi)
{
    if (length < bytesToRead)
    {
        debug("SPIReceiveDataBuffer read error, buffer is too small!\r\n");
        return *this;
    }

    spi->format(32);

    uint32_t buf = 0;
    uint32_t *writePtr = (uint32_t*) this->buffer;

    while (this->bytesToRead > 0)
    {
        buf = spi->write(0);
        mono::defaultSerial.printf("0x%X ", buf);
        //set LSByte on index 0, and MSByte on index 3
        *writePtr = (buf & 0xFF)<<24 | (buf & 0xFF00)<<8 | (buf & 0xFF0000)>>8 | (buf&0xFF000000)>>24;

        bytesToRead -= 4;
        writePtr++; // increment 4 bytes
    }
    mono::defaultSerial.printf("\r\n");
    spi->format(8);

    return *this;
}

SPIReceiveDataBuffer::~SPIReceiveDataBuffer()
{
    if (ownsMemory)
    {
        free(this->buffer);
    }

}

// MARK: Module SPI Communication

ModuleSPICommunication::ModuleSPICommunication(mbed::SPI &spi, PinName chipSelect, PinName resetPin, PinName interruptPin) :
    spiChipSelect(chipSelect, 1),
    resetLine(resetPin, 1),
    spiInterrupt(interruptPin),
    fakeISRTimer(50)
{
    this->spi = &spi;
    this->InterfaceVersion = 0xAB16; // we expect this is the default version

    //spiInterrupt.DeactivateUntilHandled();
    spiInterrupt.rise<ModuleSPICommunication>(this, &ModuleSPICommunication::interruptHandler);

//    defaultSerial.printf("starting fake IRQ timer!\r\n");
//    fakeISRTimer.setCallback<ModuleSPICommunication>(this, &ModuleSPICommunication::fakeISRHandler);
//    fakeISRTimer.Start();

    IApplicationContext::Instance->PowerManager->AppendToPowerAwareQueue(this);
}

// PROTECTED AUXILLARY METHODS

void ModuleSPICommunication::fakeISRHandler()
{
    if (pollInputQueue())
        interruptHandler();
}

void ModuleSPICommunication::interruptHandler()
{
    //defaultSerial.printf("SPI ISR has occured!\r\n");
    if (interruptCallback == true)
    {
        //defaultSerial.printf("calling event handler!\r\n");
        interruptCallback.call();
    }
}

CommandStatus ModuleSPICommunication::sendC1C2(spiCommandC1 c1, spiCommandC2 c2)
{
    int retval = BUSY_RESPONSE;
    int reTries = 0;
    spi->format(8);

    setChipSelect(true);
    while (retval == BUSY_RESPONSE && reTries < 10) {
        retval = spi->write(c1);

        if (retval != 0)
            mono::defaultSerial.printf("Unexpected response for C1 command, not zero but 0x%x!\r\n",retval);

        retval = spi->write(c2);

        if (retval == BUSY_RESPONSE)
            wait_ms(100);
        reTries++;
    }
    setChipSelect(false);

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
    int timeout = 500, to = 0;
    setChipSelect(true);
    while (retval != START_TOKEN && timeout > to)
    {
        retval = spi->write(0x00);
        // 32-bit transfer command status code is big-endian
        // our target is little-endian, compensate for that!
        if (thirtyTwoBitMode && retval == START_TOKEN << 24)
        {
            retval = START_TOKEN;
        }

        if (retval != START_TOKEN)
            wait_us(100);

        to++;
    }
    setChipSelect(false);

    // set mode back to 8 bit
    if (thirtyTwoBitMode)
        spi->format(8);

    //  we receive start token?
    if (retval == START_TOKEN)
        return true;
    else
        mono::defaultSerial.printf("Failed to receive start token\r\n");

    return false;
}

bool ModuleSPICommunication::readFrameHead(RawFrameHeader *rawHeader)
{
    return true;
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
    cmd2.DataGranularity = CommandC2::EIGHT_BITMODE;
    cmd2.RegisterSelect = 0; // ignored

    int status = sendC1C2(cmd1, cmd2);
    if (status != CMD_SUCCESS)
    {
        debug("Failed to fetch frame length header, status: 0x%x!",status);
        return false;
    }

    spiCommandC3 c3 = 0x04; // read 4 bytes of frame
    spiCommandC4 c4 = 0x00;

    setChipSelect(true);
    int retval = spi->write(c3);
    retval = spi->write(c4);
    setChipSelect(false);

    retval = waitForStartToken();

    if (retval != true)
    {
        debug("Failed to fetch frame length header");
        return false;
    }

    //receive frame head 4-bytes
    spi->format(8);

    uint16_t frmDesc[2];
    char *readBuf = (char*) frmDesc;
    setChipSelect(true);
    readBuf[0] = spi->write(0);
    readBuf[1] = spi->write(0);
    readBuf[2] = spi->write(0);
    readBuf[3] = spi->write(0);
    setChipSelect(false);

    buffer->dummyBytes = frmDesc[1] - 4;
    buffer->totalBytes = frmDesc[0];

    return true;
}

bool ModuleSPICommunication::readFrameBody(frameDescriptorHeader &frameHeader, SPIReceiveDataBuffer &buffer)
{
    uint32_t frameLength = frameHeader.totalBytes - 4 - frameHeader.dummyBytes;
    
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
    c2.DataGranularity = CommandC2::EIGHT_BITMODE;
    c2.RegisterSelect = 0; // ignored

    // send the commands to read a frame
    if (sendC1C2(c1,c2) != CMD_SUCCESS)
    {
        mono::defaultSerial.printf("Failed to send frameRead body c1 and c2 command\r\n");
        return false;
    }

    spiCommandC3 c3 = readLength & 0xFF; // lower byte
    spiCommandC4 c4 = (readLength & 0xFF00) >> 8; // upper byte

    //mono::defaultSerial.printf("Frm read, length: %i\r\n",readLength);

    setChipSelect(true);
    // send read-length
    spi->write(c3);
    spi->write(c4);
    setChipSelect(false);

    // wait for module to respond
    if (waitForStartToken() != true)
    {
        mono::defaultSerial.printf("Failed to recv START_TOKEN for frameRead body.\r\n");
        return false;
    }


    // read and discard any dummy bytes
    // dummy bytes are not 32-bit aligned
    if (frameHeader.dummyBytes > 0) {
        printf("reading dummy bytes: ");
        setChipSelect(true);
        for (int i=0; i<frameHeader.dummyBytes; i++) {
            //spi->write(0);
            printf("0x%x ", spi->write(0) );
        }
        setChipSelect(false);
        
        printf("\r\n");
    }

    //raed the real data bytes
    if (buffer.length < readLength)
    {
        mono::defaultSerial.printf("Module frame read failed: Receive buffer too small!\r\n");
        return false;
    }

    buffer.bytesToRead = readLength;

    setChipSelect(true);
    uint8_t *ptr = buffer.buffer;
    while (readLength > ptr - buffer.buffer) {
        *ptr = spi->write(0);
        ptr++;
    }

    setChipSelect(false);

    return true;
}

int ModuleSPICommunication::spiWrite(const char *data, int byteLength, bool thirtyTwoBitMode)
{
    if (thirtyTwoBitMode)
    {
        spi->format(32);

        if (byteLength != (byteLength & ~3))
            mono::Warn << "SPI Write: the data buffer is not 4-byte aligned, but transfer mode is 32-bit!";
    }

    int spiRead = 0;

    setChipSelect(true);

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

    setChipSelect(false);

    return spiRead;
}


void ModuleSPICommunication::setChipSelect(bool active)
{
    //spiChipSelect = !active; // Chip select is active low!
}


// PUBLIC METHODS

void ModuleSPICommunication::resetModule()
{
//    resetLine = 0;
//    wait_ms(100);
//    resetLine = 1;
//    wait_ms(100);
}

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

    setChipSelect(true);
    int retval = spi->write(initCmd);
    retval = spi->write(0x00);
    setChipSelect(false);

    if (retval == CMD_SUCCESS)
        return true;
    else if (retval == CMD_FAILURE)
    {
        mono::defaultSerial.printf("Failed to init SPI interface, module returned failure.\r\n");
        return false;
    }

    mono::defaultSerial.printf("Failed to init SPI interface, unknown module response: 0x%x!\r\n",retval);
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
        debug("Failed to sent ReadRegister command to module!\r\n");
        return 0;
    }

    retval = waitForStartToken();

    //  we receive start token?
    if (retval == true)
    {
        setChipSelect(true);
        retval =  spi->write(0x00);
        setChipSelect(false);
    }
    else
        debug("Register read failed\r\n");

    return retval;
}

uint16 ModuleSPICommunication::readMemory(uint32_t memoryAddress)
{
    CommandC1 cmd1;
    cmd1.CommandType = CommandC1::READ_WRITE;
    cmd1.ReadWrite = false;
    cmd1.RegisterMemoryAccess = true;
    cmd1.MemoryFrameAccess = false; // ignored
    cmd1.TransferLengthSelect = false;
    cmd1.TransferLength = CommandC1::TWO_BYTES;

    CommandC2 cmd2;
    cmd2.DataGranularity = CommandC2::EIGHT_BITMODE;

    int retval = sendC1C2(cmd1, cmd2);

    if (retval != CMD_SUCCESS)
    {
        debug("Failed to sent ReadMemory command to module!");
        return 0;
    }

    //write address
    uint8_t *addr = (uint8_t*) &memoryAddress;
    setChipSelect(true);
    spi->write(addr[0]);
    spi->write(addr[1]);
    spi->write(addr[2]);
    spi->write(addr[3]);
    setChipSelect(false);

    retval = waitForStartToken();

    //  we receive start token?
    if (retval == true)
    {
        uint8_t *retarr = (uint8_t*) &retval;
        retval = 0;
        setChipSelect(true);
        retarr[0] =  spi->write(0x00);
        retarr[1] =  spi->write(0x00);
        setChipSelect(false);
    }
    else
        debug("Memory read failed\r\n");

    return retval;
}

void ModuleSPICommunication::writeMemory(uint32_t memoryAddress, uint16_t value)
{
    CommandC1 cmd1;
    cmd1.CommandType = CommandC1::READ_WRITE;
    cmd1.ReadWrite = true;
    cmd1.RegisterMemoryAccess = true;
    cmd1.MemoryFrameAccess = false; // ignored
    cmd1.TransferLengthSelect = false;
    cmd1.TransferLength = CommandC1::TWO_BYTES;

    CommandC2 cmd2;
    cmd2.DataGranularity = CommandC2::EIGHT_BITMODE;

    int retval = sendC1C2(cmd1, cmd2);

    if (retval != CMD_SUCCESS)
    {
        debug("Failed to sent WriteMemory command to module!\r\n");
        return;
    }

    //write address
    uint8_t *addr = (uint8_t*) &memoryAddress;
    setChipSelect(true);
    spi->write(addr[0]);
    spi->write(addr[1]);
    spi->write(addr[2]);
    retval = spi->write(addr[3]);

    if (retval != CMD_SUCCESS)
    {
        debug("Failed to WriteMemory address to module!\r\n");
        setChipSelect(false);
        return;
    }

    uint8_t *retarr = (uint8_t*) &value;
    // write data
    spi->write(retarr[0]);
    retval = spi->write(retarr[1]);
    setChipSelect(false);

    if (retval != CMD_SUCCESS)
        debug("Failed to WriteMemory value to module!\r\n");
}


bool ModuleSPICommunication::pollInputQueue()
{
    //bool dataReady = spiInterrupt.read();
    uint8_t regval = readRegister(SPI_HOST_INTR);
    if ((regval & 0x08) == 0x08)
    {
//        if (!dataReady)
//            debug("input data ready, but interrupt is low!\r\n");

        return true;
    }
    else
    {
//        if (dataReady)
//            debug("no input data ready, but interrupt is high!\r\n");

        return false;
    }
}

bool ModuleSPICommunication::interruptActive()
{
    return spiInterrupt.read();
}

bool ModuleSPICommunication::readManagementFrame(ManagementFrame &frame)
{
    // get the size of the incoming frame
    frameDescriptorHeader frmHead;
    bool success = readFrameDescriptorHeader(&frmHead);

    if (!success)
    {
        mono::defaultSerial.printf("Failed to read FrameDescriptor Header from input\r\n");
        return false;
    }

    //mono::defaultSerial.printf("frm head: 0x%x dummy, 0x%x total\r\n", frmHead.dummyBytes,frmHead.totalBytes);

    //alloc memory for incoming frame
    SPIReceiveDataBuffer buffer(frmHead, this->InterfaceVersion);

    success = readFrameBody(frmHead, buffer);

    if (!success)
    {
        mono::defaultSerial.printf("Failed to read frame body from input\r\n");
        return false;
    }

    if (!bufferIsMgmtFrame(buffer))
    {
        mono::defaultSerial.printf("Frame is not a management frame!\r\n");
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
        debug("Failed to read FrameDescriptor Header from input\r\n");
        return false;
    }

    //printf("frm head: %i dummy, %i total\n\r", frmHead.dummyBytes,frmHead.totalBytes);

    //alloc memory for incoming frame
    SPIReceiveDataBuffer buffer(frmHead, this->InterfaceVersion);

    success = readFrameBody(frmHead, buffer);

    if (!success)
    {
        debug("Failed to read frame body from input\r\n");
        return false;
    }

    if (!bufferIsMgmtFrame(buffer))
    {
        memdump(buffer.buffer, buffer.length);
        debug("Frame is not a management frame!\r\n");
        return false;
    }


    mgmtFrameRaw *rawFrame = (mgmtFrameRaw*) buffer.buffer;

    //check that the frame is a correct response to the request
    if (rawFrame->CommandId != request.commandId)
    {
        debug("Read frame response. Wrong resp command Id. Was 0x%x expected 0x%x\r\n",rawFrame->CommandId,request.commandId);
        return false;
    }

    if (rawFrame->status != 0)
    {
        debug("Error response for command: 0x%x. Error code: 0x%x\r\n",rawFrame->CommandId,rawFrame->status);
    }
    // check for payload
    else if (request.responsePayload && (rawFrame->LengthType & 0xFFF) > 0)
    {
        //debug("Parsing response frame payload data...\r\n");
        request.responsePayloadHandler(((uint8_t*)rawFrame)+16);
    }
    else if (request.responsePayload)
    {
        debug("command frame request expected a response payload, but response is empty!\r\n");
        return false;
    }

    request.length = rawFrame->LengthType & 0xFFF;
    request.direction = ModuleFrame::RX_FRAME;
    request.status = rawFrame->status;

    return rawFrame->status == 0;
}

bool ModuleSPICommunication::readDataFrame(DataPayloadHandler &payloadHandler)
{
    // get the size of the incoming frame
    frameDescriptorHeader frmHead;
    bool success = readFrameDescriptorHeader(&frmHead);

    if (!success)
    {
        mono::defaultSerial.printf("Failed to read FrameDescriptor Header from input\n\r");
        return false;
    }

    //mono::defaultSerial.printf("frm head: %i dummy, %i total\n\r", frmHead.dummyBytes,frmHead.totalBytes);

    //alloc memory for incoming frame
    SPIReceiveDataBuffer buffer(frmHead, this->InterfaceVersion);

    success = readFrameBody(frmHead, buffer);

    if (!success)
    {
        mono::defaultSerial.printf("Failed to read frame body from input\n\r");
        return false;
    }

    if (!bufferIsDataFrame(buffer))
    {
        mono::defaultSerial.printf("Frame is not a data frame!\n\r");
        memdump(buffer.buffer, buffer.length);
        return false;
    }

    dataFrameRaw *rawFrame = (dataFrameRaw*) buffer.buffer;

    // check for payload
    if ((rawFrame->LengthType & 0xFFF) > 0)
    {
        struct DataPayload payload = { ((uint8_t*)rawFrame)+16, (uint16_t)(rawFrame->LengthType & 0xFFF) };
        payloadHandler.call(payload);
    }
    else
    {
        debug("Data frame with no payload!\t\n");
    }

    return true;
}

bool ModuleSPICommunication::writeRawFrame(const char *rawFrame)
{
    //see if there is room in the module input buffer
    uint8_t regval = readRegister(SPI_HOST_INTR);

    if (regval != 0)
    {
        debug("Cannot write frame to module, input buffer is full!\r\n");
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
        debug("Failed to write frame to module, error status: 0x%x\r\n", statusCode);
        return false;
    }

    spiCommandC3 c3 = 16 & 0xFF; // lower bytes
    spiCommandC4 c4 = 0; // upper byte

    // send write-length
    setChipSelect(true);
    statusCode = spi->write(c3);
    statusCode = spi->write(c4);
    setChipSelect(false);

    if (statusCode != CMD_SUCCESS)
    {
        debug("Failed to send length of frame to module, got response: 0x%x\r\n",statusCode);
        return false;
    }

    // write the mgmt/cmd frame
    statusCode = spiWrite(rawFrame, 16);

    if (statusCode != CMD_SUCCESS)
    {
        debug("Failed to send raw frame to module, got response: 0x%x\r\n",statusCode);
        return false;
    }

    return true;
}

bool ModuleSPICommunication::writeDataFrame(const char *data, uint32_t length)
{
    dataFrameRaw frame;
    memset(&frame, 0, sizeof(dataFrameRaw));
    frame.LengthType = length | (0x50 << 8); // 0x50 for data frame type

    bool success = writeRawFrame((char*)&frame);
    if (!success)
    {
        return false;
    }

    success = writePayloadData(data, length);

    if (!success)
    {
        return false;
    }

    return true;
}

bool ModuleSPICommunication::writeFrame(ManagementFrame *frame)
{
    mgmtFrameRaw rawFrame;
    frame->rawFrameFormat(&rawFrame);

    bool success = writeRawFrame((char*)&rawFrame);

    if (!success)
        return false;

    if (frame->payloadLength() <= 0)
    {
        return true;
    }

    // write the data frame with payload
    // the size of 872 comes from redpines documentation of POST data,
    // but this size might be too large for embedded memory sizes
    char payloadBuffer[872];
    memset(payloadBuffer, 0, 872);

    if (frame->payloadLength() > 872)
    {
        debug("Frame payload data is too large! More than 300 bytes!\r\n");
        return false;
    }

    frame->dataPayload((uint8_t*)payloadBuffer);

    success = writePayloadData(payloadBuffer, frame->payloadLength());

    if (!success)
    {
        return false;
    }

    return true;
}

bool ModuleSPICommunication::writePayloadData(const char *data, uint16_t byteLength)
{
    if (byteLength != (byteLength & ~3))
    {
        debug("Data Frame payload data is not 4-byte aligned!\r\n");
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
        debug("Failed to write data to module, error status: 0x%x\r\n", statusCode);
        return false;
    }

    spiCommandC3 c3 = byteLength & 0xFF; // lower byte
    spiCommandC4 c4 = (byteLength & 0xFF00) >> 8; // upper byte

    // send write-length
    setChipSelect(true);
    statusCode = spi->write(c3);
    statusCode = spi->write(c4);
    setChipSelect(false);

    if (statusCode != CMD_SUCCESS)
    {
        debug("Failed to send length of data to module, got response: 0x%x\r\n", statusCode);
        return false;
    }

    // write the data
    statusCode = spiWrite(data, byteLength);

    if (statusCode != CMD_SUCCESS)
    {
        debug("Failed to transfer data frame payload data, got response: 0x%x\r\n", statusCode);
        return false;
    }

    return true;
}


// MARK: Power Aware Interface

void ModuleSPICommunication::onSystemPowerOnReset()
{
}
void ModuleSPICommunication::onSystemEnterSleep()
{
    spiInterrupt.disable_irq();
}
void ModuleSPICommunication::onSystemWakeFromSleep()
{
    spiInterrupt.enable_irq();
}
void ModuleSPICommunication::OnSystemBatteryLow()
{

}













