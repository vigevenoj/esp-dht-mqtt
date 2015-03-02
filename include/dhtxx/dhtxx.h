/*
 Driver for the temperature and humidity sensor DHT11 and DHT22
 Official repository: https://github.com/CHERTS/esp8266-dht11_22

 Copyright (C) 2014 Mikhail Grigorev (CHERTS)

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along
 with this program; if not, write to the Free Software Foundation, Inc.,
 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __DHT22_H__
#define __DHT22_H__

#include "c_types.h"

typedef enum {
  DHT11, DHT22
} DHTType ;

typedef struct {
  float temperature;
  float humidity;
} DHT_Sensor_Output;

typedef struct {
  uint8_t pin;
  DHTType type;
} DHT_Sensor;

/**
 * quick and dirty os_sprintf("%.2f", value) (two decimals).
 * You better be sure that your buffer can hold the string, because there is no snprintf()
 * in the sdk.
 */
char* dht_float2String(char* buffer, float value);

/**
 * read the sensor (blocking function)
 */
bool dht_read(DHT_Sensor *sensor, DHT_Sensor_Output* output);

/**
 * Initializes the sensor, sets up the GPIO as output
 */
bool dht_init(DHT_Sensor *sensor, DHTType dht_type, uint8_t pin);

#endif
