#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hardware/adc.h"
#include "libs/inc/lcd1602_i2c.h"
#include "libs/inc/dht.h"



const uint LED_PIN = PICO_DEFAULT_LED_PIN;
const uint DHT_PIN = 15;
const uint MAX_TIMINGS = 85;
const uint mq135 = 26;

int main() {
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_init(DHT_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    adc_init();
    adc_gpio_init(mq135);
    adc_select_input(0);
    

    LCD my_lcd;
    lcd_init(&my_lcd, 0x27, 4, 5, i2c0);


    while (1) {
        uint16_t lectura_mq = adc_read(); // valor analogico que se lee del sensor de gas

        dht_reading reading;
        read_from_dht(&reading, LED_PIN, DHT_PIN, MAX_TIMINGS);
        char ftemp[15];
        sprintf(ftemp, "Temp = %.1f C", reading.temp_celsius); // aca convierto los floats en chars
        char humid[15];
        sprintf(humid, "Hum = %.1f%%", reading.humidity);
        char gas[15];
        sprintf(gas,"CO2 = %i%% ppm", lectura_mq); // aca convierto un int en char  
        //muestra temp y humedad en el display
        lcd_set_cursor_pos(&my_lcd, 0,0);  //reglon 1 del lcd
        lcd_string(&my_lcd, ftemp);
        lcd_set_cursor_pos(&my_lcd, 1,0); // reglon 2 del lcd
        lcd_string(&my_lcd, humid);

        sleep_ms(2000); // pausa por 2 secs para evitar que el lcd se sature
        lcd_clear(&my_lcd);
        // muestra el valor en ppm de gas
        lcd_string(&my_lcd, gas);
        sleep_ms(2000); 
        lcd_clear(&my_lcd);

    }
    return 0;
}


    

    