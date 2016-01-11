//
//  module_frames.cpp
//  Mono::Redpine
//
//  Created by Kristoffer Andersen on 01/06/15.
//
//

#include "module_frames.h"
#include "redpine_module.h"
#include <consoles.h>
#include <mbed.h>

using namespace mono::redpine;

ModuleFrame::ModuleFrame()
{
    this->_queueNextPointer = NULL;
    this->length = 0;
    this->commandId = 0;
    //this->queueNextPointer = NULL;
}

ModuleFrame::~ModuleFrame()
{
    // if this object exists in a queue - remove it
    if (_queueNextPointer != NULL)
    {
        debug("freeing frame: 0x%x from queues...\n\r",commandId);
        Module *mod = Module::Instance();
        mod->requestFrameQueue.Remove(this);
        if (mod->responseFrameQueue.Remove(this))
        {
            warning("A Redpine response queue frame was freed! The communication will be out of sync!\n\r");
        }
    }
}

ManagementFrame::ManagementFrame() : ModuleFrame()
{
    this->length = 0;
    this->responsePayload = false;
    this->lastResponseParsed = true;
    this->status = 0;
    this->autoReleaseWhenParsed = true;
    this->handlerContextObject = NULL;
}

ManagementFrame::ManagementFrame(mgmtFrameRaw *frame)
{
    this->direction = RX_FRAME;
    this->length = frame->LengthType & 0xFF00 >> 8;
    this->commandId = frame->CommandId & 0xFF;
    this->status = frame->status;
    this->responsePayload = false;
    this->lastResponseParsed = true;
    this->autoReleaseWhenParsed = true;
    this->handlerContextObject = NULL;
}

ManagementFrame::ManagementFrame(RxTxCommandIds commId)
{
    this->direction = TX_FRAME;
    this->commandId = commId;
    this->responsePayload = false;
    this->lastResponseParsed = true;
    this->length = 0;
    this->status = 0;
    this->autoReleaseWhenParsed = true;
    this->handlerContextObject = NULL;
}

ManagementFrame::ManagementFrame(const ManagementFrame &other)
{
    direction = other.direction;
    commandId = other.commandId;
    responsePayload = other.responsePayload;
    lastResponseParsed = other.lastResponseParsed;
    length = other.length;
    status = other.status;
    autoReleaseWhenParsed = other.autoReleaseWhenParsed;
    handlerContextObject = other.handlerContextObject;
    
    completionHandler = other.completionHandler;
}

ManagementFrame &ManagementFrame::operator=(const mono::redpine::ManagementFrame &other)
{
    debug("mgmt frame assign\n\r");
    
    direction = other.direction;
    commandId = other.commandId;
    responsePayload = other.responsePayload;
    lastResponseParsed = other.lastResponseParsed;
    length = other.length;
    status = other.status;
    autoReleaseWhenParsed = other.autoReleaseWhenParsed;
    handlerContextObject = other.handlerContextObject;
    
    completionHandler = other.completionHandler;
    
    return *this;
}

bool ManagementFrame::commit()
{
    if (this->direction != TX_FRAME)
    {
        mono::Error << "You cannot send a RX frame to the module!\n\r";
        return false;
    }
    
    Module *mod = Module::Instance();
    
    bool success = mod->comIntf->writeFrame(this);
    
    if (!success)
    {
        mono::Error << "Failed to send RX frame\n\r";
        return false;
    }
    
    // always execute once, and maybe more: if lastReponseParsed == false
    do
    {
        //wait for response
        int retries = 0;
        while (retries < 20) {
            
            if (mod->comIntf->interruptActive())
                break;
            
            retries++;
            wait_ms(50*retries);
        }
        
        // sum(50*x, x=1..20) = 10,5 secs timeout
        if (retries == 20)
        {
            debug("Response interrupt for frame timed out!\n\r");
            return false;
        }
        
        //mono::Debug << "Got frame response in " << retries << " retries\n\r";
        
        success = mod->comIntf->readManagementFrameResponse(*this);
        
        if (!success)
        {
            warning("Failed to read response frame!\n\r");
            return false;
        }
        
    } while (!this->lastResponseParsed);
    
    
    return true;
}

void ManagementFrame::commitAsync()
{
    if (this->direction != TX_FRAME)
    {
        error("You cannot send a RX frame to the module!\n\r");
        return;
    }
    
    Module *module = Module::Instance();
    
    module->requestFrameQueue.Enqueue(this);
    
    
    Timer::callOnce<Module>(0, module, &Module::moduleEventHandler);
}

void ManagementFrame::abort()
{
    debug("Aborting MGMT Frame: 0x%x\n\r",commandId);
    
    Module *mod = Module::Instance();
    mod->requestFrameQueue.Remove(this);
    
    this->completionHandler.attach<ManagementFrame>(NULL, NULL);
}

bool ManagementFrame::writeFrame()
{
    Module *module = Module::Instance();
    
    return module->comIntf->writeFrame(this);
}

void ManagementFrame::triggerCompletionHandler()
{
    //if (completionHandler == true)
    {
        FrameCompletionData data;
        data.Success = status == 0 ? true : false;
        data.Context = this;
        completionHandler.call(&data);
    }
    
}


void ManagementFrame::rawFrameFormat(mgmtFrameRaw *data)
{
    data->LengthType = this->payloadLength() | (0x40 << 8); // 0x40 for mgmt frame type
    data->CommandId = this->commandId;
    data->status = this->status;
    
    //zero out reserved memeory
    memset(data->reserved, 0, 8);
    memset(data->reserved2, 0, 2);
}


int ManagementFrame::payloadLength()
{
    return this->length;
}


void ManagementFrame::dataPayload(uint8_t *)
{
}


void ManagementFrame::responsePayloadHandler(uint8_t *)
{
}