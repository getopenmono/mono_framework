//
//  spi_commands.h
//  Mono::Redpine
//
//  Created by Kristoffer Andersen on 26/05/15.
//
//

#ifndef spiTest_spi_commands_h
#define spiTest_spi_commands_h

#include "stdint.h"

namespace mono { namespace redpine
{
    /**
     * Enum with the modules possible command responses
     * Sent at the end of module receiving the command
     */
    enum CommandStatus
    {
        CMD_SUCCESS = 0x58, /**< Command succeeded */
        CMD_FAILURE = 0x52, /**< Command failed */
        START_TOKEN = 0x55, /**< Read request is received, the module is ready to send data */
        BUSY_RESPONSE = 0x54/**< New transaction is initiated, while the previous transaction is still pending */
    };
    
    /**
     * SPI Command byte C1
     * See RS9113 Software PRM, p 13
     */
    typedef uint8_t spiCommandC1;
    
    /**
     * SPI Commands byte C2
     * See RS9113 Software PRM, p 14
     */
    typedef uint8_t spiCommandC2;
    
    /**
     * SPI Commands byte C3
     * LSB of the transfer’s length (which is in terms of bytes) in case bit 2
     * of C1 is set.
     *
     * This command is skipped if bit 2 of C1 is cleared.
     */
    typedef uint8_t spiCommandC3;
    
    /**
     * SPI Commands byte C4
     * MSB of the transfer Length (15:8)
     * (Which is in terms of bytes) in case bit 2 of C1 is set.
     *
     * This command is skipped if bit 2 of C1 is cleared
     */
    typedef uint8_t spiCommandC4;
    
    /**
     * C1 SPI Command structure, used to create C1 command bytes easy
     * Configure the command properties here, and convert it a byte format
     * before sending via SPI.
     */
    class CommandC1
    {
    public:
        /**
         * Command types for C1 commands
         */
        enum C1CommandTypes
        {
            INITIALIZATION = 0x00,
            READ_WRITE = 0x01
        };
        
        /** This C1 commands type */
        C1CommandTypes CommandType;
        
        /**
         * The command is a read or a write
         * False is read, True is Write
         */
        bool ReadWrite;
        
        /**
         * Determines if this command read/write a register or memory/frames
         * True is Memory R/W, False is Register R/W
         */
        bool RegisterMemoryAccess;
        
        /**
         * Determines if command is Memory or Frame R/W
         * True is Frame R/W, False is Memory R/W
         */
        bool MemoryFrameAccess;
        
        /**
         * Determines the later C3 and C4 commands
         * False is 2-bit transfer length, True is 16-bit transfer length
         */
        bool TransferLengthSelect;
        
        /** The possible 2-bit transfer lengths */
        enum TransferLengths
        {
            FOUR_BYTES  = 0x00,
            THREE_BYTES = 0x03,
            TWO_BYTES   = 0x02,
            ONE_BYTE    = 0x01
        };
        
        /**
         * If TransferLengthSelect is False, this value determines the
         * data length.
         */
        TransferLengths TransferLength;
        
        /**
         * Convert this structure to the compact byte format
         */
        spiCommandC1 exportByteCommand();
    };
    
    /**
     * C2 SPI Command structure, used to create C2 command bytes easy
     * Configure the command properties here, and convert it a byte format
     * before sending via SPI.
     */
    class CommandC2
    {
    public:
        /** The possible data bit-modes for SPI data transfers*/
        enum DataModes
        {
            EIGHT_BITMODE       = 0x00,
            THIRTYTWO_BITMODE   = 0x01
        };
        
        /**
         * Determine the bit-mode of the data transfer.
         * 
         * The SPI (C1, C2, C3, C4) commands and addresses (A1, A2, A3, A4) 
         * will always be 8-bit irrespective of this value.
         */
        DataModes DataGranularity;
        
        /**
         * If C1 is register access type, this indicate the register address.
         * Else ignored.
         */
        uint8_t RegisterSelect;
        
        /**
         * Convert this C2 structure to the compact byte format
         */
        spiCommandC2 exportByteCommand();
    };
    
    
    /**
     * Before you can read a mgmt/data frame, you must read this header structure
     * The existence of this header is not documented by redpine signals
     */
    typedef struct
    {
        uint16_t dummyBytes; /**< bytes that can be discarded from read input stream */
        uint16_t totalBytes; /**< number of bytes to read */
    } frameDescriptorHeader;
    
}
}
#endif
