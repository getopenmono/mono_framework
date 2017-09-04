
#include <redpine_module.h>
#include "redpine_net_interface.h"
#include <consoles.h>

using namespace mono::redpine;

RedpineSocketInterface RedpineSocketInterface::current = RedpineSocketInterface();
mono::net::MonoNetInterface* mono::net::MonoNetInterface::CurrentInterface = &RedpineSocketInterface::current;
mono::net::MonoNetInterface::SocketContext* RedpineSocketInterface::activeSockets[RedpineSocketInterface::MaxAllowedSockets];

// MARK: Command methods

void RedpineSocketInterface::Command::frameCompleted(ManagementFrame::FrameCompletionData *data)
{
    if (!data->Success && cnxt != 0)
    {
        cnxt->_onError(SocketContext::CONNECT_ERROR);
    }

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

void RedpineSocketInterface::Command::closeFrameResponse(const CloseSocketFrame::rsi_rsp_socket_close *recv)
{
    RedpineSocketInterface::activeSockets[recv->socket_id - 1] = 0;

    if (cnxt != 0)
    {
        cnxt->_onClose(recv->socket_id, recv->sent_bytes_count);
    }
}

// MARK: Redpine interface methods

RedpineSocketInterface::RedpineSocketInterface()
{
    dataFrameHandler.attach<RedpineSocketInterface>(this, &RedpineSocketInterface::handleDataFrames);
    asyncFrameHandler.attach<RedpineSocketInterface>(this, &RedpineSocketInterface::handleAsyncMgmtFrames);

    Module::Instance()->defaultDataFramePayloadHandler = &dataFrameHandler;
    Module::Instance()->asyncManagementFrameHandler = &asyncFrameHandler;

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
    openFrm->autoReleaseWhenParsed = true;
    cmd->frame = openFrm;

    openFrm->commitAsync();
}

void RedpineSocketInterface::createServerSocket(SocketContext *cnxt, uint16_t port, uint8_t maxConnections, bool isUdp)
{
    Command *cmd = new Command();
    cmd->cnxt = cnxt;

    uint8_t ip[4] = {0,0,0,0};
    OpenSocketFrame *openFrm = new OpenSocketFrame(OpenSocketFrame::TCP_SSL_SERVER, ip, port, port, maxConnections);
    openFrm->setCompletionCallback<Command>(cmd, &Command::frameCompleted);
    openFrm->createdHandler.attach<Command>(cmd, &Command::openFrameResponse);
    openFrm->autoReleaseWhenParsed = true;
    cmd->frame = openFrm;

    openFrm->commitAsync();
}

void RedpineSocketInterface::handleIncomingData(const char *data, uint32_t length,
                                                uint32_t sockDesc, uint8_t fromIp[],
                                                uint16_t fromPort)
{
    if (sockDesc < 1)
    {
        return;
    }
    if (RedpineSocketInterface::activeSockets[sockDesc - 1] != 0)
    {
        SocketContext *socket = RedpineSocketInterface::activeSockets[sockDesc - 1];
        socket->_onData(data, length, fromIp, fromPort);
    }
}

void RedpineSocketInterface::handleConnectEvent(uint32_t sockDesc)
{

}


bool RedpineSocketInterface::writeData(const char *data, uint32_t length, uint32_t sockDesc, const uint8_t ipAddr[], uint16_t destPort, bool isUdp)
{
    uint32_t frmHeaderSize;
    if (isUdp)
        frmHeaderSize = OpenSocketFrame::TxDataOffsetUdp;
    else
        frmHeaderSize = OpenSocketFrame::TxDataOffsetTcp;

    int extraAlignmentPadding = 0;
    if (length != (length & ~3))
    {
        int newLength = (length + 3) & (~3);
        extraAlignmentPadding = newLength - length;
        length = newLength;
        debug("padding %i to 4-byte byte-align data\r\n", extraAlignmentPadding);
    }

    uint32_t totalSize = frmHeaderSize + length;
    OpenSocketFrame::rsi_frameSend *frame = (OpenSocketFrame::rsi_frameSend*) malloc(totalSize);
    memset(frame, 0, totalSize);

    frame->ip_version = 4;
    frame->socketDescriptor = sockDesc;
    frame->sendBufLen = length;
    frame->sendDataOffsetSize = frmHeaderSize;
    frame->destPort = destPort;
    memcpy(frame->destIPaddr.ipv4_address, ipAddr, 4);
    memcpy(((char*)frame)+frmHeaderSize, data, length - extraAlignmentPadding);

    bool success = Module::Instance()->sendDataFrame((uint8_t*)frame, totalSize);
    free(frame);

    return success;
}

void RedpineSocketInterface::closeSocket(SocketContext *cnxt, uint32_t sockDesc, uint16_t destPort)
{
    Command *cmd = new Command();
    cmd->cnxt = cnxt;

    CloseSocketFrame *closeFrm = new CloseSocketFrame(sockDesc, destPort);
    closeFrm->setCompletionCallback<Command>(cmd, &Command::frameCompleted);
    closeFrm->closedHandler.attach<Command>(cmd, &Command::closeFrameResponse);
    closeFrm->autoReleaseWhenParsed = true;
    cmd->frame = closeFrm;

    closeFrm->commitAsync();
}

// MARL: Redpine module HAL event interface

void RedpineSocketInterface::handleDataFrames(ModuleCommunication::DataPayload const &payload)
{
    OpenSocketFrame::recvFrameTcp *dataPayload = (OpenSocketFrame::recvFrameTcp*) payload.data;

    handleIncomingData(dataPayload->recvDataBuf, dataPayload->recvBufLen,
                       dataPayload->recvSocket, dataPayload->fromIPaddr.ipv4_address,
                       dataPayload->fromPortNum);
}


bool RedpineSocketInterface::handleAsyncMgmtFrames(ManagementFrame *frame)
{
    switch (frame->commandId) {
        case ModuleFrame::SocketClose:
        case ModuleFrame::AsyncSckTerminated:
        {
            CloseSocketFrame *closeFrm = (CloseSocketFrame*) frame;
            closeFrm->closedHandler.attach(this, &RedpineSocketInterface::handleCloseSocketFrame);
            break;
        }
        case ModuleFrame::AsyncTcpConnect:
        {
            AsyncTcpClientConnect *connFrm = (AsyncTcpClientConnect*) frame;
            connFrm->respHandler.attach(this, &RedpineSocketInterface::handleConnectSocketFrame);
            break;
        }
        default:
            debug("RP interface cannot handle this commandid: 0x%x\r\n", frame->commandId);
            return false;
            break;
    }

    return true;
}

void RedpineSocketInterface::handleCloseSocketFrame(const CloseSocketFrame::rsi_rsp_socket_close *resp)
{
    SocketContext *cnxt = activeSockets[resp->socket_id - 1];
    if (cnxt == 0)
    {
        return;
    }

    cnxt->_onClose(resp->socket_id, resp->sent_bytes_count);
    activeSockets[resp->socket_id - 1] = 0;
}

void RedpineSocketInterface::handleConnectSocketFrame(const AsyncTcpClientConnect::rsi_rsp_ltcp_est *resp)
{
    SocketContext *cnxt = activeSockets[resp->socket_id - 1];
    if (cnxt == 0)
    {
        return;
    }

    cnxt->_onConnectEstablish(resp->socket_id, resp->dest_ip_addr.ipv4_address, resp->dest_port, resp->src_port_num);
}




