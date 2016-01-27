
#include "ets_sys.h"
#include "osapi.h"

#include "mqtt.h"
#include "wifi.h"
#include "config.h"
#include "debug.h"
#include "user_config.h"
#include "easygpio/easygpio.h"
#include "dhtxx.c"

#define SAMPLE_PERIOD 10000 // 10,000 milliseconds / 10 seconds
#define GPIO12_DHT_TYPE DHT22

MQTT_Client mqttClient;
volatile static bool mqttIsIdle = true;
static volatile os_timer_t loop_timer;
DHT_Sensor sensors[1];
static char topic[40]; 
static char message[200];

static void setup(void);
static void loop(void);

void ICACHE_FLASH_ATTR
wifiConnectCb(uint8_t status) {
  
  if (status == STATION_GOT_IP) {
    os_printf("HAVE IP ADDRESS, will connect to mqtt...");
    MQTT_Connect(&mqttClient);
  } else {
    MQTT_Disconnect(&mqttClient);
  }

}

/**
 * callback to tell if mqtt is busy or not
 */
void ICACHE_FLASH_ATTR
mqttPublishedCb(uint32_t *args) {
  mqttIsIdle = true;
}

/**
 * Main user program loop
 */
void ICACHE_FLASH_ATTR
loop(void) {
  static float oldHumidity = -2.0; // negative humidity is impossible
  static float oldTemperature = -32.0; // this is well outside the PDX temp range
  bool sentMessage = false;
  DHT_Sensor_Output o;

  if (mqttIsIdle) {
    os_printf("in idle loop");
    if (dht_read(sensors+0, &o)) {
      if (oldHumidity != o.humidity || oldTemperature != o.temperature) { 
        char buff[20];
        os_printf("Current temperature is %s", dht_float2String(buff, o.temperature));
        os_printf("Current humidity is    %s %%", dht_float2String(buff, o.humidity));
        os_printf(message, "[{'type': 'humidity', 'value' : %s }, 'type' : 'temperature', 'value' : %s ]", dht_float2String(buff, o.humidity), dht_float2String(buff, o.temperature));
        mqttIsIdle = false;
        sentMessage = true;
        os_printf("about to publish a message");
        MQTT_Publish(&mqttClient, topic, message, strlen(message), 0, 0); 
        oldHumidity = o.humidity;
        oldTemperature = o.temperature;
      } // No change in either sensor, try again later
    }  // Probably no reading?
  } else {
    // MQTT client is busy
  }

  // re-arm the system timer at the end of each iteration of the loop
  os_timer_arm(&loop_timer, SAMPLE_PERIOD, true);
}

void ICACHE_FLASH_ATTR
setup(void) {
  CFG_Load();

  os_printf("calling dht_init");
  dht_init(sensors+0, GPIO12_DHT_TYPE, 12);
  os_printf("finished dht_init");

  WIFI_Connect(sysCfg.sta_ssid, sysCfg.sta_pwd, wifiConnectCb);

  MQTT_InitConnection(&mqttClient, sysCfg.mqtt_host, sysCfg.mqtt_port, sysCfg.security);
  MQTT_InitClient(&mqttClient, sysCfg.device_id, sysCfg.mqtt_user, sysCfg.mqtt_pass, sysCfg.mqtt_keepalive, 1);
  MQTT_InitLWT(&mqttClient, "/lwt", "offline", 0, 0);
  MQTT_OnPublished(&mqttClient, mqttPublishedCb);

  // Start loop timer
  os_timer_disarm(&loop_timer);
  os_timer_setfn(&loop_timer, (os_timer_func_t *) loop, NULL);
  os_timer_arm(&loop_timer, SAMPLE_PERIOD, false); 
  os_printf("ran setup, starting main loop");
}

void ICACHE_FLASH_ATTR
user_init(void) {
  // Set up the UART or get garbage output
  uart_div_modify(0, UART_CLK_FREQ / 115200);
  // Set the setup timer
  os_delay_us(1000000);
  os_printf("running user init");
  os_timer_disarm(&loop_timer);
  os_timer_setfn(&loop_timer, (os_timer_func_t *) setup, NULL);
  os_timer_arm(&loop_timer, 1000, false);
}
