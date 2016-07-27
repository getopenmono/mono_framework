//
//  redpine_command_frames.h
//  Mono::Redpine
//
//  Created by Kristoffer Andersen on 01/06/15.
//
//

#ifndef __spiTest__redpine_command_frames__
#define __spiTest__redpine_command_frames__

#include "module_frames.h"

#include <mn_string.h>
#include <FileSystemLike.h>

namespace mono { namespace redpine {
    
    /**
     * A Set Operating Mode management frame with the needed command payload data.
     * 
     */
    class SetOperatingModeFrame : public ManagementFrame
    {
    public:
        
        /** Operating modes for wifi */
        enum WifiOperModes
        {
            WIFI_CLIENT_MODE = 0, /**< Connects to a Access Point */
            WIFI_DIRECT_MODE = 1, /**< Is either Wifi Direct or Autonomous GO mode */
            WIFI_ENTERPRISE  = 2, /**< Can connect to an Access Point with WPA/WPA2-Enterprise security. */
            WIFI_ACCESS_POINT= 6, /**< The module acts as an Access Point */
            WIFI_PER_MODE    = 8  /**< This mode is used for calculating packet error rate and mostly used during RF certification tests. */
        };
        
        /** Co-Existence modes */
        enum CoexModes
        {
            COEX_WIFI_ONLY  = 1, /**< Only Wifi (WLAN) is enabled */
            COEX_WIFI_ZIGBEE= 2, /**< Wifi and ZigBee is enabled */
            COEX_WIFI_BT    = 4, /**< Wifi and Bluetooth Classic is enabled no TCP/IP */
            COEX_WIFI_BLE   = 0xC/**< Wifi and Bluetooth LE is enabled, no TCP/IP */
        };
        
        /** Feature bitmap defines, OR these together to det features */
        enum Features
        {
            NO_WIFI_SECURITY        = 0x01, /**< Allow no Wifi security (open network) */
            ENABLE_PSK_SECURITY     = 0x02, /**<  */
            ENABLE_AGGREGATION      = 0x04, /**<  */
            ENABLE_LP_GPIO_HANDSHAKE= 0x08, /**<  */
            ENABLE_ULP_GPIO_HANDSHAKE=0x10, /**<  */
            USE_ULP_GPIO_1_AS_WAKEUP= 0x20, /**< Use GPIO 1 on ULP, instead of GPIO 21 */
            USE_3V3_RF_VOLTAGE      = 0x40, /**< Use 3.3V RF voltage, instead of 1.9V  */
        };
        
        /** Feature bitmap for TCP/IP */
        enum TcpIpFeatures
        {
            ENABLE_TCP_BYPASS       = 0x0001, /**<  */
            ENABLE_HTTP_SERVER      = 0x0002, /**<  */
            ENABLE_DHCPV4_CLIENT    = 0x0004, /**<  */
            ENABLE_DHCPV6_CLIENT    = 0x0008, /**<  */
            ENABLE_DHCPV4_SERVER    = 0x0010, /**<  */
            ENABLE_DHCPV6_SERVER    = 0x0020, /**<  */
            ENABLE_JSON_OBJECTS     = 0x0040, /**<  */
            ENABLE_HTTP_CLIENT      = 0x0080, /**<  */
            ENABLE_DNS_CLIENT       = 0x0100, /**<  */
            ENABLE_SNMP_CLIENT      = 0x0200, /**<  */
            ENABLE_SSL              = 0x0400, /**<  */
            ENABLE_ICMP             = 0x0800, /**<  */
            ENABLE_HTTPS_SERVER     = 0x1000, /**<  */
            SEND_CONFIG_TO_HOST     = 0x4000, /**<  */
        };
        
