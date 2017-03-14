// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef __mono_redpine__module_frames__
#define __mono_redpine__module_frames__

#include <mbed.h>
#include <queue.h>

namespace mono { namespace redpine {

    /**
     * Management Frame structure as sent from/to the module
     * This frame type deliver commands and status information to and from
     * the module.
     */
    typedef struct __attribute__((packed))
    {
        uint16_t LengthType;    /**< LSB is the frame data length, MSB is always 0x40 */
        uint16_t CommandId;     /**< LSB is the frames Command Id */
        uint8_t  reserved[8];   /**< Reserved for future use */
        uint16_t status;        /**< An integer status of the frame */
        uint8_t  reserved2[2];  /**< Reserved for future use */
    } mgmtFrameRaw;
    
    /**
     * Data Frame for transferring data related to a command, as sent to/from
     * the module.
     *
     * The first 16-bit word is the length bit 0-11, and the frame type
     * bit 12-15.
     */
    typedef struct __attribute__((packed))
    {
        uint16_t LengthType;    /**< LSB is the frame data length, MSB is always 0x50 */
        uint8_t  DataType;      /**< Always 0x00 */
        uint8_t  reserved[13];  /**< Reserved for future use */
    } dataFrameRaw;
    

    typedef union RawFrameHeaderU {
            uint16_t LengthType;    /**< LSB is the frame data length, MSB is always 0x40 on mgmt and 0x50 on data frames */
            mgmtFrameRaw MgmtFrame;
            dataFrameRaw DataFrame;
    } RawFrameHeader;


    /**
     * A generic frame that is used to communicate which the module
     * Subclasses of this will represent data or management frames.
     */
    class ModuleFrame : public IQueueItem
    {
    public:
        /** List of frame direction, sent og received */
        enum FrameDirection
        {
            TX_FRAME,   /**< The frame is a request to the module */
            RX_FRAME    /**< The frame is a response from module */
        };
        
        /** List of request commands id's */
        enum RxTxCommandIds
        {
            SendData            = 0x00, /**< Frame is data frame */
            SetOperatingMode    = 0x10, /**<  */
            Band                = 0x11, /**<  */
            Init                = 0x12, /**<  */
            Scan                = 0x13, /**<  */
            Join                = 0x14, /**<  */
            PowerSaveMode       = 0x15, /**<  */
            SleepTimer          = 0x16, /**<  */
            SetMacAddress       = 0x17, /**<  */
            QueryNetworkParams  = 0x18, /**<  */
            Disconnect          = 0x19, /**<  */
            AntennaSelect       = 0x1B, /**<  */
            SoftReset           = 0x1C, /**<  */
            SetRegion           = 0x1D, /**<  */
            ConfigSave          = 0x20, /**<  */
            ConfigEnable        = 0x21, /**<  */
            ConfigGet           = 0x22, /**<  */
            UserStoreConfig     = 0x23, /**<  */
            APConfig            = 0x24, /**<  */
            SetWEPKeys          = 0x25, /**<  */
            DebugPrintUART2     = 0x26, /**<  */
            PingCommand         = 0x29, /**<  */
            RSSIQuery           = 0x3A, /**<  */
            MulticastAddrFilter = 0x40, /**<  */
            SetIPParameters     = 0x41, /**<  */
            SocketCreate        = 0x42, /**<  */
            SocketClose         = 0x43, /**<  */
            DnsResolution       = 0x44, /**< DNS Resolution lookup command */
            QueryFirmware       = 0x49, /**< Query the modules firmware version */
            HttpGet             = 0x51, /**< HTTP Client, GET request */
            HttpPost            = 0x52, /**< HTTP Client, POST request */
            WakeFromSleep       = 0xCD,
            PowerSaveACK        = 0xDE
        };
        
        /** List of response command Id's */
        enum RxCommandIds
        {
            AsyncConnAcceptReq  = 0x30, /**< Async connection accept request from remote wfd device */
            CardReady           = 0x89 /**< Card Ready command, when module is initialized and ready */
        };
        
