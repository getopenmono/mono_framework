// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#include "http_client.h"
#include "regex.h"

using namespace mono::network;

const char *HttpClient::ipRegex = "(http://)(\\d+\\.\\d+\\.\\d+\\.\\d+):?(\\d*)(/?[^\\s'\\\"<>]*)";
const char *HttpClient::domainRegex = "(http://)([^\\s/'\\\"<>\\?:,_;\\*\\^\\!<>]+):?(\\d*)(/?[^\\s'\\\"<>]*)";

HttpClient::HttpClient() : INetworkRequest(), respData(this), getFrame(NULL) {}

HttpClient::HttpClient(String anUrl) : INetworkRequest(), respData(this), getFrame(NULL)
{
    destPort = 80;
    mono::Regex ipreg(HttpClient::ipRegex);

    mono::Regex::Capture ipCaps[4];
    bool success = ipreg.Match(anUrl, ipCaps, 4);
    if (success)
    {
        String ip = ipreg.Value(ipCaps[1]);
        String httpPort = ipreg.Value(ipCaps[2]);
        path = ipCaps[3].len == 0 ? "/" : ipreg.Value(ipCaps[3]);

        if (httpPort.Length() > 0 && httpPort != String("80"))
        {
            sscanf(httpPort(), "%lu",&destPort);
        }

        getFrame = new redpine::HttpGetFrame(ip, ip, path, NULL, destPort);
        getFrame->setDataReadyCallback<HttpClient>(this, &HttpClient::httpData);
        getFrame->setCompletionCallback<HttpClient>(this, &HttpClient::httpCompletion);

        getFrame->commitAsync();
    }
    else
    {
        mono::Regex reg(domainRegex);

        mono::Regex::Capture caps[4];
        success = reg.Match(anUrl, caps, 4);

        if (!success) {
            debug("url parse err");
            lastErrorCode = URL_PARSE_ERROR;
            triggerQueuedErrorHandler();
            return;
        }

        domain = reg.Value(caps[1]);
        path = caps[3].len == 0 ? "/" : reg.Value(caps[3]);
        String httpPort = reg.Value(caps[2]);

        if (httpPort.Length() > 0 && httpPort != String("80"))
        {
            sscanf(httpPort(), "%lu", &destPort);
        }

        dns = DnsResolver(domain);
        dns.setCompletionCallback<HttpClient>(this, &HttpClient::dnsComplete);
        dns.setErrorCallback<HttpClient>(this, &HttpClient::dnsResolutionError);
        getFrame = NULL;
    }

    setState(IN_PROGRESS_STATE);
}


HttpClient::HttpClient(const HttpClient &other) :
    INetworkRequest(other),
    respData(other.respData),
    getFrame(other.getFrame)
{
    domain = other.domain;
    path = other.path;
    dns = other.dns;
    destPort = other.destPort;

    dns.setCompletionCallback<HttpClient>(this, &HttpClient::dnsComplete);
    dns.setErrorCallback<HttpClient>(this, &HttpClient::dnsResolutionError);

    if (getFrame != NULL)
    {
        getFrame->setDataReadyCallback<HttpClient>(this, &HttpClient::httpData);
        getFrame->setCompletionCallback<HttpClient>(this, &HttpClient::httpCompletion);
    }
}

HttpClient &HttpClient::operator=(const mono::network::HttpClient &other)
{
    INetworkRequest::operator=(other);
    respData = other.respData;
    getFrame = other.getFrame;
    domain = other.domain;
    path = other.path;
    dns = other.dns;
    destPort = other.destPort;

    dns.setCompletionCallback<HttpClient>(this, &HttpClient::dnsComplete);
    dns.setErrorCallback<HttpClient>(this, &HttpClient::dnsResolutionError);

    if (getFrame != NULL)
    {
        getFrame->setDataReadyCallback<HttpClient>(this, &HttpClient::httpData);
        getFrame->setCompletionCallback<HttpClient>(this, &HttpClient::httpCompletion);
    }

    return *this;
}

void HttpClient::httpCompletion(redpine::ManagementFrame::FrameCompletionData *data)
{
    if (!data->Success)
    {
        lastErrorCode = COMMUNICATION_ERROR;
        triggerDirectErrorHandler();
    }
    else
    {

    }
}

void HttpClient::dnsResolutionError(INetworkRequest::ErrorEvent *evnt)
{
    this->lastErrorCode = evnt->ErrorCode;
    triggerDirectErrorHandler(); // Error evnt is already queued
}

void HttpClient::dnsComplete(INetworkRequest::CompletionEvent *evnt)
{
    if (getFrame != NULL)
    {
        debug("getFrame != NULL, Invalid state! getFrame is: 0x%x\r\n",getFrame);
        lastErrorCode = INVALID_STATE_ERROR;
        triggerDirectErrorHandler(); // dns complete cb is already queued
        return;
    }

    getFrame = new redpine::HttpGetFrame(domain, dns.IpAddress(), path, NULL, destPort);
    getFrame->setDataReadyCallback<HttpClient>(this, &HttpClient::httpData);
    getFrame->setCompletionCallback<HttpClient>(this, &HttpClient::httpCompletion);
    getFrame->commitAsync();
}

void HttpClient::httpData(redpine::HttpGetFrame::CallbackData *data)
{
    respData.bodyChunk = String((char*)(data->data), data->dataLength+1);
    respData.bodyChunk.stringData[data->dataLength] = 0;
    respData.Finished = data->context->lastResponseParsed;

    //debug("got data call data ready async\r\n");
    //debug("-> %s\r\n",respData.bodyChunk());
    // call data ready async, to release mem. objs on stack
    Timer::callOnce<HttpClient>(0, this, &HttpClient::triggerDataReady);
}

void HttpClient::triggerDataReady()
{
    dataHandler.call(respData);

    if (respData.Finished)
    {
        //debug("comp handlr\r\n");
        triggerCompletionHandler();
    }
}

HttpClient::~HttpClient()
{
    if (getFrame != NULL && getFrame->handlerContextObject == this)
    {
        delete getFrame;
    }
}