        /** Custom features bitmap definitions */
        enum CustomFeatures
        {
            DISABLE_DHCP_SERVER_GATEWAY = 0x04,     /**<  */
            ENABLE_HIDDEN_SSID          = 0x20,     /**<  */
            ENABLE_GPIO16_LED_INIT      = 0x0200,   /**<  */
            ENABLE_ASYNC_MESSAGES       = 0x0400,   /**<  */
            ENABLE_UART_PACKET_PENDING  = 0x0800,   /**<  */
            ENABLE_AP_BLACKLIST         = 0x1000,   /**<  */
            ENABLE_DEAUTHENTICATION     = 0x020000, /**<  */
            ENABLE_AUTO_CONFIGURATION   = 0x100000, /**<  */
            ENABLE_PACKET_BUFFER_LIMIT  = 0x400000, /**<  */
            ENABLE_HTTP_AUTHENCICATION  = 0x800000  /**<  */
        };
        
        /** The raw data struct */
        typedef struct __attribute__((packed))
        {
            uint32_t oper_mode;
            uint32_t feature_bit_map;
            uint32_t tcp_ip_feature_bit_map;
            uint32_t custom_feature_bit_map;
        } operModeFrameSnd;
        
        /** The Wifi operation mode */
        WifiOperModes operMode;
        
        /** The wireless co-existence mode */
        CoexModes coexMode;
        
        /** Sets the opermode feature bitmap */
        Features featureBitmap;
        
        /** Sets the opermodes TCP/IP feature bitmap */
        TcpIpFeatures tcpipFeatureBitmap;
        
        /** Sets the opermode other custom features bitmap */
        CustomFeatures customFeatureBitmap;
        
        /**
         * Construct a SetOperMode frame with a given operation mode
         * This will not set any coex modes of additional feature settings.
         *
         * Only the Wifi operation mode is set here. You must call the method
         * @ref setDefaultConfiguration to initialize all the settings, before
         * you send this frameto the module.
         *
         * It initialize standard configurations refer to the static initializer
         * functions.
         *
         * @param mode The module operation mode (Client, AP, etc.)
         */
        SetOperatingModeFrame(WifiOperModes mode);
        
        /**
         * Set a standard Wifi only configuration. This means setting the
         * radio Co-Existence to Wifi-only. this method also sets the module
         * functions to these default settings:
         *
         * * Dont allow open wifi networks (network must have security)
         * * Built-in HTTP client enabled
         * * DHCPv4 client enabled (not IPv6 DHCP)
         * * DNS client enabled
         * * Module respond in ICMP (ping)
         * 
         * No further features are enabled. (@ref CustomFeatures)
         */
        void setDefaultConfiguration();
        
        void dataPayload(uint8_t *dataBuffer);
    };
    
    
    /**
     * Command frame to send the Band command.
     * This class contains the means to send the command with the needed 
     * payload data.
     *
     * This command configures the band in which the module has to be 
     * configured. The valid bands are:
     *
     * * 2.4 GHz
     * * 5 GHz
     * * Dual Band (2.4 and 5 GHz)
     *
     * _NOTE: Dual band is supported only in station and WiFi Direct mode_
     */
    class BandFrame : public ManagementFrame
    {
    public:
        /**
         * The Band commands payload data struct
         * This defines the band to be used.
         *
         * The parameter `bandVal` support these values:
         *
         * Value | Description
         * ------|------------
         * 0     | 2.4 GHz Wifi mode
         * 1     | 5 GHz Wifi mode
         * 2     | Dual band (_Only supported stadion and WiFi Direct mode_)
         *
         */
        typedef struct {
            uint8_t bandVal;        /**< 2.4 GHz, 5 GHz or dual band property */
            uint8_t alignment[3];   /**< Used only to align the data to 4-bytes */
        } bandFrameSnd;
        
        /** The selected Band, default is 2.4 GHz */
        bandFrameSnd band;
        
        /**
         * Construct a default band frame command that can be sent to the module.
         * The frame is initialized with the default band selected: 2.4 GHz
         */
        BandFrame();
        
