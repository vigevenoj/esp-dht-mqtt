# ESP8266 DHTxx to MQTT

This program uses [esp\_mqtt](https://github.com/tuanpmt/esp_mqtt), [esp8266\_dhtxx](https://github.com/eadf/esp8266_dhtxx), and [esp8266\_easygpio](https://github.com/eadf/esp8266_easygpio) to read a DHTxx sensor and publish readings to an MQTT topic

## Installation
Clone the repository including the subtree modules.

Update your $PATH to include the xtensa-lx106-elf/bin directory

Set $SDK\_BASE to the esp\_iot\_sdk location

## Usage
Add configuration in localinclude/user\_config.h. This file needs to contain the topics to which you will publish sensor data, so
```#define TEMPERATURE_TOPIC ""
#define HUMIDITY_TOPIC ""```
in order to get those into the user_main. MQTT broker and wifi configuration go in mqtt/include/user_config.local.h

To build,
`make clean && make`

To flash onto the esp8266 hardware, set ESPPORT and make flash like this:
`make ESPPORT=dev/tty.usbserial-AFJRNX35 flash`

## Troubleshooting
If you mess up and a merge/pull from the mqtt repo overwrites all the other updates, 
fix it by removing the whole repo, cloning it again, and then adding these remotes:
`git remote add -f -t master --no-tags easygpio https://github.com/eadf/esp8266_easygpio.git`
`git remote add -f -t master --no-tags dhtxx https://github.com/eadf/esp8266_dhtxx.git`
`git remote add -f -t master --no-tags mqtt https://github.com/tuanpmt/esp_mqtt.git`
Then, remove the subtree subdirectories so we can add them later
`rm -rf dhtxx`
`rm -rf mqtt`
`rm -rf easygpio`

read the esp_mqtt repository's master branch into the 'mqtt' subdirectory
`git read-tree --prefix=mqtt/  mqtt/master -u`
read the dhtxx repository's master branch into the dhtxx subdirectory
`git read-tree --prefix=dhtxx/ dhtxx/master -u`
read the easygpio repository's master branch into the easygpio subdirectory
`git read-tree --prefix=easygpio/ easygpio/master -u`

References: http://stackoverflow.com/questions/23937436/add-subdirectory-of-remote-repo-with-git-subtree
How to build the sdk: see https://github.com/pfalcon/esp-open-sdk


## License
MIT License. 
