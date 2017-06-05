/*
	Serial2Net ESP8266
	https://github.com/soif/Serial2Net_ESP8266
	Copyright 2017 François Déchery

	** Description **********************************************************
	Briges a Serial Port to/from a (Wifi attached) LAN using a ESP8266 board

	** Inpired by ***********************************************************
	* ESP8266 Ser2net by Daniel Parnell
	https://github.com/dparnell/esp8266-ser2net/blob/master/esp8266_ser2net.ino

	* WiFiTelnetToSerial by Hristo Gochkov.
	https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/WiFiTelnetToSerial/WiFiTelnetToSerial.ino
*/

// Use your Own Config #########################################################
#include	"config_CUSTOM.h"
//#include	"config_315.h"
//#include	"config_433.h"


// Includes ###################################################################
#include <ESP8266WiFi.h>
#include <FancyLED.h> 		// https://github.com/jej/Arduino-Library-FancyLED
#include <SyncLED.h> 		// https://github.com/martin-podlubny/arduino-library-syncled


// Defines #####################################################################
#define MAX_SRV_CLIENTS 		4

#define PATTERN_DURATION		180ul
#define PATTERN_PAUSE_COUNT		3
#define PATTERN_ON_COUNT		1
#define PATTERN_OFF_COUNT		1

#define LED_PLULSE_DURATION		120
#define LED_PLULSE_ON_PERCENT	70


// Variables ###################################################################
int last_srv_clients_count=0;

WiFiServer server(TCP_LISTEN_PORT);
WiFiClient serverClients[MAX_SRV_CLIENTS];

FancyLED	led_tx		= FancyLED(RX_LED,	HIGH);
FancyLED	led_rx		= FancyLED(TX_LED,	HIGH);
FancyLED	led_wifi	= FancyLED(WIFI_LED,	HIGH);
SyncLED		led_connect(CONNECTION_LED);


// #############################################################################
// Main ########################################################################
// #############################################################################

// ----------------------------------------------------------------------------
void setup(void){

#ifdef USE_WDT
	wdt_enable(1000);
#endif

	// Set Leds
	led_connect.setRate(PATTERN_DURATION);
	led_connect.Off();
	led_rx.setFullPeriod(LED_PLULSE_DURATION);
	led_rx.setDutyCycle(LED_PLULSE_ON_PERCENT);
	led_tx.setFullPeriod(LED_PLULSE_DURATION);
	led_tx.setDutyCycle(LED_PLULSE_ON_PERCENT);

	// Connect to WiFi network
	connect_to_wifi();

	// Start UART
	Serial.begin(BAUD_RATE);

	// Start server
	server.begin();
	server.setNoDelay(true);
}


// ----------------------------------------------------------------------------
void loop(void){
	led_tx.update();
	led_rx.update();
	led_connect.update();
	//led_wifi.update();

#ifdef USE_WDT
	wdt_reset();
#endif

	// Check Wifi connection -----------------
	if(WiFi.status() != WL_CONNECTED) {
		// we've lost the connection, so we need to reconnect
		for(byte i = 0; i < MAX_SRV_CLIENTS; i++){
			if(serverClients[i]){
				serverClients[i].stop();
			}
		}
		connect_to_wifi();
	}

	// Check if there are any new clients ---------
	uint8_t i;
    if (server.hasClient()){
		for(i = 0; i < MAX_SRV_CLIENTS; i++){
			//find free/disconnected spot
			if (!serverClients[i] || !serverClients[i].connected()){
				if(serverClients[i]){
					serverClients[i].stop();
				}
         		serverClients[i] = server.available();
				//Serial1.print("New client: "); Serial1.print(i);
				continue;
        	}
		}
		// No free/disconnected spot so reject
		WiFiClient serverClient = server.available();
		serverClient.stop();
    }

	//blink according to clients connected ---------
	int srv_clients_count=0;
	for(i = 0; i < MAX_SRV_CLIENTS; i++){
		if (serverClients[i] && serverClients[i].connected()){
			srv_clients_count++;
		}
	}

	if(srv_clients_count != last_srv_clients_count){
		last_srv_clients_count=srv_clients_count;
		UpdateBlinkPattern(srv_clients_count);
	}

    // check clients for data ------------------------
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
		if (serverClients[i] && serverClients[i].connected()){
        	if(serverClients[i].available()){
    			//get data from the telnet client and push it to the UART
    			while(serverClients[i].available()) {
			  		led_tx.pulse();
			  		Serial.write(serverClients[i].read());
			  		led_tx.update();
				}
			}
		}
    }

    // check UART for data --------------------------
    if(Serial.available()){
	      size_t len = Serial.available();
	      uint8_t sbuf[len];
		  Serial.readBytes(sbuf, len);
		  //push UART data to all connected telnet clients
		  for(i = 0; i < MAX_SRV_CLIENTS; i++){
			  if (serverClients[i] && serverClients[i].connected()){
				  led_rx.pulse();
				  serverClients[i].write(sbuf, len);
				  led_tx.update();
				  delay(1);
			  }
    	}
    }
}


// Functions ###################################################################

// ----------------------------------------------------------------------------
void connect_to_wifi() {

	// is this really needed ?
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	delay(100);

	// connect
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

#ifdef STATIC_IP
	IPAddress ip_address = parse_ip_address(IP_ADDRESS);
	IPAddress gateway_address = parse_ip_address(GATEWAY_ADDRESS);
	IPAddress netmask = parse_ip_address(NET_MASK);
	WiFi.config(ip_address, gateway_address, netmask);
#endif

	led_connect.Off();
	led_wifi.turnOff();
	led_tx.turnOff();
	led_rx.turnOff();

	// Wait for WIFI connection
	while (WiFi.status() != WL_CONNECTED) {
#ifdef USE_WDT
		wdt_reset();
#endif
		led_wifi.toggle();
		delay(100);
	}
	led_wifi.turnOn();
}


// ----------------------------------------------------------------------------
void UpdateBlinkPattern(int srv_count){
	if(srv_count > 0){
		unsigned long pattern=0;
		int len=  ( (PATTERN_ON_COUNT + PATTERN_OFF_COUNT) * srv_count ) + PATTERN_PAUSE_COUNT;

		if(len > 32){
			pattern=0B1101101010;
			len=10;
		}
		else if(srv_count==1){
			pattern=0B1111;
			len=4;
		}
		else{
			//build pattern (from right to left)
			int b=0;
			for(int i=0; i < PATTERN_PAUSE_COUNT; i++){
				bitWrite(pattern,b,0);
				b++;
			}
			for (int i=0 ; i < srv_count; i++){
				for(int j=0; j < PATTERN_OFF_COUNT; j++){
					bitWrite(pattern,b, 0);
					b++;
				}
				for(int k=0; k < PATTERN_ON_COUNT; k++){
					bitWrite(pattern,b, 1);
					b++;
				}
			}
		}
		//Serial.print(pattern,BIN);
		led_connect.setPattern(pattern, len);
	}
	else{
		led_connect.Off();
	}
}


// ----------------------------------------------------------------------------
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
