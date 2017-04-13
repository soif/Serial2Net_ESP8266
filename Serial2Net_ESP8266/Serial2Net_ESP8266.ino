/*
	Serial2Net ESP8266
	https://github.com/soif/Serial2Net_ESP8266
	Copyright 2017 François Déchery

	** Description ********
	Briges a Serial Port to/from	(Wifi attached) LAN using a ESP8266 board

	** Inpired by *********
	* ESP8266 mDNS serial wifi bridge by Daniel Parnell
	https://github.com/dparnell/esp8266-ser2net/blob/master/esp8266_ser2net.ino
	* WiFiTelnetToSerial by Hristo Gochkov.
	https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/WiFiTelnetToSerial/WiFiTelnetToSerial.ino
*/

// Config #####################################################################
//#include		"config_CUSTOM.h"
//#include	"config_315.h"
#include	"config_433.h"


// Includes ###################################################################
#include <ESP8266WiFi.h>
#include <FancyLED.h> //https://github.com/carlynorama/Arduino-Library-FancyLED
#include <SyncLED.h> //https://github.com/martin-podlubny/arduino-library-syncled


// Defines #####################################################################
#define MAX_SRV_CLIENTS 4

#define LED_CONNECT_RATE 180ul
#define NUM_PAUSE 3
#define NUM_ON 1
#define NUM_OFF 1

#define LED_CYCLE_DURATION 120
#define LED_ON_PERCENT 70


// Variables ###################################################################
int last_srv_clients_count=0;

WiFiServer server(TCP_LISTEN_PORT);
WiFiClient serverClients[MAX_SRV_CLIENTS];

FancyLED led_tx		= FancyLED(RX_LED,	HIGH);
FancyLED led_rx		= FancyLED(TX_LED,	HIGH);
FancyLED led_wifi	= FancyLED(WIFI_LED,	HIGH);
SyncLED led_connect(CONNECTION_LED);


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
void setup(void){

#ifdef USE_WDT
	wdt_enable(1000);
#endif

	//set Leds
	led_connect.setRate(LED_CONNECT_RATE);
	led_connect.Off();
	led_rx.setFullPeriod(LED_CYCLE_DURATION);
	led_rx.setDutyCycle(LED_ON_PERCENT);
	led_tx.setFullPeriod(LED_CYCLE_DURATION);
	led_tx.setDutyCycle(LED_ON_PERCENT);

	// Connect to WiFi network
	connect_to_wifi();

	//start UART
	Serial.begin(BAUD_RATE);
	//start server
	server.begin();
	server.setNoDelay(true);
}


// ----------------------------------------------------------------------------
void UpdateBlinkPattern(int srv_count){

	if(srv_count > 0){

		//Serial.print(srv_count);

		unsigned long pattern=0;
		int len=  ( (NUM_ON + NUM_OFF) * srv_count ) + NUM_PAUSE;

		if(len > 32){
			//Serial.print(" (over 32) ");
			pattern=0B1101101010;
			len=10;
		}
		else if(srv_count==1){
			//Serial.print(" (special) ");
			pattern=0B1111;
			len=4;
		}
		else{
			//Serial.print(", len=");
			//Serial.print(len);
			int b=0;
			for(int i=0; i < NUM_PAUSE; i++){
				bitWrite(pattern,b,0);
				b++;
			}
			for (int i=0 ; i < srv_count; i++){
				for(int j=0; j < NUM_OFF; j++){
					bitWrite(pattern,b, 0);
					b++;
				}
				for(int k=0; k < NUM_ON; k++){
					bitWrite(pattern,b, 1);
					b++;
				}
			}
		}

		//Serial.print(" => ");
		//Serial.print(pattern,BIN);
		//Serial.println("");
		led_connect.setPattern(pattern, len);
	}
	else{
		led_connect.Off();
	}
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

	// Check connection -----------------
	if(WiFi.status() != WL_CONNECTED) {
		// we've lost the connection, so we need to reconnect
		for(byte i = 0; i < MAX_SRV_CLIENTS; i++){
			if(serverClients[i]){
				serverClients[i].stop();
			}
		}
		connect_to_wifi();
	}

	// ----------------------------------
	uint8_t i;
    //check if there are any new clients
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
		//no free/disconnected spot so reject
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
