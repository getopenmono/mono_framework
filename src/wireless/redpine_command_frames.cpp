// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "redpine_command_frames.h"
#include "redpine_module.h"
#include <consoles.h>

#include <stdio.h>

using namespace mono::redpine;

// MARK: Op. Mode Frame

SetOperatingModeFrame::SetOperatingModeFrame(WifiOperModes mode) : ManagementFrame(ModuleFrame::SetOperatingMode)
{
    this->length = sizeof(operModeFrameSnd);
    this->operMode = mode;
}

void SetOperatingModeFrame::setDefaultConfiguration()
{
    this->coexMode = COEX_WIFI_ONLY;
    this->featureBitmap = (Features) 0x00;
    this->tcpipFeatureBitmap = (TcpIpFeatures) (ENABLE_HTTP_CLIENT | ENABLE_DHCPV4_CLIENT | ENABLE_DNS_CLIENT | ENABLE_ICMP);
    this->customFeatureBitmap = (CustomFeatures) 0x00;
}

void SetOperatingModeFrame::dataPayload(uint8_t *dataBuffer)
{
    memset(dataBuffer, 0, sizeof(operModeFrameSnd));
    operModeFrameSnd *payload = (operModeFrameSnd*) dataBuffer;

    payload->oper_mode = operMode | (coexMode << 16);
    payload->feature_bit_map = featureBitmap;
    payload->tcp_ip_feature_bit_map = tcpipFeatureBitmap;
    payload->custom_feature_bit_map = customFeatureBitmap;
}


// MARK: BAND FRAME

BandFrame::BandFrame() : ManagementFrame(ManagementFrame::Band)
{
    this->length = sizeof(bandFrameSnd);
    this->band.bandVal = 0;

    for (int i=0; i<3; i++) {
        band.alignment[i] = 0;
    }
}

void BandFrame::dataPayload(uint8_t *dataBuffer)
{
    memset(dataBuffer, 0, sizeof(bandFrameSnd));
    bandFrameSnd *payload = (bandFrameSnd*) dataBuffer;
    *payload = this->band;
}


// MARK: INIT FRAME

InitFrame::InitFrame() : ManagementFrame(ManagementFrame::Init)
{
    this->length = 0;
    this->responsePayload = true;

    // zero the mac address and alignment
    for (int i=0; i<6; i++) {
        response.macAddress[i] = 0;
    }
    for (int i=0; i<2; i++) {
        response.alignment[i] = 0;
    }
}

void InitFrame::responsePayloadHandler(uint8_t *payloadBuffer)
{
    memcpy((void*)&this->response, payloadBuffer, sizeof(initFrameResponse));
}

// MARK: SCAN FRAME


ScanFrame::ScanFrame() : ManagementFrame(ManagementFrame::Scan)
{
    this->length = sizeof(scanFrameSnd);
    this->responsePayload = true;
    this->ssid = "";
    this->scanChannelNumber = 0;
}

void ScanFrame::dataPayload(uint8_t *dataBuffer)
{
    memset(dataBuffer, 0, sizeof(scanFrameSnd));
    scanFrameSnd *frm = (scanFrameSnd*) dataBuffer;

    // copy the ssid string, with length limit
    memcpy(frm->ssid, ssid, maxSsidLength<strlen(ssid) ? maxSsidLength : strlen(ssid) );
    frm->channel = scanChannelNumber;
}

void ScanFrame::responsePayloadHandler(uint8_t *dataBuffer)
{
    scanResponse *resp = (scanResponse*) dataBuffer;

    if (scanResponseHandler)
    {
        scanResponseHandler.call(resp);
    }
    else
    {
        debug("Scan found %i APs:\r\n", resp->scanCount);
        if (resp->scanCount < 12) {
            for (uint16_t i=0; i<resp->scanCount; i++) {
                scanInfoResp *ap = &(resp->scanInfos[i]);
                debug(" * %s, rssi: %i\r\n",ap->ssid, ap->rssiVal);
            }
        }
    }
}


// MARK: JOIN FRAME

JoinFrame::JoinFrame(String ssid, String pass, int secMode) : ManagementFrame(Join)
{
    this->length = sizeof(joinFrameSnd);
    this->responsePayload = false;
    this->ssid = ssid;
    this->passphrase = pass;
    this->securityMode = secMode;
}