        /** Indicate if the frame is request or response */
        FrameDirection direction;
        
        /** This is the payload data length */
        uint16_t length;
        
        /** Either a RxTxCommandId or a TxCommand id */
        uint8_t commandId;
        
        /** The size of frame in raw format */
        static const uint8_t size = sizeof(mgmtFrameRaw);
        
        ///** If this object sits in a queue, this is the pointer the next in the queue */
        //ModuleFrame *queueNextPointer;
        
        /**
         * Construct an empty frame with no properties set.
         * The frame will not represent any data or any command
         */
        ModuleFrame();
        
        /**
         * Dealloc the frame if its subclass contains resources that should
         * be removed gracefully.
         */
        virtual ~ModuleFrame();
        
    };
    
    
    class ManagementFrame : public ModuleFrame
    {
    public:
        
        /**
         * The datastructure provided by the Management Frames async completion
         * handler.
         *
         * You should use the member variable `Context` to reference any frame
         * response data. You should expect the frame to be deallocated right after
         * you callback handler functions returns.
         *
         * **Note**: You type safety you can check the frames `CommandId` before
         * downcasting the `Context` pointer to a specific frame subclass.
         */
        typedef struct {
            bool Success;               /**< See if the request was successfull */
            ManagementFrame *Context;   /**< A pointer to the request frame object */
        } FrameCompletionData;
        
        /** Management response status */
        uint16_t status;
        
        /** The length of this frames payload data, differs for every subclass. */
        //int dataPayloadLength;
        
        /**
         * Frames can handle responses from the module themselves
         * This `bool` indicates that a frame subclass can handle a resposne 
         * frame and data payload.
         *
         * Normally the (@ref commit) method handles and parses a frames response,
         * but when there is a payload, it needs a _response payload handler_
         *
         * This you set this property to `true` in your subclass, when you must
         * overload the method (@ref responsePayloadHandler).
         *
         * The default value for this property is `false`
         * @default `false`
         */
        bool responsePayload;
        
        /**
         * @brief As long as this property is `false` the frame will continue 
         * to process reponses.
         *
         * Some commands gets multiple reponses from the module.
         * Say you use the HTTP GET command or a socket receive command, 
         * the received data arrive in chunks of different lengths.
         *
         * Set this protoperty to `false` and the @ref commit method will know,
         * it whould parse multiple responses for the command.
         *
         * If you set this property to `false`, the method 
         * @ref reposnePayloadHandler will be called multiple times. 
         *
         * Finally, when you know the module will not sent any further response 
         * frames, you must indicate this by setting this property to `true`
         *
         * If a command frame takes only one response, your subclass can ignore
         * this property.
         *
         * @default `true`
         */
        bool lastResponseParsed;
        
        
        /**
         * The module handle class (@ref Module) should dealloc this object when
         * it has parsed the last chunk of response data.
         */
        bool autoReleaseWhenParsed;
        
        /**
         * @brief The handler called when @ref commit finishes
         * 
         * To handle frame request and responses asynchronously, this handler
         * is triggered when the frame is committed.
         */
        mbed::FunctionPointerArg1<void, FrameCompletionData*> completionHandler;
        
        /** A reference to the completion handler context object */
        void *handlerContextObject;
        
        /**
         * Construct an empty (uninitialized) management frame
         * This is used when you need to allocate memory on the stack for a
         * frame, and pass it to another function.
         */
        ManagementFrame();
        
        /**
         * Construct a management frame from reponse data
         *
         * The contructed object will not reference the raw data in any way.
         *
         * @param rawFrame A pointer to the raw frame data structure
         */
        ManagementFrame(mgmtFrameRaw *rawFrame);
        
        /**
         * Construct a outgoing management frame, having the TX_FRAME direction
         * parameter set.
         * No other frame parameters are initailzed
         *
         * @param commandId The TX command id
         */
        ManagementFrame(RxTxCommandIds commandId);
        
        ManagementFrame(const ManagementFrame &other);
        
        ManagementFrame &operator=(const ManagementFrame &other);
        