        void dataPayload(uint8_t *dataBuffer);
    };
    
    
    /**
     * Command Frame for initializing the module.
     * 
     * This command programs the module’s Baseband and RF components and returns
     * the MAC address of the module to the host.
     *
     * The frame contains no payload data, but its response has a payload.
     */
    class InitFrame : public ManagementFrame
    {
    public:
        /**
         * The response payload data format for the Init frame
         */
        typedef struct {
            uint8_t macAddress[6];  /**< The MAC address of the module */
            uint8_t alignment[2];   /**< Used only to align the data to 4-bytes */
        } initFrameResponse;
        
        /** 
         * The received MAC address for the module. This value is only present
         * after the command response has been parsed.
         */
        initFrameResponse response;
        
        /**
         * Construct the object
         * The command takes no argumemts nor contain any properties.
         */
        InitFrame();
        
        void responsePayloadHandler(uint8_t *payloadBuffer);
    };
    
    /**
     * Scan command frame to sent to the module.
     * 
     * This command scans for Access Points and gives the scan results to the 
     * host. The scan results are sorted in decreasing order of signal strength
     * (RSSI value). The scanned access point with highest signal strength will 
     * be the first in the list.
     *
     * The command response is a complex structure where the identofied AP 
     * networks are descibed.
     * The property `securityMode` is a number form 0 to 5 and defined as:
     *
     * `securityMode` | Description
     * ---------------|------------
     *       0        | Open mode, no security
     *       1        | WPA Personal
     *       2        | WPA2 Personal
     *       3        | WEP
     *       4        | WPA Enterprise
     *       5        | WPA2 Enterprise
     * 
     *
     */
    class ScanFrame : public ManagementFrame
    {
    public:
        
        /** Module defines the maximum length of a SSID name to be this value */
        static const int maxSsidLength = 34;
        
        /** Module defines the maximum number of scan results as this number */
        static const int scannedMaximumResponse = 11;
        
        /** 
         * Raw scan frame format, as sent to the module.
         * Structure ti 4-byte aligned
         */
        typedef struct
        {
            uint32_t    channel;                /**< @ RF channel to scan, 0=All, 1-14 for 2.5GHz channels 1-14 */
            uint8_t     ssid[maxSsidLength];    /**< optional: hidden ssid to scan */
            uint8_t     reserved[6];            /**< reserved fields */
            uint8_t     channel_bit_map_2_4[2]; /**< channel bit map for 2.4 Ghz */
            uint8_t     channel_bit_map_5[4];   /**< channel bit map for 5 Ghz */
        } scanFrameSnd;
        
        
        /** Command response structure, describing an identified AP */
        typedef struct
        {
            uint8_t rfChannel;          /**< The APs channel */
            uint8_t securityMode;       /**< AP Wifi Security settings. See @ref ScanFrame */
            uint8_t rssiVal;            /**< Signal strength */
            uint8_t uNetworkType;       /**< AP Type: 1 is Infrastructure Mode */
            uint8_t ssid[maxSsidLength];/**< SSID of the access pint */
            uint8_t bssid[6];           /**< MAC address of the access point */
            uint8_t reserved[2];        /**< Reserved bytes */
        } scanInfoResp;
        
        /** The raw command response format, as returned by the module. */
        typedef struct
        {
            uint32_t scanCount;                             /**< Number of found networks */
            uint8_t  padding[4];                            /**< Reserved */
            scanInfoResp scanInfos[scannedMaximumResponse]; /**< List of results */
        } scanResponse;
        
        /** Callback handler, used when the list of SSIDs are returned */
        mbed::FunctionPointerArg1<void, scanResponse*> scanResponseHandler;

        const char *ssid;
        
        /**
         * The channel number to scan.
         * 0 means all channels.
         */
        uint32_t scanChannelNumber;
        
        /** Construct a Scan command frame */
        ScanFrame();
        
        
        void dataPayload(uint8_t *dataBuffer);
        
        
        void responsePayloadHandler(uint8_t *dataBuffer);

