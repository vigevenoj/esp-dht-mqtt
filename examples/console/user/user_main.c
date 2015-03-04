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

#define DHT_SAMPLE_PERIOD 500 /* 0.5 seconds between each sensor x7 sensors = 3.5 seconds period on each sensor  */
#define DHT_NUMBER_OF_SENSORS 7

static os_timer_t dht22_timer;
DHT_Sensor sensors[DHT_NUMBER_OF_SENSORS];

static void ICACHE_FLASH_ATTR
dht22_timerf(void *arg) {

  static uint8_t i = 0;
  DHT_Sensor_Output o;

  if(dht_read(sensors+i, &o)) {
    char buff[20];
    os_printf("Temperature: %s *C",dht_float2String(buff, o.temperature));
    os_printf(" Humidity: %s %%",dht_float2String(buff, o.humidity));
    os_printf(" [GPIO%d]\n", sensors[i].pin);
  } else {
    os_printf("Failed to read temperature and humidity sensor on GPIO%d\n", sensors[i].pin);
  }
  i=i>=(DHT_NUMBER_OF_SENSORS-1)?0:i+1;
}

void ICACHE_FLASH_ATTR
dht22_setup(void) {

  dht_init(sensors+0, DHT22, 0);  // GPIO0
  dht_init(sensors+1, DHT22, 2);  // GPIO2
  dht_init(sensors+2, DHT22, 4);  // GPIO4
  dht_init(sensors+3, DHT22, 5);  // GPIO5
  dht_init(sensors+4, DHT22, 12); // GPIO12
  dht_init(sensors+5, DHT22, 13); // GPIO13
  dht_init(sensors+6, DHT22, 14); // GPIO14

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
