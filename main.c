/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       fire-Alarm-System
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

#include "gpio_dht.h"

#define FLAME_PIN ADC_LINE(0)
#define ADC_RES ADC_RES_12BIT

gpio_t LED_PIN = GPIO_PIN(0, 12);
#define LED_PIN_NUMBER 12

gpio_t BUZZER_PIN = GPIO_PIN(0, 23);
#define BUZZER_PIN_NUMBER 23

gpio_t TEMPERATURE_PIN = GPIO_PIN(0, 2);
int start_value=0;

int main(void)
{
    //puts("Hello World!");

    printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
    printf("This board features a(n) %s MCU.\n\n", RIOT_MCU);

    while(1){
        
        xtimer_msleep(2000);//wait_for_dht11();
        start_signal(TEMPERATURE_PIN);
        read_dht11(TEMPERATURE_PIN);
                
        if(start_value==0){
            // initialize the FLAME_PIN (ADC) line
            if (adc_init(FLAME_PIN) < 0) {
                printf("Initialization of ADC_LINE(%u) failed\n", FLAME_PIN);
                return 1;
            } else {
                printf("Successfully initialized ADC_LINE(%u)\n", FLAME_PIN);
            }
            
            // initialize the LED_PIN line
            if (gpio_init(LED_PIN, GPIO_OUT)) {
                        printf("Error to initialize GPIO_PIN(%d %d) failed\n", 0, LED_PIN_NUMBER);
                        return -1;
            } else {
                printf("Successfully initialized GPIO_PIN(%d %d)\n", 0, LED_PIN_NUMBER);
            }

            // initialize the BUZZER_PIN line
            if (gpio_init(BUZZER_PIN, GPIO_OUT)) {
                    printf("Error to initialize GPIO_PIN(%d %d) failed\n", 0, BUZZER_PIN_NUMBER);
                    return -1;
            } else {
                printf("Successfully initialized GPIO_PIN(%d %d)\n", 0, BUZZER_PIN_NUMBER);
            }
            start_value++;
            printf("\n"); 
        }
        int sample=0;
        int flame_sensor_val=0;
        
        //while (1) {
        {
            // initialize the TEMPERATURE_PIN line

            xtimer_sleep(1);
            // read the flame sensor value
            sample = adc_sample(FLAME_PIN,ADC_RES);
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
    }

    return 0;
}
