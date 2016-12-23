# ESP8266 DHTxx to MQTT

This program uses [esp\_mqtt](https://github.com/tuanpmt/esp_mqtt), [esp8266\_dhtxx](https://github.com/eadf/esp8266_dhtxx), and [esp8266\_easygpio](https://github.com/eadf/esp8266_easygpio) to read a DHTxx sensor and publish readings to an MQTT topic

## Installation
Clone the repository including the subtree modules.

Mount a read-write case-sensitive volume,
`sudo hdiutil attach -readwrite ~/code/esp-open-sdk/case-sensitive.dmg`

Update your $PATH to include the xtensa-lx106-elf/bin directory

Set $SDK\_BASE to the esp\_iot\_sdk location

## Usage
Add configuration in localinclude/user\_config.h. This file needs to contain MQTT broker information (host, port, ssl, client ID, username, password), local wifi login information (SSID and password), and the topic to which you will publish sensor data

To build,
`make clean && make`

To flash onto the esp8266 hardware, set ESPPORT and make flash like this:
`make ESPPORT=dev/tty.usbserial-AFJRNX35 flash`


## License
MIT License. 
