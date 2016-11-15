// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#include "http_post_client.h"
#include "regex.h"

using namespace mono::network;

HttpPostClient::HttpPostClient() : HttpClient()
{
    shouldPost = false;
}

HttpPostClient::HttpPostClient(String anUrl, String headers)
{
    shouldPost = false;
    destPort = 80;
    this->headers = headers;

    mono::Regex ipreg(HttpClient::ipRegex);

    mono::Regex::Capture ipCaps[4];
    bool success = ipreg.Match(anUrl, ipCaps, 4);
    if (success)
    {
        String ip = ipreg.Value(ipCaps[1]);
        domain = ip;
        String httpPort = ipreg.Value(ipCaps[2]);
        path = ipCaps[3].len == 0 ? "/" : ipreg.Value(ipCaps[3]);

        if (httpPort.Length() > 0 && httpPort != String("80"))
        {
            sscanf(httpPort(), "%lu",&destPort);
        }

        getFrame = 0;
        createFrameRequest(ip);
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
        dns.setCompletionCallback<HttpPostClient>(this, &HttpPostClient::dnsComplete);
        dns.setErrorCallback<HttpPostClient>(this, &HttpPostClient::dnsResolutionError);
        getFrame = NULL;
        postFrame = NULL;
    }
    

}

HttpPostClient::HttpPostClient(const HttpPostClient &other) : HttpClient(other)
{
    postFrame = other.postFrame;
    shouldPost = other.shouldPost;
    frameDataHandler = other.frameDataHandler;
    frameDataLengthHandler = other.frameDataLengthHandler;

    if (postFrame != NULL)
    {
        postFrame->setDataReadyCallback<HttpPostClient>(this, &HttpPostClient::httpData);
        postFrame->setCompletionCallback<HttpPostClient>(this, &HttpPostClient::httpCompletion);
    }
}

HttpPostClient &HttpPostClient::operator=(const HttpPostClient &other)
{
    this->HttpClient::operator=(other);
    postFrame = other.postFrame;
    shouldPost = other.shouldPost;
    frameDataHandler = other.frameDataHandler;
    frameDataLengthHandler = other.frameDataLengthHandler;

    if (postFrame != NULL)
    {
        postFrame->setDataReadyCallback<HttpPostClient>(this, &HttpPostClient::httpData);
        postFrame->setCompletionCallback<HttpPostClient>(this, &HttpPostClient::httpCompletion);
    }

    return *this;
}

void HttpPostClient::createFrameRequest(String ip)
{
    postFrame = new redpine::HttpPostFrame(domain, ip, path, NULL, destPort);
    postFrame->setDataReadyCallback<HttpPostClient>(this, &HttpPostClient::httpData);
    postFrame->setCompletionCallback<HttpPostClient>(this, &HttpPostClient::httpCompletion);
    postFrame->requestDataLengthCallback = frameDataLengthHandler;
    postFrame->requestDataCallback = frameDataHandler;

    if (headers.Length() > 0)
        postFrame->extraHeader = headers();
}

bool HttpPostClient::hasFrameRequest()
{
    return postFrame != 0;
}

void HttpPostClient::dnsComplete(INetworkRequest::CompletionEvent *evnt)
{
    if (hasFrameRequest())
    {
        debug("post frame already exists!\r\n");
    }

    this->HttpClient::dnsComplete(evnt);

    if (postFrame != 0 && shouldPost)
    {
        setState(IN_PROGRESS_STATE);
        postFrame->commitAsync();
    }
}

void HttpPostClient::post()
{
    shouldPost = true;

    if (hasFrameRequest())
    {
        setState(IN_PROGRESS_STATE);
        postFrame->commitAsync();
    }
}
