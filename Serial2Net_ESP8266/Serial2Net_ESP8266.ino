/*
	Serial2Net ESP8266
	https://github.com/soif/Serial2Net_ESP8266
	Copyright 2017 François Déchery

	** Description **
	Briges a Serial Port to/from	(Wifi attached) LAN using a ESP8266 board

	Inpired by : ESP8266 mDNS serial wifi bridge by Daniel Parnell
	https://github.com/dparnell/esp8266-ser2net/blob/master/esp8266_ser2net.ino
*/

// Config #####################################################################
//#include		"config_CUSTOM.h"
#include	"config_315.h"
//#include	"config_433.h"


// Includes ###################################################################
#include <ESP8266WiFi.h>

#include <FancyLED.h> //https://github.com/carlynorama/Arduino-Library-FancyLED

//#include <FancyLED.h> //https://github.com/carlynorama/Arduino-Library-FancyLED


#ifdef BONJOUR_SUPPORT
#include <ESP8266mDNS.h>
#endif

// Defines #####################################################################
#define LED_CYCLE_DURATION 120
#define LED_ON_PERCENT 70

// Variables ###################################################################

#ifdef BONJOUR_SUPPORT
MDNSResponder mdns; // multicast DNS responder
#endif

WiFiServer server(TCP_LISTEN_PORT);
WiFiClient client;

FancyLED led_tx		= FancyLED(RX_LED,	HIGH);
FancyLED led_rx		= FancyLED(TX_LED,	HIGH);
FancyLED led_wifi	= FancyLED(WIFI_LED,	HIGH);



// #############################################################################
// Main ########################################################################
// #############################################################################

// ----------------------------------------------------------------------------
#ifdef STATIC_IP
IPAddress parse_ip_address(const char *str) {
	IPAddress result;
	int index = 0;

	result[0] = 0;
	while (*str) {
		if (isdigit((unsigned char)*str)) {
			result[index] *= 10;
			result[index] += *str - '0';
		}
		else {
			index++;
			if(index<4) {
				result[index] = 0;
			}
		}
		str++;
	}

	return result;
}

#endif


// ----------------------------------------------------------------------------
void connect_to_wifi() {

	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	delay(100);
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

#ifdef STATIC_IP
	IPAddress ip_address = parse_ip_address(IP_ADDRESS);
	IPAddress gateway_address = parse_ip_address(GATEWAY_ADDRESS);
	IPAddress netmask = parse_ip_address(NET_MASK);
	WiFi.config(ip_address, gateway_address, netmask);
#endif

	digitalWrite(CONNECTION_LED, LOW);
	led_wifi.turnOff();
	led_tx.turnOff();
	led_rx.turnOff();

	// Wait for WIFI connection
	while (WiFi.status() != WL_CONNECTED) {
#ifdef USE_WDT
		wdt_reset();
#endif
		led_wifi.toggle();
		delay(150);
	}
	led_wifi.turnOn();
}


// ----------------------------------------------------------------------------
void errorMdns() {
	int count = 0;

	digitalWrite(CONNECTION_LED, LOW);
	led_tx.turnOff();
	led_rx.turnOff();

	while(1) {
		led_wifi.toggle();
		delay(100);
	}
}


// ----------------------------------------------------------------------------
void setup(void){

#ifdef USE_WDT
	wdt_enable(1000);
#endif

	//set Leds
	digitalWrite(CONNECTION_LED, LOW);
	pinMode(CONNECTION_LED, OUTPUT);
	led_rx.setFullPeriod(LED_CYCLE_DURATION);
	led_rx.setDutyCycle(LED_ON_PERCENT);
	led_tx.setFullPeriod(LED_CYCLE_DURATION);
	led_tx.setDutyCycle(LED_ON_PERCENT);

	Serial.begin(BAUD_RATE);

	// Connect to WiFi network
	connect_to_wifi();

#ifdef BONJOUR_SUPPORT
	// Set up mDNS responder:
	if (!mdns.begin(DEVICE_NAME, WiFi.localIP())) {
		errorMdns();
	}
#endif

	// Start TCP server
	server.begin();
}


// ----------------------------------------------------------------------------
void loop(void){
	led_tx.update();
	led_rx.update();
	//led_wifi.update();

	size_t bytes_read;
	uint8_t net_buf[BUFFER_SIZE];
	uint8_t serial_buf[BUFFER_SIZE];

#ifdef USE_WDT
	wdt_reset();
#endif

	if(WiFi.status() != WL_CONNECTED) {
		// we've lost the connection, so we need to reconnect
		if(client) {
			client.stop();
		}
		connect_to_wifi();
	}

#ifdef BONJOUR_SUPPORT
	// Check for any mDNS queries and send responses
	mdns.update();
#endif

	// Check if a client has connected
	if (!client) {
		// eat any bytes in the serial buffer as there is nothing to see them
		while(Serial.available()) {
			Serial.read();
		}

		client = server.available();

		if(!client) {
#ifdef CONNECTION_LED
			digitalWrite(CONNECTION_LED, LOW);
#endif
			return;
		}

#ifdef CONNECTION_LED
		digitalWrite(CONNECTION_LED, HIGH);
#endif
	}


#define min(a,b) ((a)<(b)?(a):(b))

	if(client.connected()) {
		// check the network for any bytes to send to the serial
		int count = client.available();
		if(count > 0) {
			//led_tx.pulse(1, LED_CYCLE_DURATION, LED_ON_PERCENT);
			led_tx.pulse();

			bytes_read = client.read(net_buf, min(count, BUFFER_SIZE));
			Serial.write(net_buf, bytes_read);
			Serial.flush();
		}

		// now check the serial for any bytes to send to the network
		bytes_read = 0;
		while(Serial.available() && bytes_read < BUFFER_SIZE) {
			serial_buf[bytes_read] = Serial.read();
			bytes_read++;
		}

		if(bytes_read > 0) {
			//led_rx.pulse(1, LED_CYCLE_DURATION, LED_ON_PERCENT);
			led_rx.pulse();
			client.write((const uint8_t*)serial_buf, bytes_read);
			client.flush();
		}
	}
	else {
		client.stop();
	}
}