void JoinFrame::dataPayload(uint8_t *dataBuffer)
{
    memset(dataBuffer, 0, sizeof(joinFrameSnd));
    joinFrameSnd *raw = (joinFrameSnd*) dataBuffer;

    raw->Security_mode = securityMode;
    raw->dataRate = 0; // auto
    raw->powerLevel = 0; // low signal power
    memcpy(raw->psk, passphrase(), passphrase.Length());
    memcpy(raw->ssid, ssid(), ssid.Length());
    raw->ssid_len = ssid.Length();
}


// MARK: Set IP Parameters frame

SetIpParametersFrame::SetIpParametersFrame() : ManagementFrame(ManagementFrame::SetIPParameters)
{
    this->length = sizeof(ipparamFrameSnd);
    this->responsePayload = true;

    for (int i=0; i<4; i++) {
        this->ipAddress[i] = 0;
        this->netmask[i] = 0;
        this->gateway[i] = 0;
    }

    dhcpMode = DHCP_ENABLE;
    hostname = "";
}

void SetIpParametersFrame::dataPayload(uint8_t *dataBuffer)
{
    memset(dataBuffer, 0, sizeof(ipparamFrameSnd));
    ipparamFrameSnd *frm = (ipparamFrameSnd*) dataBuffer;
    memcpy(frm->hostname, hostname, maxHostnameLength<strlen(hostname) ? maxHostnameLength : strlen(hostname));

    frm->dhcpMode = dhcpMode;

    for (int i=0; i<4; i++)
    {
        frm->ipaddr[i] = ipAddress[i];
        frm->netmask[i] = netmask[i];
        frm->gateway[i] = gateway[i];
    }
}

void SetIpParametersFrame::responsePayloadHandler(uint8_t *databuffer)
{
    ipparamFrameResp *resp = (ipparamFrameResp*) databuffer;

    memcpy(this->ipAddress, resp->ipaddr, 4);
    memcpy(this->gateway, resp->gateway, 4);
    memcpy(this->netmask, resp->netmask, 4);
    memcpy(this->macAddress, resp->macAddr, 6);
}

// MARK: DNS RESOLUTE FRAME

DnsResolutionFrame::DnsResolutionFrame(String domainName) : ManagementFrame(DnsResolution)
{
    this->length = sizeof(dnsQryFrameSnd);
    this->responsePayload = true;
    this->domain = domainName;

    memset(resIpAddress, 0, 16);
}

void DnsResolutionFrame::dataPayload(uint8_t *dataBuffer)
{
    memset(dataBuffer, 0, sizeof(dnsQryFrameSnd));
    dnsQryFrameSnd *frm = (dnsQryFrameSnd*) dataBuffer;
    memcpy(frm->aDomainName, domain(), (domain.Length()+1)<maxDomainNameLength ? (domain.Length()+1) : maxDomainNameLength);
    frm->ip_version = 4; // use IPv4
}

void DnsResolutionFrame::responsePayloadHandler(uint8_t *databuffer)
{
    if (this->status != 0)
    {
        respSuccess = false;
        return;
    }

    TCP_EVT_DNS_Query_Resp *resp = (TCP_EVT_DNS_Query_Resp*) databuffer;
    respSuccess = true;
    if (resp->uIPCount == 0)
    {
        mono::Debug << "No IP address resolved from " << this->domain();
        respSuccess = false;
    }
    else if (resp->ip_version == 4)
        memcpy(resIpAddress, resp->IpAddrs[0].ipv4_address, 4);
    else if (resp->ip_version == 6)
        memcpy(resIpAddress, resp->IpAddrs[0].ipv6_address, 16);
    else
    {
        mono::Warn << "Unsupported IP version: " << resp->ip_version << "\n";
        respSuccess = false;
    }

    ipVersion = resp->ip_version;
}


// MARK: HTTP GET Frame

HttpGetFrame::HttpGetFrame(String host, String ipaddrs, String url, FILE *destFile, uint32_t httpPort) :
    ManagementFrame(HttpGet)
{
    this->hostname = host;
    this->ipaddress = ipaddrs;
    this->url = url;
    this->httpPort = httpPort;
    this->extraHeader = "";
    this->responsePayload = true;
    this->lastResponseParsed = false;
    destinationFile = destFile;
}

