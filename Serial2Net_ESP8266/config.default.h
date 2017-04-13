/*
    Configuration example
    Rename me to "config_CUSTOM.h"
    and include me from  Serial2Net_ESP8266.ino
*/

#define USE_WDT

//#define BONJOUR_SUPPORT
#define DEVICE_NAME     "Serial2Net_ESP8266" // Bonjour  name

// IP Address ------------------------------------------------------------------
#define STATIC_IP   // comment  to enable DHCP

#ifdef STATIC_IP
#define IP_ADDRESS      "10.1.7.41"
#define GATEWAY_ADDRESS "10.1.11.1"
#define NET_MASK        "255.255.0.0"
#endif

// Wifi credentials ------------------------------------------------------------
#define WIFI_SSID       "SSID_NAME"
#define WIFI_PASSWORD   "Your_SSID_Password"

// Server / Client Settings ----------------------------------------------------
#define TCP_LISTEN_PORT 9999
#define BAUD_RATE       57600   //RFLink default speed
#define BUFFER_SIZE     128     // serial end ethernet buffer size

// PINS ------------------------------------------------------------------------
#define WIFI_LED        16
#define CONNECTION_LED  14
#define TX_LED          12
#define RX_LED          13