        /**
         * @brief Set the ScanResponse callback function
         *
         * When the scan for access points is completed, this handler is 
         * triggered. You set your callback function here, to retrieve a list of
         * found APs.
         *
         * The module returns a maximum of 11 visible AP's, ordered DESC by
         * signal strength. (RSSI)
         *
         * @param obj A pointer to the callback functions member context (the this pointer)
         * @param memPtr A point to the member function itself.
         *
         * @see scanResponse
         */
        template <typename Owner>
        void setScanResponseCallback(Owner *obj, void(Owner::*memPtr)(scanResponse*))
        {
            this->scanResponseHandler.attach<Owner>(obj, memPtr);
        }
    };
    
    
    /**
     * this command frame will associate the module with an access point or
     * create one - depending on the operation mode settings.
     *
     * This command is used for following:
     *
     * 1) Associate to an access point (operating mode = 0 or 2)
     * 2) Associate to a remote device in Wi-Fi Direct mode or to create autonomous GO(operating mode 1)
     * 3) Create an Access Point (operating mode 6)
     * 4) Allow a third party to associate to a Wi-Fi Direct group created by the module (operating mode 1)
     * 5) To enable WPS PUSH method in Access point mode
     *
     */
    class JoinFrame : public ManagementFrame
    {
    public:
        /**
         * Raw join frame format, as sent to the module.
         * Bytes are are appended to make the structure 4-byte aligned
         */
        typedef struct
        {
            uint8_t reserved1;      /**<  */
            uint8_t Security_mode;
            uint8_t dataRate;
            uint8_t powerLevel;
            uint8_t psk[64];
            uint8_t ssid[34];
            uint8_t reserved[3];
            uint8_t ssid_len;
            uint8_t alignment[2];   /**< Needed for 4-byte alignment */
        } joinFrameSnd;
        
        String ssid;
        String passphrase;
        int securityMode;
        
        /**
         *
         * 
         */
        JoinFrame(String ssid, String passphrase, int secMode = 6);
        
        void dataPayload(uint8_t *dataBuffer);
    };
    
    
    /**
     * Command frame for the command "Set IP Parameters"
     * 
     * This command configures the IP address, subnet mask and default gateway
     * for the module. Or activates DHCP client mode.
     *
     * You can setup 3 modes of IP configuration, defnied by the field `dhcpMode`.
     * See the table below for the different modes:
     *
     * `dhcpMode` | Description
     * -----------|------------
     *      0     | Manual / Static IP configuration
     *      1     | DHCP Enabled, dynamic IP configuration
     *      3     | DHCP Enabled and send the hostname in DHCP discover (default)
     *
     * The hostname is the identifier for the DHCP server, and can be maximum 
     * 31 characters. The default hostname is "mono".
     */
    class SetIpParametersFrame : public ManagementFrame
    {
    public:
        
        /** The raw frame payload data format */
        typedef struct
        {
            uint8_t dhcpMode;       /**< Toggles the DHCP modes, static IP vs dynamic IP */
            uint8_t ipaddr[4];
            uint8_t netmask[4];
            uint8_t gateway[4];
            uint8_t hostname[31];
        } ipparamFrameSnd;
        
        /** The raw frame response payload */
        typedef struct
        {
            uint8_t macAddr[6];
            uint8_t ipaddr[4];
            uint8_t netmask[4];
            uint8_t gateway[4];
        } ipparamFrameResp;
        
        /** The available DHCP configuaration modes, bit map */
        enum dhcpModes
        {
            STATIC_IP = 0,              /**< Set manual ip address */
            DHCP_ENABLE = 1,            /**< Enable the dhcp client, assign dynamic address */
            DHCP_HOSTNAME_ENABLE = 2    /**< Enable transmit of the hostname (default) */
        };
        
        /** The maximum allowed hostname length */
        static const int maxHostnameLength = 31;
        
        /** The hostname transmitted in DHCP mode */
        const char *hostname;
        
        /**
         * Bit map of the dhcp configuration, with default constructor the mode
         * is: DHCP_ENABLE | DHCP_HOSTNAME_ENABLE
         */
        uint8_t dhcpMode;
        
        /** Byte array with the static ip address, used only in STATIC_IP mode */
        uint8_t ipAddress[4];
        