void HttpGetFrame::dataPayload(uint8_t *data)
{
    memset(data, 0, this->payloadLength());

    HttpReqFrameSnd *frm = (HttpReqFrameSnd*) data;
    frm->ip_version = 4;
    frm->http_port = httpPort;
    frm->options = ENABLE_NULL_DELIMITER;
    uint8_t *strPnt = (uint8_t*) &(frm->buffer);

    memcpy(strPnt++, "", 1); // username param
    memcpy(strPnt++, "", 1); // password param

    //debug("sizeof struct: %i, hstnm: %i, ipaddr: %i, url: %i, extHdr: %i\r\n",sizeof(HttpReqFrameSnd),hostname.Length(),ipaddress.Length(),url.Length(),extraHeader.Length());

    memcpy(strPnt, hostname(), hostname.Length());
    strPnt += hostname.Length()+1;

    memcpy(strPnt, ipaddress(), ipaddress.Length());
    strPnt += ipaddress.Length()+1;

    memcpy(strPnt, url(), url.Length());
    strPnt += url.Length()+1;

    memcpy(strPnt, extraHeader(), extraHeader.Length());
}

void HttpGetFrame::responsePayloadHandler(uint8_t *data)
{
    HttpRsp *resp = (HttpRsp*) data;
    //mono::defaultSerial.printf("HttpGet Recv %i bytes\r\n",resp->data_len);

    if (destinationFile)
    {
        fwrite(&(resp->data), resp->data_len, 1, destinationFile);
        fflush(destinationFile);
    }
    else
    {
        CallbackData cbData;
        cbData.dataLength = resp->data_len;
        cbData.data = &(resp->data);
        cbData.context = this;
        lastResponseParsed = resp->more == 1 ? true : false;
        dataReadyHandler.call(&cbData);
    }

    // "more" is high when the data is transferred
    if (resp->more == 1)
    {
        lastResponseParsed = true;
        if (destinationFile)
            fclose(destinationFile);
        //mono::defaultSerial.printf("HttpGet ended\r\n");
    }
}

int HttpGetFrame::payloadLength()
{
    int size = sizeof(HttpReqFrameSnd);
    size += 1 + hostname.Length() + 1 + ipaddress.Length() + 1 + url.Length() + 1 + extraHeader.Length();

    return (size+3) & ~3;
}

HttpGetFrame::~HttpGetFrame()
{
    //debug("dealloc HttpGetFrame\r\n");
}



// MARK: HTTP POST FRAME

HttpPostFrame::HttpPostFrame(String hostname, String serverIp, String Url, FILE *destFile, uint32_t serverPort) :
    mono::redpine::HttpGetFrame(hostname, serverIp, Url, destFile, serverPort)
{
    commandId = HttpPost;
}

int HttpPostFrame::payloadLength()
{
    int size = sizeof(HttpReqFrameSnd);
    size += 1 + hostname.Length() + 1 + ipaddress.Length() + 1 + url.Length() + 1 + extraHeader.Length() + 1;

    if (requestDataLengthCallback)
        size += requestDataLengthCallback.call();

    // the padding zeroes becomes a part of the request body (but 4-byte alignment is required by the API)
    return (size+3) & ~3; //4 byte align
}

