#ifndef _DHT_H_
#define _DHT_H_

#include "pico/stdlib.h"
#include "hardware/gpio.h"




typedef struct {
    float humidity;
    float temp_celsius;
} dht_reading;

void read_from_dht(dht_reading *result, uint LED_PIN, uint DHT_PIN, uint MAX_TIMINGS);


#endif  /* _DHT_H_ */