        /** Byte array with the ip netmask, used only in STATIC_IP mode */
        uint8_t netmask[4];
        
        /** Byte array with the default gateway, used only in STATIC_IP mode */
        uint8_t gateway[4];
        
        /** The mac address receive from the frame response */
        uint8_t macAddress[6];
        
        /**
         * Construct a the command as dynamic ip (DHCP enabled) with hostname
         * "mono".
         */
        SetIpParametersFrame();
        
        void dataPayload(uint8_t *dataBuffer);
        
        void responsePayloadHandler(uint8_t *databuffer);
        
    };
    
    
    /**
     * This command is to obtain the IP address of the specified domain name.
     * 
     */
    class DnsResolutionFrame : public ManagementFrame
    {
    public:
        
        /** The maximum length for a domain name to query */
        static const int maxDomainNameLength = 90;
        
        /** raw frame payload Data, bytes appended for alignment. */
        typedef struct {
            uint16_t ip_version;                        /**< IP version, 4 or 6 */
            uint8_t aDomainName[maxDomainNameLength];   /**< The domain name to query */
            uint8_t reserved[4];                        /**< Reserved + 4-byte alignment */
        } dnsQryFrameSnd;
        
        /** Maximum number of returned ip addresses */
        static const int maxDnsReply = 10;
        
        /** Raw response frame payload data */
        typedef struct
        {
            uint16_t ip_version;            /**< The IP version used, is either 4 or 6 */
            uint16_t uIPCount;              /**< The number of IP adresses resolved from domain name */
            union
            {
                uint8_t ipv4_address[4];    /**< If IP version is 4, this is the IP address */
                uint8_t ipv6_address[16];   /**< If IP version is 6, this is the IP address */
            } IpAddrs[maxDnsReply];         /**< The list of returned IP addresses */
        } TCP_EVT_DNS_Query_Resp;
        
        /** The domain to lookup */
        String domain;
        
        /**  Is `true` if DNS resolution is completed and successful. `false` otherwise. */
        bool respSuccess;
        
        /** Either the IPc4 or IPv6 ip address, after dns resolution finised */
        uint8_t resIpAddress[16];
        
        /** The IP version returned from resolution. Either 4 or 6 */
        uint8_t ipVersion;
        
        /** 
         * Construct a new DNS query command.
         * Use the @ref commit() method to execute the resolution.
         *
         * @param domainName The domain name string to resolve
         */
        DnsResolutionFrame(String domainName);
        
        void dataPayload(uint8_t *dataBuffer);
        
        void responsePayloadHandler(uint8_t *databuffer);
    };
    
    /**
     * This command is used to transmit HTTP GET request from the module to
     * a remote HTTP server. A subsequent HTTP GET request can be issued
     * only after receiving the response of the previously issued HTTP GET
     * request. Module acts as a HTTP client when this command is issued.
     *
     * Currently the payload data can only be 300 bytes.
     */
    class HttpGetFrame : public ManagementFrame
    {
    public:
        
        /** Data struct used in the DataReady callback function */
        typedef struct
        {
            uint16_t dataLength;    /**< The number of data bytes */
            uint8_t *data;          /**< A pointer to the data returned */
            HttpGetFrame *context;  /**< A pointer to the HttpGetFrame that triggered the callback */
        } CallbackData;
        
        /** The maximum length of the TTP Request Buffer blob */
        static const int maxHttpBufferLength = 1200;
        
        /** @brief List of possible Http Get options for the raw payload data format */
        enum GetOptions
        {
            ENABLE_HTTPS = 1,           /**< Use HTTPS */
            ENABLE_NULL_DELIMITER = 2,  /**< Use NULL chars instead of commas in buffer property */
            ENABLE_SSL_TLS_1_0 = 4,     /**< Use SSL 1.0, available only when has SSL enabled */
            ENABLE_SSL_TLS_1_2 = 8      /**< Use SSL 1.2, available only when has SSL enabled */
        };
        