void HttpPostFrame::dataPayload(uint8_t *data)
{
    int payLength = this->payloadLength();
    memset(data, 0, payLength);

    HttpReqFrameSnd *frm = (HttpReqFrameSnd*) data;
    frm->ip_version = 4;
    frm->http_port = httpPort;
    frm->options = ENABLE_NULL_DELIMITER;
    uint8_t *strPnt = (uint8_t*) &(frm->buffer);

    memcpy(strPnt++, "", 1); // username param
    memcpy(strPnt++, "", 1); // password param

    //debug("sizeof struct: %i, hstnm: %i, ipaddr: %i, url: %i, extHdr: %i\r\n",sizeof(HttpReqFrameSnd),hostname.Length(),ipaddress.Length(),url.Length(),extraHeader.Length());

    memcpy(strPnt, hostname(), hostname.Length());
    strPnt += hostname.Length()+1;

    memcpy(strPnt, ipaddress(), ipaddress.Length());
    strPnt += ipaddress.Length()+1;

    memcpy(strPnt, url(), url.Length());
    strPnt += url.Length()+1;

    memcpy(strPnt, extraHeader(), extraHeader.Length());
    strPnt += extraHeader.Length()+1;

    int dataLen = requestDataLengthCallback.call();;

    if (requestDataCallback)
        requestDataCallback.call((char*)(strPnt));

    strPnt += dataLen;

    int remainder = payLength - (strPnt - (uint8_t*)frm);


    if (remainder > 0)
    {
        debug("Padding packet with spaces (0x20) to align with 4-byte length!\r\n");
        memset(strPnt, ' ', remainder);
    }
}



// MARK: OPEN SOCKET FRAME

OpenSocketFrame::OpenSocketFrame() : ManagementFrame(SocketCreate)
{
    this->responsePayload = true;
}

OpenSocketFrame::OpenSocketFrame(SocketTypes type, uint8_t *ipAddress, uint16_t localPort, uint16_t remotePort) : ManagementFrame(SocketCreate)
{
    this->responsePayload = true;

    this->frameData.ip_version = 4;
    frameData.socketType = TCP_SSL_CLIENT;
    frameData.moduleSocket = localPort;
    frameData.destSocket = remotePort;

    memcpy((void*)frameData.destIPaddr.ipv4_address, ipAddress, 4);

    frameData.max_count = 0;
    frameData.tos = 0;
    frameData.ssl_ws_enable = 0;
    frameData.ssl_ciphers = 0;
}

void OpenSocketFrame::dataPayload(uint8_t *data)
{
    memcpy((void*)data, (void*)&frameData, sizeof(socketFrameSnd));

}

void OpenSocketFrame::responsePayloadHandler(uint8_t *data)
{
    printf("Response:\t\n");

    struct socketFrameRcv *revc = (struct socketFrameRcv*) data;
    printf("SocDesc: %u\n\tPort: %u\t\n",revc->socketDescriptor, revc->moduleSocket);

    this->lastResponseParsed = true;

    createdHandler.call(revc);
}

int OpenSocketFrame::payloadLength()
{
    return sizeof(socketFrameSnd);
}

// MARK: QUERY FIRMWARE

QueryFirmwareFrame::QueryFirmwareFrame() : ManagementFrame(QueryFirmware)
{
    this->responsePayload = true;
    this->length = 0;
}

void QueryFirmwareFrame::responsePayloadHandler(uint8_t *payloadBuffer)
{
    QueryFirmwareFrame::rsi_qryFwversionFrameRecv *recv = (QueryFirmwareFrame::rsi_qryFwversionFrameRecv*) payloadBuffer;

    memdump(payloadBuffer, sizeof(rsi_qryFwversionFrameRecv));
    String firm((char*)recv->fwversion, 20);

    responseCb.call(firm);
}


// MARK: SET POWER MODE FRAME

PowerModeFrame::PowerModeFrame(PowerModeFrame::PowerSaveModes saveMode, PowerModeFrame::UltraLowPowerModes powMode, bool dtimBeacon) : ManagementFrame(PowerSaveMode)
{
    frame.powerVal = saveMode;
    frame.ulp_mode_enable = powMode;
    frame.listen_interval_dtim = dtimBeacon;

    this->length = sizeof(PowerFrameSnd);
}

void PowerModeFrame::dataPayload(uint8_t *data)
{
    memcpy(data, &frame, sizeof(PowerFrameSnd));
}



// MARK: SET SLEEP TIMER FRAME

SetSleepTimerFrame::SetSleepTimerFrame(uint16_t sleepSecs) : ManagementFrame(SleepTimer)
{
    frame.TimeVal[0] = SleepTimer & 0xFF;
    frame.TimeVal[1] = (SleepTimer >> 8) & 0xFF;

    this->length = sizeof(struct SleepTimerFrameSnd);
}

void SetSleepTimerFrame::dataPayload(uint8_t *data)
{
    memcpy(data, &frame, sizeof(struct SleepTimerFrameSnd));
}


