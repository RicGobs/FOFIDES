/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       fire alarm application
 *
 * @author      Riccardo Gobbato 
 *
 * @}
 */

#include "stdio.h"
#include "periph/gpio.h"
#include "periph/adc.h"
#include "analog_util.h"
#include "xtimer.h"

#include "dht.h"
#include "dht_params.h"
#include "fmt.h"

#define FLAME_SENSOR_PIN ADC_LINE(0)
#define ADC_RES ADC_RES_12BIT

gpio_t LED_PIN = GPIO_PIN(0, 12);
#define LED_PIN_NUMBER 12

gpio_t BUZZER_PIN = GPIO_PIN(0, 2);
#define BUZZER_PIN_NUMBER 2


int main(void)
{
    puts("Hello World!");

    printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
    printf("This board features a(n) %s MCU.\n", RIOT_MCU);
    
    
    // initialize the ADC line
    if (adc_init(FLAME_SENSOR_PIN) < 0) {
        printf("Initialization of ADC_LINE(%u) failed\n", FLAME_SENSOR_PIN);
        return 1;
    } else {
        printf("Successfully initialized ADC_LINE(%u)\n", FLAME_SENSOR_PIN);
    }
    
    if (gpio_init(LED_PIN, GPIO_OUT)) {
                printf("Error to initialize GPIO_PIN(%d %d) failed\n", 0, LED_PIN_NUMBER);
                return -1;
    } else {
        printf("Successfully initialized GPIO_PIN(%d %d)\n", 0, LED_PIN_NUMBER);
    }

    if (gpio_init(BUZZER_PIN, GPIO_OUT)) {
            printf("Error to initialize GPIO_PIN(%d %d) failed\n", 0, BUZZER_PIN_NUMBER);
            return -1;
    } else {
        printf("Successfully initialized GPIO_PIN(%d %d)\n", 0, BUZZER_PIN_NUMBER);
    }
    
    printf("\n");

    int sample=0;
    int flame_sensor_val=0;
    while (1) {
        // read the flame sensor value
        sample = adc_sample(FLAME_SENSOR_PIN,ADC_RES);
        flame_sensor_val = adc_util_map(sample, ADC_RES, 100, 0);


        // print the value on the console
        printf("Raw value %i \t Fire value: %i \n", sample, flame_sensor_val);

        if(flame_sensor_val>=90){
            printf("Set led to HIGH ---- ");
            gpio_set(LED_PIN);

            printf("Set buzzer to HIGH\n\n");
            gpio_set(BUZZER_PIN);
        }
        else{
            printf("Set led to LOW ---- ");
            gpio_clear(LED_PIN);

            printf("Set buzzer to LOW\n\n");
            gpio_clear(BUZZER_PIN);
        }
        xtimer_sleep(2);
        
    }
    

    
    /*
    ----------------THIS PART DOES NOT WORK-----------------------
    dht_params_t my_params;
    my_params.pin =  GPIO_PIN(0, 25);
    my_params.in_mode =  DHT_PARAM_PULL;

    dht_t dev;

    if (dht_init(&dev, &my_params) == DHT_OK) {
        printf("DHT sensor connected\n");
    }
    else {
        printf("Failed to connect to DHT sensor\n");
        return 1;
    }

    
    xtimer_sleep(1);
    int16_t temp, hum;
    
    while(1){
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            int a=dht_read(&dev, &temp, &hum);
            printf("%d\n",a);
            printf("Error read\n");
        }   
        
        char temp_s[10];
        size_t n = fmt_s16_dfp(temp_s, temp, -1);
        temp_s[n] = '\0';

        char hum_s[10];
        n = fmt_s16_dfp(hum_s, hum, -1);
        hum_s[n] = '\0';
        
        printf("DHT values - temp: %s°C - relative humidity: %s%%\n",
        temp_s, hum_s);
        //printf("DHT values - temp: %d°C - relative humidity: %d%%\n",
        //temp, hum);
        
        xtimer_sleep(3);
    }
    */  


    return 0;
}
