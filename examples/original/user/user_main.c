/*
 *  Example read temperature and humidity from DHT22
 *  
 *  https://www.sparkfun.com/datasheets/Sensors/Temperature/DHT22.pdf
 *  https://www.adafruit.com/datasheets/Digital%20humidity%20and%20temperature%20sensor%20AM2302.pdf
 * 
 *  For a single device, connect as follows:
 *  DHT22 1 (Vcc) to Vcc (3.3 Volts)
 *  DHT22 2 (DATA_OUT) to ESP Pin GPIO2
 *  DHT22 3 (NC)
 *  DHT22 4 (GND) to GND
 *
 * Between Vcc and DATA_OUT need to connect a pull-up resistor of 10 kOh.
 *
 */

#include <ets_sys.h>
#include <osapi.h>
#include <os_type.h>
#include <gpio.h>
#include "stdout/stdout.h"
#include "dhtxx/dhtxx.h"

#define DHT_SAMPLE_PERIOD 2000 /* milliseconds */

static os_timer_t dht22_timer;
DHT_Sensor sensors[3];

static void ICACHE_FLASH_ATTR
dht22_timerf(void *arg) {

  static uint8_t i = 0;
  DHT_Sensor_Output r;

	if(dht_read(sensors+i, &r)) {
	  float lastTemp = r.temperature;
	  float lastHum = r.humidity;
		console_printf("Temperature: %d.%d *C, Humidity: %d.%d %%\r\n", (int)(lastTemp),(int)((lastTemp - (int)lastTemp)*100), (int)(lastHum),(int)((lastHum - (int)lastHum)*100));
	}	else {
		console_printf("Error reading temperature and humidity\r\n");
	}
	i=i>=2?0:i+1;
}

void dht22_setup(void) {
  dht_init(sensors+0, DHT22, 0); // GPIO0
  dht_init(sensors+1, DHT22, 2); // GPIO2
  dht_init(sensors+2, DHT22, 3); // GPIO3 (RX)

  os_timer_disarm(&dht22_timer);
  os_timer_setfn(&dht22_timer, (os_timer_func_t *)dht22_timerf, NULL);
  os_timer_arm(&dht22_timer, DHT_SAMPLE_PERIOD, true);
}

void ICACHE_FLASH_ATTR
user_init(void)
{
  // Make uart0 work with just the TX pin. Baud:115200,n,8,1
  // The RX pin is now free for GPIO use.
  stdout_init();

  // turn off WiFi for this console only demo
  wifi_station_set_auto_connect(false);
  wifi_station_disconnect();

  dht22_setup();
}