        /** @brief Raw frame payload data */
        typedef struct
        {
            uint16_t ip_version;    /**< ip version 4 or 6 */
            uint16_t options;       /**< Options for HTTPS, delimiter and SSL */
            uint16_t http_port;     /**< server port number */
            uint8_t  buffer;        /**< Username, Password,Hostname, IP address,url,header,data */
        } HttpReqFrameSnd;
        
        /** @brief The raw data response payload */
        typedef struct
        {
            uint32_t more;      /**< Indicate if more HTTP data is pending. 0: more data, 1: EOF */
            uint32_t offset;    /**< Is always '0' */
            uint32_t data_len;  /**< Lenght of current data chunk (bytes) */
            uint8_t  data;      /**< The current data chunk, max is 1400 bytes */
        } HttpRsp;
        
        /** The HTTP servers hostname (for virtual hosts) */
        String hostname;
        
        /** The server ip address as a string, ex: `192.168.1.10` */
        String ipaddress;
        
        /** The URL to call, ex: `/index.html` */
        String url;
        
        /** Extra HTTP headers to sent along with the GET request. */
        String extraHeader;
        
        /**
         * If this is set the HTTP Body is written to the file
         * If this is NULL, then nothing is obviosly written :-)
         */
        FILE *destinationFile;
        
        /** Callback handler, used when no file pointer is given */
        mbed::FunctionPointerArg1<void, CallbackData*> dataReadyHandler;
        
        /**
         * Construct a HTTP GET request frame
         *
         * @param hostname The hostname of the HTTP server (might be a virtual host)
         * @param serverIp The real IP address of the server, as string
         * @param Url The URL to call on the HTTP server
         */
        HttpGetFrame(String hostname, String serverIp, String Url, FILE *destFile);
        
        ~HttpGetFrame();
        
        void dataPayload(uint8_t *data);
        
        void responsePayloadHandler(uint8_t *data);
        
        int payloadLength();
        
        /**
         * @brief Set the DataReady callback function
         * 
         * When you do not supply a `FILE` pointer, the HTTP body is returned
         * through a DataReady callback function. The function must take one parameter
         * that is of the type @ref HttpGetFrame::CallbackData .
         * 
         * @param obj A pointer to the callback functions member context (the this pointer)
         * @param memPtr A point to the member function itself.
         */
        template <typename Owner>
        void setDataReadyCallback(Owner *obj, void(Owner::*memPtr)(CallbackData*))
        {
            this->dataReadyHandler.attach<Owner>(obj, memPtr);
        }
    };

    /**
     * This command opens a TCP/UDP/SSL/Websoket client socket, a Listening 
     * TCP/UDP/SSL socket.
     *
     */
    class OpenSocketFrame : public ManagementFrame
    {
    public:

        static const uint8_t WebSocketMaxUrlLen   = 51; /**< If Websocket this is the max url length */
        static const uint8_t WebSocketMaxHostLen  = 51; /**< If WebSocket this is the max hostname length */

        /** The available types of IP sockets */
        enum SocketTypes
        {
            TCP_SSL_CLIENT = 0, /**< A TCP or SSL Client Socket */
            TCP_SSL_SERVER = 2, /**< A TCP or SSL Server / Listening Socket */
            UDP_LISTEN = 4      /**< An UDP Server / Listening Socket */
        };

        /** The Types of services, as defined by the IPv4 standard */
        enum TypeOfServices {
            TOS_BEST_EFFORT = 0,
            TOS_PRIORITY = 1,
            TOS_IMMEDIATE = 2,
            TOS_FLASH = 3,
            TOS_FLASH_OVERRIDE = 4,
            TOS_CRITICAL = 5,
            TOS_INTERNET = 6,
            TOS_NETWORK = 7
        };

