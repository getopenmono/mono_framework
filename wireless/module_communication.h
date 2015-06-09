//
//  module_communication.h
//
//  Created by Kristoffer Andersen on 27/05/15.
//
//

#ifndef __spiTest__module_communication__
#define __spiTest__module_communication__

#include <SPI.h>
#include "spi_commands.h"
#include "module_frames.h"

namespace mono { namespace redpine {
    
    /**
     * This class encapsulates a receive buffer for use when receiving frames or
     * memory.
     * It provides the raw byte storage and properties like length and test
     * functions.
     * This class should be subclassed to fit specific communication
     * implementations.
     */
    class DataReceiveBuffer
    {
    public:
        
        /** The pointer to the buffer */
        uint8_t *buffer;
        
        /** The length of the databuffer */
        int length;
        
        /**
         * Indicate how many bytes this buffer should contain
         * This can be used to pipe data from a communication interface into
         * the buffer.
         * The buffer should then use this variable to determine how many bytes
         * to read.
         */
        int bytesToRead;
    };
    
    
    /**
     * Class that represent a SPI receive data buffer.
     * This class can be used to hold data frames or raw memory read from the
     * modules SPI interface .
     */
    class SPIReceiveDataBuffer : public DataReceiveBuffer
    {
    public:
        
        /** Whould free memory buffer on dealloc */
        bool ownsMemory;
        
        /**
         * Create a new buffer with a fixed length.
         * This new buffer will allocate the memory on the heap
         * @param size The number og bytes to allocate on heap
         */
        SPIReceiveDataBuffer(int size);
        
        /**
         * Create a new buffer with fixed, based on an existing Frame Descriptor
         * Header.
         * @param frmHead The frame desc header structure
         */
        SPIReceiveDataBuffer(frameDescriptorHeader &frmHead);
        
        /**
         * Pipe data from SPI to the buffer
         * The number of bytes read, is based on the property
         * bytesToRead.
         * @param spi The interface to read from
         * @return A reference to itself, to enable chaining.
         */
        SPIReceiveDataBuffer& operator<<(mbed::SPI *spi);
        
        ~SPIReceiveDataBuffer();
    };
    
    
    /**
     * Abstract class that defines redpine module communication
     * through a hardware interface.
     * Subclasses of this should utilize UART, USB or SPI communication.
     */
    class ModuleCommunication
    {
    public:
        
        /**
         * Send a interface initialization command to
         * the module.
         */
        virtual bool initializeInterface() = 0;
        
        
        /**
         * Test the data inside a buffer to see if it is a management frame
         * @param buffer The buffer to test
         */
        virtual bool bufferIsMgmtFrame(DataReceiveBuffer &buffer);
        
        /**
         * Test if a buffer is a data Frame
         *
         */
        virtual bool bufferIsDataFrame(DataReceiveBuffer &buffer);
        
        /**
         * Polls the module for waiting input data. This can be frame responses
         * or any other data, waiting to be read by us.
         *
         * The method uses the current communication interface method to wait
         * for pending input data. This function blocks, until data is ready.
         *
         * This function can be used it interrupts are not applicable
         * @return `true` if there is data to read, `false` otherwise
         */
        virtual bool pollInputQueue() = 0;
        
        /**
         * Read the first available frame from the modules input queue
         * This function should be called when you are sure there is data pending
         *
         * @param frame A reference to management frame placeholder object
         * @return True on success, false otherwise
         */
        virtual bool readManagementFrame(ManagementFrame &frame) = 0;
        
        /**
         * Read a pending frame from the module, and interpret it as a response
         * to an earlier frame.
         *
         * The response frame (the one that is read) and the provided frame 
         * (the request), are compared by commandId and the response status is
         * validated.
         *
         * This there is any response payload data, this data is passed to the
         * request frames @ref ManagementFrame::responsePayloadHandler function.
         *
         * When this method returns the request frame object is converted to an
         * response frame (@ref RX_FRAME) with status and any payload data.
         *
         * @param request A reference to the request frame, that is awaiting a response
         * @return `true` on success, `false` otherwise.
         */
        virtual bool readManagementFrameResponse(ManagementFrame &request) = 0;
        
        /**
         * Send a frame to the module
         * @param frame A pointer to the frame to send
         * @return true on success, false otherwise
         */
        virtual bool writeFrame(ManagementFrame *frame) = 0;
        
