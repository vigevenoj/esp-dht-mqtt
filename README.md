ESP8266 - Driver for the temperature and humidity sensor DHT11 and DHT22
========================================================================

I've modified CHERTS's driver to allow for any number of sensors on any pin.
I also made this project into a pure driver, clone this project inside your own project and point your MODULES variable in the Makefile to that folder. 

<b>For a single device, connect as follows DHT22 to ESP:</b><br>
DHT22 1 (Vcc) to Vcc (3.3 Volts) ESP<br>
DHT22 2 (DATA_OUT) to your GPIO ESP<br>
DHT22 3 (NC)<br>
DHT22 4 (GND) to GND ESP-01<br>
Between the terminal Vcc and DATA_OUT need to connect a pullup resistor of 5K ohms.