        /** Raw command sent to the module */
        struct socketFrameSnd {
            uint16_t  ip_version;   /**< IP version used, either 4 or 6. */
            uint16_t  socketType;   /**< Type of the socket, @ref SocketTypes */
            uint16_t  moduleSocket; /**< Port number of the socket in the module. Value ranges from 1024 to 49151 */
            uint16_t  destSocket;   /**< destination port. Value ranges from 1024 to 49151. Ignored when TCP server or Listening UDP sockets are to be opened. */
            union{
                /**
                 * @brief IP Address of the target server.
                 * Ignored when TCP server or Listening UDP sockets are to be opened.
                 * If ip_version is 4 then only first four bytes of the ipv4_address
                 * is filled rest twelve bytes will be 0.
                 */
                uint8_t   ipv4_address[4];
                /**
                 * @brief IPv6 Address of the target server.
                 * Ignored when TCP server or Listening UDP sockets are to be opened.
                 * All 16 bytes are filled if ip_version is 6.
                 */
                uint32_t   ipv6_address[4];
            } destIPaddr;           /**< The sockets destination IP (clients only) */
            uint16_t max_count;     /**< Maximum number of clients can be connect in case of listening socket. */
            uint32_t tos;           /**< type of service field. @ref TypeOfServices */
            uint8_t ssl_ws_enable;  /**< This field is used to enable SSL and/or WebSockets */
            uint8_t ssl_ciphers;    /**< 1 byte bitmap used to select the various cipher modes. */
            uint8_t webs_resource_name[WebSocketMaxUrlLen];
            uint8_t webs_host_name[WebSocketMaxHostLen];
        };

        /** Response payload for the open socket command */
        struct socketFrameRcv {
            uint16_t  ip_version;       /**< IP version used, either 4 or 6 */
            uint16_t  socketType;       /**< Type of the created socket. @ref SocketTypes */
            uint16_t  socketDescriptor; /**< Created socket’s descriptor or handle, starts from 1. sockeDescriptor ranges from 1 to 10. */
            uint16_t  moduleSocket;     /**< Port number of the socket in the module. */
            union{
                /**
                 * @brief The IPv4 address of the module.
                 * Only first four bytes of ipv4_address is filled rest 12 bytes
                 * are `0` in case of IPv4.
                 */
                uint8_t ipv4_addr[4];
                /**
                 * The IPv6 address of the module in case of IPv6
                 */
                uint32_t ipv6_addr[4];
            }moduleIPaddr;              /**< Modules IP address */
            uint16_t  mss;              /**< maximum segment size of the remote peer. In case of Ludp/Ltcp this field will not present. */
            uint32_t  window_size;      /**< Window size of the remote peer. In case of Ludp/Ltcp this field will not present. */
        };


        // MARK: Socket receive frame

        static const uint8_t RxDataOffsetTcpV4 = 26;
        static const uint8_t RxDataOffsetTcpV6 = 46;
        static const uint8_t RxDataOffsetUdpV4 = 14;
        static const uint8_t RxDataOffsetUdpV6 = 34;
        static const uint16_t TcpMaxPayloadSize = 1460;
        static const uint16_t UdpMaxPayloadSize = 1472;

        typedef struct {
            uint16_t ip_version;
            uint16_t recvSocket;
            uint32_t recvBufLen;
            uint16_t recvDataOffsetSize;
            uint16_t fromPortNum;
            union{
                uint8_t     ipv4_address[4];
                uint8_t     ipv6_address[16];
            } fromIPaddr;
            uint8_t recvDataOffsetBuf[RxDataOffsetTcpV4];
            uint8_t recvDataBuf[TcpMaxPayloadSize];
        } recvFrameTcp;

    protected:

        struct socketFrameSnd frameData;
    public:
        
        OpenSocketFrame();
        OpenSocketFrame(SocketTypes type, uint8_t *ipAddress, uint16_t localPort, uint16_t remotePort);

        void dataPayload(uint8_t *data);

        void responsePayloadHandler(uint8_t *data);

