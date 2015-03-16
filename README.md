ESP8266 - Driver for the temperature and humidity sensor DHT11 and DHT22
========================================================================

I've modified CHERTS's driver to allow for any number of sensors on any pin (one sensor per pin).<br>
I also made this project into a pure driver.<br>

Clone this project inside your own project and point your MODULES variable in the Makefile to that folder. 

<b>For a single device, connect as follows DHT22/DHT11 to ESP:</b><br>
DHT 1 (Vcc) to Vcc (3.3 Volts) ESP<br>
DHT 2 (DATA_OUT) to your GPIO ESP<br>
DHT 3 (NC)<br>
DHT 4 (GND) to GND ESP<br>
Between the terminal Vcc and DATA_OUT you should connect a 5KΩ resistor.

You can connect any GPIO to a DHTxx sensor, including GPIO16. But i don't recommend using GPIO15. A pullip resistor on GPIO15 will cause the ESP to boot in "Boot from SD-card" mode.