// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#include "http_post_client.h"
#include "regex.h"

using namespace mono::network;

HttpPostClient::HttpPostClient() : HttpClient()
{

}

HttpPostClient::HttpPostClient(String anUrl)
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

        postFrame = new redpine::HttpPostFrame(ip, ip, path, NULL, destPort);
        postFrame->setDataReadyCallback<HttpClient>(this, &HttpPostClient::httpData);
        postFrame->setCompletionCallback<HttpClient>(this, &HttpPostClient::httpCompletion);


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
        dns.setCompletionCallback<HttpClient>(this, &HttpPostClient::dnsComplete);
        dns.setErrorCallback<HttpClient>(this, &HttpPostClient::dnsResolutionError);
        getFrame = NULL;
    }
    

}

HttpPostClient::HttpPostClient(const HttpPostClient &other) : HttpClient(other)
{
    postFrame = other.postFrame;
}

HttpPostClient &HttpPostClient::operator=(const HttpPostClient &other)
{
    this->HttpClient::operator=(other);
    postFrame = other.postFrame;

    return *this;
}

void HttpPostClient::post()
{
    setState(IN_PROGRESS_STATE);
    postFrame->commitAsync();
}
