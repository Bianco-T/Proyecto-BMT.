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
const float high_ppm= 600.0;
const uint cooler_pin=19;

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

        float co2_concentration = calcular_concentracion_co2(mq135_reading);
        float h2_concentration = calcular_concentracion_h2(mq135_reading);
        float c6h6_concentration = calcular_concentracion_c6h6(mq135_reading);
        float alcohol_concentration = calcular_concentracion_alchol(mq135_reading);

        char co2_str[15];
        sprintf(co2_str, "CO2 = %.1f ppm", co2_concentration); 
        char h2_str[15];
        sprintf(h2_str, "H2 = %.1f ppm", h2_concentration); 
        char c6h6_str[15];
        sprintf(c6h6_str, "c6h6 = %.1f ppm", c6h6_concentration);
        char alcohol_str[15];
        sprintf(alcohol_str, "alcohol =%.1f ppm", alcohol_concentration);
        //muestra temp y humedad en el display
        lcd_set_cursor_pos(&my_lcd, 0,0);  //reglon 1 del lcd
        lcd_string(&my_lcd, ftemp);
        lcd_set_cursor_pos(&my_lcd, 1,0); // reglon 2 del lcd
        lcd_string(&my_lcd, humid);

        sleep_ms(2000); // pausa por 2 secs para evitar que el lcd se sature
        lcd_clear(&my_lcd);
        // muestra el valor en ppm de gas
       lcd_set_cursor_pos(&my_lcd, 0, 0);  // Línea 1 del LCD
       lcd_string(&my_lcd, co2_str);
       lcd_set_cursor_pos(&my_lcd, 1, 0);  // Línea 2 del LCD
       lcd_string(&my_lcd, h2_str);
       
        sleep_ms(2000); 
        lcd_clear(&my_lcd);

       control_cooler(co2_concentration, high_ppm);
      if (co2_concentration>high_ppm) {
        gpio_put(cooler_pin, 1);
        sleep_ms(2000);
      } else {
        gpio_put(cooler_pin, 0);
      }

    }
    return 0;

}
float convert_co2_ppm(float rs, float r0) {
  return (rs - r0) / r0 * 1000;
}
float convert_h2_ppm(float rs, float r0) {
  return (rs - r0) / r0 * 1000;
}
float convert_c6h6_ppm(float rs, float r0) {
  return (rs - r0) / r0 * 1000;
}
float convert_alcohol_ppm(float rs, float r0) {
  return (rs - r0) / r0 * 1000;
}




    

    