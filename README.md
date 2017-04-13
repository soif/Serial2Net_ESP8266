# Serial2Net ESP8266

This ESP8266 based project is a bridge which allows to send/receive the TX/RX Serial port of any device (ie another arduino) to the LAN, using the ESP8266 Wifi connection. I use it mainly to connect RFLink to my LAN, but it could be used whenever you need a "Ser2net" LAN connection.


## Features

- Supports 4 (or more) LAN clients
- 4 Status Leds (Wifi State, Number of Clients Connected, TX, RX)
- The "Clients Connected" Led blinks the time of clients count.


## Schematic
![schematic](images/schematic.png)


## Wiring
![wiring](images/wiring.png)


## License

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
