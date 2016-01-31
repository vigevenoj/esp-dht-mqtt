
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
static char humidityTopic[40] = "sensors/harold/feather/humidity"; // This probably belongs in user_config
static char temperatureTopic[40] = "sensors/harold/feather/temperature"; // Again, user_config

static void setup(void);
static void loop(void);

void ICACHE_FLASH_ATTR
wifiConnectCb(uint8_t status) {
  
  if (status == STATION_GOT_IP) {
    os_printf("HAVE IP ADDRESS, will connect to mqtt...\n");
    MQTT_Connect(&mqttClient);
  } 
}

void ICACHE_FLASH_ATTR
mqttConnectedCb(uint32_t *args) {
  MQTT_Client* client = (MQTT_Client*)args;
  os_printf("Connected to broker\n");
}

/**
 * callback to tell if mqtt is busy or not
 */
void ICACHE_FLASH_ATTR
mqttPublishedCb(uint32_t *args) {
  mqttIsIdle = true;
  os_printf("Published message\n");
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
    os_printf("in idle loop\n");
    if (dht_read(sensors+0, &o)) {
      os_printf("Reading sensors\n");

      if (oldHumidity != o.humidity || oldTemperature != o.temperature) { 
        char temperatureBuff[20];
        char humidityBuff [20];
        char temperatureMessage[64] = "{ \"type\" : \"temperature\", \"value\" : %s, \"units\" : \"C\" }";
        char humidityMessage[64] = "{ \"type\" : \"humidity\", \"value\" : %s, \"units\" : \"percent\" }";
        os_printf("Current temperature is %s\n", dht_float2String(temperatureBuff, o.temperature));
        os_printf("Current humidity is    %s %%\n", dht_float2String(humidityBuff, o.humidity));
        os_printf("{\"type\": \"humidity\", \"value\" : %s }\n", dht_float2String(humidityBuff, o.humidity));
        os_printf("{\"type\" : \"temperature\", \"value\" : %s }\n", dht_float2String(temperatureBuff, o.temperature));
        os_sprintf(temperatureMessage, "{\"type\" : \"temperature\", \"value\" : %s }", dht_float2String(temperatureBuff, o.temperature));
        os_sprintf(humidityMessage, "{\"type\": \"humidity\", \"value\" : %s }", dht_float2String(humidityBuff, o.humidity));
        mqttIsIdle = false;
        sentMessage = true;
        MQTT_Publish(&mqttClient, temperatureTopic, temperatureMessage, strlen(temperatureMessage), 0, 0); 
        MQTT_Publish(&mqttClient, humidityTopic, humidityMessage, strlen(humidityMessage), 0, 0); 
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
  os_delay_us(1000000);
  CFG_Load();
  dht_init(sensors+0, GPIO12_DHT_TYPE, 12);

  MQTT_InitConnection(&mqttClient, sysCfg.mqtt_host, sysCfg.mqtt_port, sysCfg.security);
  MQTT_InitClient(&mqttClient, sysCfg.device_id, sysCfg.mqtt_user, sysCfg.mqtt_pass, sysCfg.mqtt_keepalive, 1);
  MQTT_InitLWT(&mqttClient, "/lwt", "offline", 0, 0);
  MQTT_OnConnected(&mqttClient, mqttConnectedCb);
  MQTT_OnPublished(&mqttClient, mqttPublishedCb);
  WIFI_Connect(sysCfg.sta_ssid, sysCfg.sta_pwd, wifiConnectCb);

  // Start loop timer
  os_timer_disarm(&loop_timer);
  os_timer_setfn(&loop_timer, (os_timer_func_t *) loop, NULL);
  os_timer_arm(&loop_timer, SAMPLE_PERIOD, false); 
}

void ICACHE_FLASH_ATTR
user_init(void) {
  // Set up the UART or get garbage output
  uart_div_modify(0, UART_CLK_FREQ / 115200);
  // Set the setup timer
  os_delay_us(1000000);
  os_timer_disarm(&loop_timer);
  os_timer_setfn(&loop_timer, (os_timer_func_t *) setup, NULL);
  os_timer_arm(&loop_timer, 1000, false);
}