        /**
         * Sends a frame's payload data to the module.
         *
         * This payload data must be sent after the command / management frame
         * has been sent to the module.
         *
         * This format of the data is determined by which command that was sent
         * by the Command/Management frame. The RS9113 Software PRM is not 
         * clear on this data frame matter. But this code is from reverse 
         * engineering the example code by RSI.
         *
         * @param data A pointer to the raw data to write to the module
         * @param byteLength The length of the data in bytes
         * @return `true` upon success, `false` otherwise.
         */
        virtual bool writePayloadData(uint8_t *data, uint16_t byteLength) = 0;
    };
    
    
    
    
    
    /**
     * SPI based communication subclass
     */
    class ModuleSPICommunication : public ModuleCommunication
    {
    public:
        
        /** List of available SPI Registers on RS9113 */
        enum SpiRegisters
        {
            SPI_HOST_INTR   /**< SPI Interrupt occured register */
        };
        
        
    protected:
        mbed::SPI *spi;
        
        /**
         * Auxillary function to transfer C1 and C2 commands
         *
         * @param c1 The C1 command byte, the SPI reponse here is ignored
         * @param c2 The C2 command byte, the response for this byte is the cmd status
         * @return The Redpine command status code (0x58 is success)
         */
        CommandStatus sendC1C2(spiCommandC1 c1, spiCommandC2 c2);
        
        /**
         * Auxillary function to transfer C1 and C2 commands
         *
         * @param c1 The C1 command object, the SPI reponse here is ignored
         * @param c2 The C2 command object, the response for this byte is the cmd status
         * @return The Redpine command status code (0x58 is success)
         */
        CommandStatus sendC1C2(CommandC1 c1, CommandC2 c2);
        
        /**
         * Auxillary function to poll module for a start token
         * Transfers zeros to the module until some other than zeroes are received
         *
         * @param thirtyTwoBitMode Defaults to false, set TRUE for 32-bit mode SPI
         * @return True is module send a START_TOKEN command, false on timeout or unknown response.
         */
        bool waitForStartToken(bool thirtyTwoBitMode = false);
        
        /**
         * Reads the 4 bytes of a data og management frame header.
         *
         * The module sends a 4 byte header before sending the 16-byte
         * frame descriptor.
         * Used to query a pending frame for its length.
         *
         * @param buffer A pointer to where the length is stored
         * @return `true` is success, `false` otherwise
         */
        bool readFrameDescriptorHeader(frameDescriptorHeader *buffer);
        
        /**
         * Read a pending frame, based on a frame descriptor header
         * (describing the length), read the real frame - data og mgmt.
         *
         * @param frameHeader Reference to the header for the pending frame
         * @param buffer A reference to the buffer where the frame is to be stored
         * @return `true` on success, `false` otherwise
         */
        bool readFrameBody(frameDescriptorHeader &frameHeader, SPIReceiveDataBuffer &buffer);
        
        /**
         * Convenience function to write data to mbed SPI
         * The method only return the last received input from SPI, all other 
         * read bytes are discarded.
         *
         * Use this function if you need to write multiple bytes at once. The 
         * method can transfer the data as 32-bit values if you choose.
         *
         * @param data A ponter to the data buffer, that will be written
         * @param byteLength The length of the data to write, in bytes
         * @param thirtyTwoBitFormat Set this to `true` to use 32-bit mode, default is `false`
         * @return the last read value on the SPI bus
         */
        int spiWrite(uint8_t *data, int byteLength, bool thirtyTwoBitFormat = false);
        
    public:
        
        /** 
         * Create a communication class, and assign a SPI hardware interface
         * 
         * @param spi The initialized and ready SPI hardware module
         * @param interruptPin The pin where the modules interrupt signal is connected
         */
        ModuleSPICommunication(mbed::SPI &spi, PinName interruptPin);
        
        bool initializeInterface();
        
        /**
         * Internal function to read content of a SPI register
         * SPI registers are only available to the modules SPI interface
         *
         * @param reg The register to read
         * @return the content of the 8-bit register
         */
        uint8_t readRegister(SpiRegisters reg);
        
        
        bool pollInputQueue();
        
        bool readManagementFrame(ManagementFrame &frame);
        
        bool readManagementFrameResponse(ManagementFrame &request);
        
        bool writeFrame(ManagementFrame *frame);
        
        bool writePayloadData(uint8_t *data, uint16_t byteLength);
    };
    
}}


#endif /* defined(__spiTest__module_communication__) */
