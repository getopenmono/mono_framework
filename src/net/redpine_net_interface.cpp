
#include <redpine_module.h>
#include "redpine_net_interface.h"

using namespace mono::redpine;

RedpineSocketInterface RedpineSocketInterface::current = RedpineSocketInterface();
mono::net::MonoNetInterface* mono::net::MonoNetInterface::CurrentInterface = &RedpineSocketInterface::current;
mono::net::MonoNetInterface::SocketContext* RedpineSocketInterface::activeSockets[RedpineSocketInterface::MaxAllowedSockets];

// MARK: Command methods

void RedpineSocketInterface::Command::frameCompleted(ManagementFrame::FrameCompletionData *data)
{
    if (!data->Success)
    {
        printf("Command (Id: 0x%X) failed with error: 0x%X\r\n", data->Context->commandId, data->Context->status);
    }

    delete frame;
    delete this;
}

void RedpineSocketInterface::Command::openFrameResponse(const OpenSocketFrame::socketFrameRcv *recv)
{
    if (cnxt != 0)
    {
        RedpineSocketInterface::activeSockets[recv->socketDescriptor - 1] = cnxt;
        cnxt->_onCreate(recv->socketDescriptor, recv->moduleSocket);
    }
}

// MARK: Redpine interface methods

RedpineSocketInterface::RedpineSocketInterface()
{
    dataFrameHandler.attach<RedpineSocketInterface>(this, &RedpineSocketInterface::handleDataFrames);

    Module::Instance()->defaultDataFramePayloadHandler = &dataFrameHandler;

    memset(activeSockets, 0, sizeof(SocketContext*)*MaxAllowedSockets);
}

void RedpineSocketInterface::createClientSocket(SocketContext *cnxt,
                                                uint8_t ip[], uint16_t destPort,
                                                uint16_t localPort, bool isUdp)
{
    Command *cmd = new Command();
    cmd->cnxt = cnxt;

    OpenSocketFrame *openFrm = new OpenSocketFrame(OpenSocketFrame::TCP_SSL_CLIENT, ip, localPort, destPort);
    openFrm->setCompletionCallback<Command>(cmd, &Command::frameCompleted);
    openFrm->createdHandler.attach<Command>(cmd, &Command::openFrameResponse);
    cmd->frame = openFrm;

    openFrm->commitAsync();
}

void RedpineSocketInterface::createServerSocket(uint16_t port, bool isUdp)
{

}

void RedpineSocketInterface::handleDataFrames(ModuleCommunication::DataPayload const &payload)
{
    OpenSocketFrame::recvFrameTcp *dataPayload = (OpenSocketFrame::recvFrameTcp*) payload.data;
    handleIncomingData(dataPayload->recvDataBuf, dataPayload->recvBufLen,
                       dataPayload->recvSocket, dataPayload->fromIPaddr.ipv4_address,
                       dataPayload->fromPortNum);
}

void RedpineSocketInterface::handleIncomingData(const char *data, uint32_t length,
                                                uint32_t sockDesc, uint8_t fromIp[],
                                                uint16_t fromPort)
{
    if (sockDesc < 1)
    {
        debug("Redpine module provided socket descriptor less than 1, ignoring!\r\n");
        return;
    }
    if (RedpineSocketInterface::activeSockets[sockDesc - 1] != 0)
    {
        SocketContext *socket = RedpineSocketInterface::activeSockets[sockDesc - 1];
        socket->_onData(data, length, fromIp, fromPort);
    }
    else
    {
        debug("non existing socket %d!\r\n", sockDesc);
    }
}

void RedpineSocketInterface::handleConnectEvent(uint32_t sockDesc)
{

}

void RedpineSocketInterface::handleDisconnectEvent()
{

}

bool RedpineSocketInterface::writeData(const char *data, uint32_t length, uint32_t sockDesc, uint16_t destPort, bool isUdp)
{
    OpenSocketFrame::rsi_frameSend frame;
    memset(&frame, 0, sizeof(OpenSocketFrame::rsi_frameSend));

    if (isUdp)
    {

    }
    else
    {
        frame.ip_version = 4;
        frame.socketDescriptor = sockDesc;
        frame.sendBufLen = length;
        memcpy(frame.sendDataBuf, data, length);
        frame.destPort = destPort;
    }

    return Module::Instance()->sendDataFrame((char*)&frame, length);
}