        int payloadLength();
    };
    
//    class HttpPostFrame : public HttpGetFrame
//    {
//        HttpPostFrame(String hostname, String serverIp, String path, );
//    };
    
    
    /**
     * Command to set the module into one of 5 power saving modes. Some modes is
     * used while connected to an AP, other are full sleep modes, where module is
     * not connected.
     *
     * The module boots into *power save mode* disabled.
     *
     *
     * #### The DTIM Parameter
     * This parameter is valid only if BIT(1) is set in the `join_feature_bitmap`
     * and valid listen interval is given in join command. If this parameter is set,
     * the module computes the desired sleep duration based on listen interval
     * (from join command) and its wakeup align with Beacon or DTIM Beacon
     * (based on this parameter).
     *
     * The DTIM parameter controls if:
     *
     * 0. module wakes up before nearest Beacon that does not exceed the specified
     *    listen interval time.
     * 1. module wakes up before nearest DTIM Beacon that does not exceed the 
     *    specified listen interval time.
     */
    class PowerModeFrame : public ManagementFrame
    {
    public:
        
        /**
         * @brief The possible power save modes for the module
         * The following modes can be used while the module is connected to an AP,
         * after the JOIN command:
         * * `RF_SLEEP_ONLY_CONNECTED`
         * * `RF_SOC_SLEEP_GPIO_CONNECTED`
         * * `RF_SOC_SLEEP_CONNECTED`
         *
         * The two remaining modes are for use after the INIT command before the JOIN
         * command.
         */
        enum PowerSaveModes
        {
            DISABLED                    = 0, /**< No power save mode active, *default* */
            RF_SLEEP_ONLY_CONNECTED     = 1, /**< Antenna off during AP DTIM intervals */
            RF_SOC_SLEEP_GPIO_CONNECTED = 2, /**< Antenna and SoC off during DTIM, GPIO wake up. */
            RF_SOC_SLEEP_CONNECTED      = 3, /**< Antenna and SoC off during DTIM, no async wake up */
            SLEEP_MODE_GPIO             = 8, /**< Full system sleep mode, AP not connected. GPIO wake up */
            SLEEP_MODE                  = 9  /**< Full system sleep, AP disconnected. Only timer wake up  */
        };
        
        /**
         * Power modes. The power save modes can be further controlled by choosing
         * an ultro low power mode here.
         */
        enum UltraLowPowerModes
        {
            /** Normal low power mode */
            LOW_POWER_MODE = 0,
            /** Ultra low power mode, with RAM retension (only in save mode 2 - 9). */
            ULTRA_LOW_POWER_MODE_KEEP_RAM = 1,
            /** Ultra low power mode, without RAM retension (only save mode 8 & 9). */
            ULTRA_LOW_POWER_MODE_LOOSE_RAM = 2
        };
        
    protected:
        
        /** Redpine command struct for the powerMode frame */
        typedef struct {
            uint8_t powerVal;
            uint8_t ulp_mode_enable;
            uint32_t listen_interval_dtim;
            uint8_t _alignment[2]; // struct must be 4-byte aligned
        } PowerFrameSnd;
        
        PowerFrameSnd frame;
        
        void dataPayload(uint8_t *data);
        
    public:
        
        /**
         * Construct a PowerModeFrame that will set the module into a low power mode,
         * or disabled any current low power mode set earlier.
         * 
         * @param saveMode One of the 5 power save modes. See @ref PowerSaveModes
         * @param powMode Deide if power safe mode should be Ultra Low power or Low Power
         * @param dtimBeacon Decide whether the DTIM or Beason is used af wake up interval.
         */
        PowerModeFrame(PowerSaveModes saveMode, UltraLowPowerModes powMode, bool dtimBeacon);
    };
    
    
    class SetSleepTimerFrame : public ManagementFrame
    {
    protected:
        
        struct SleepTimerFrameSnd {
            uint8_t TimeVal[2];
            uint8_t _alignment[2]; // struct must be 4-byte aligned
        };
        
        struct SleepTimerFrameSnd frame;
        
        void dataPayload(uint8_t *data);
        
    public:
        
        SetSleepTimerFrame(uint16_t sleepSecs);
    };
    
    
}}

#endif /* defined(__spiTest__redpine_command_frames__) */