        /**
         * If this frame is of type TX_FRAME this method will sent it to the
         * module.
         *
         * When the command frame is sent, the method will wait for a response
         * and then check the response against the request. If the property
         * @ref responsePayload is set to `true`, the method @ref responsePayloadHandler
         * is called automatically.
         *
         * If the command takes multiple responses, like TCP data receive on 
         * sockets, you should see the @ref lastResponseParsed property.
         *
         * When the method returns, the frame response (with payload data) is
         * expected to be present.
         *
         * @return `true` on success, false otherwise
         */
        virtual bool commit();
        
        /**
         * @brief Send a TX_FRAME to the module asynchronous
         * 
         * Same as @ref commit, but but asynchronous and return immediately. You
         * should set the completion callback handler (@ref setCompletionCallback)
         * before calling this method.
         *
         *
         *
         * @see commit
         * @see setCompletionCallback
         */
        virtual void commitAsync();
        
        /**
         * If the frame is pending, it is aborted and removed from the to-be-sent
         * request queue.
         * If the frame has already been sent to the module, the abort is ignored.
         *
         * Still the completion callback handler if removed, to avoid calling
         * freed objects.
         *
         * @brief Abort the execution (commit) of the frame
         */
        virtual void abort();
        
        /**
         * Internal method used by @ref commitAsync method to send the frame to
         * the module, inside the async function handler.
         *
         * You should not call this directly.
         */
        virtual bool writeFrame();
        
        /**
         * Internal method to trigger the completion handler callback - if any
         * This method should only be used by the @ref Module member method
         * `moduleEventHandler`.
         */
        virtual void triggerCompletionHandler();
        
        /**
         * Gets the frames raw data format for transfer via the communication
         * channel.
         * This format is only the frame itself, not any data payload
         *
         * @param data A pointer to the raw frame structure were the data is written to.
         * @return raw data struct
         */
        virtual void rawFrameFormat(mgmtFrameRaw *data);
        
        /**
         * Get the byte length of the data payload for this management frame
         * The payload length varys for every subclass.
         * @return payload data byte length
         */
        virtual int payloadLength();
        
        /**
         * Write the payload data for the frame into the provided buffer.
         * The payload data is dependent on the specific management frame 
         * subclass.
         *
         * If the managament frame subclass does not have any payload data, this
         * method not do anything. The target buffer will be left untouched. 
         * Before you call this method, you should check if payload data exists
         * using the [payloadLength](@ref ManagementFrame::payloadLength) method.
         *
         * The data returned to ready to be transferred to the module, it is
         * 4-byte aligned as required. It is you responsibility to ensure the
         * provided data buffer is large enought to contain the needed bytes.
         * 
         * @param dataBuffer A pointer to the target buffer, where the payload data is written.
         */
        virtual void dataPayload(uint8_t *dataBuffer);
        
        /**
         * When frames need to parse or react to response payloads, there are 
         * received after a call to @ref commit.
         *
         * You must overload this method in your subclass and handle the parsing
         * of the response payload. This parsing should set object properties 
         * that can later be accessed by outside objects.
         *
         * This mehod is called internally by the @ref commit method, only if
         * property @ref responsePayload is set to `true`.
         * 
         * @param payloadBuffer A pointer to the raw payload data buffer
         */
        virtual void responsePayloadHandler(uint8_t *payloadBuffer);
        
        /**
         * @brief Set the frame completion callback handler
         * 
         * The member function you provide is called when the frame is successfully
         * committed. This means it has been sent to the module, and a response has
         * been received.
         *
         * The callback function must accept an input parameter:
         * @code
         * void functionName(ManagementFrame::FrameCompletionData *);
         * @endcode
         *
         * @param obj The member functions context pointer (the `this` pointer)
         * @param memPtr A pointer to the member function on the class
         */
        template <typename Owner>
        void setCompletionCallback(Owner *obj, void(Owner::*memPtr)(FrameCompletionData*))
        {
            completionHandler.attach<Owner>(obj, memPtr);
            handlerContextObject = obj;
        }
    };
    
}}

#endif /* defined(__mono_redpine__module_frames__) */
