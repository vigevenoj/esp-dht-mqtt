ESP8266 - Driver for the temperature and humidity sensor DHT11 and DHT22
========================================================================

I've modified CHERTS's driver to allow you to connect DHTxx sensors to any of the GPIO pins.<br>
I also made this project into a pure driver.<br>

Clone this project (and [easygpio](https://github.com/eadf/esp8266_easygpio)) inside your own project and point your MODULES variable in the Makefile to those folders. 

<b>For a single device, connect as follows DHT22/DHT11 to ESP:</b><br>
DHT 1 (Vcc) to Vcc (3.3 Volts) ESP<br>
DHT 2 (DATA_OUT) to your ESP GPIO<br>
DHT 3 (NC)<br>
DHT 4 (GND) to ESP GND<br>
Connect a 5KΩ resistor between Vcc and the DATA_OUT pin. If you are using several sensors you will need one such pullup resistor per sensor.

You can connect any GPIO to a DHTxx sensor, including GPIO16. But i don't recommend using GPIO15. A DHT pullup resistor (on GPIO15) will cause the ESP to boot in "Boot from SD-card" mode.
