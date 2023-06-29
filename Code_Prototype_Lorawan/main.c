#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xtimer.h"
#include "ztimer.h"
#include "net/loramac.h"
#include "semtech_loramac.h"
#include "sx127x.h"
#include "sx127x_netdev.h"
#include "sx127x_params.h"
#include "random.h"
#include <stdbool.h>
#include "timex.h"
#include "xtimer.h"
#include "ztimer.h"
#include "periph/gpio.h"
#include "periph/adc.h"
#include "thread.h"
#include "fmt.h"
#include "analog_util.h"

int BOARD = 0; // 0->sensor 1->actuator

static semtech_loramac_t loramac;
static sx127x_t sx127x;
static uint8_t deveui[LORAMAC_DEVEUI_LEN];
static uint8_t appeui[LORAMAC_APPEUI_LEN];
static uint8_t appkey[LORAMAC_APPKEY_LEN];

#define CO_PIN ADC_LINE(2) //pin37
#define ADC_RES_CO ADC_RES_12BIT

#define FLAME_PIN ADC_LINE(0) //pin36
#define ADC_RES_FIRE ADC_RES_12BIT

#define TEMP_PIN ADC_LINE(1) //pin39
#define ADC_RES_TEMP ADC_RES_12BIT

gpio_t BUTTON_PIN = GPIO_PIN(0, 16);
#define BUTTON_PIN_NUMBER 16

char stack_led[THREAD_STACKSIZE_MAIN];
kernel_pid_t thread_led;

char stack_buzzer[THREAD_STACKSIZE_MAIN];
kernel_pid_t thread_buzzer;

gpio_t LED_PIN = GPIO_PIN(0, 2); //pin2
#define LED_PIN_NUMBER 2

gpio_t BUZZER_PIN = GPIO_PIN(0, 15); //pin23
#define BUZZER_PIN_NUMBER 15

xtimer_ticks32_t periodic_time;
xtimer_ticks32_t last_time;
xtimer_ticks32_t now_time;
xtimer_ticks32_t diff_time;

#define DAILY_TIME 86400*US_PER_SEC //time of a day -> 60*60*24*US_PER_SEC

#define HIGH_RISK_PERIODIC 5*US_PER_SEC
#define MIDDLE_RISK_PERIODIC 10*US_PER_SEC
#define SMALL_RISK_PERIODIC 60*US_PER_SEC

//mosquitto_sub -h 192.168.177.168 -p 1883 -t "mytopic"
//sudo BOARD=esp32-heltec-lora32-v2 BUILD_IN_DOCKER=1 DOCKER="sudo docker" PORT=/dev/ttyUSB0 make flash term

int ret = -1;
int daily_send=0;
char* mex;
int fire=0;
char json[200];
char* message;
int value_button_pin;

void* led(void* arg){
    (void) arg;
    while(1){
        printf("Set led to HIGH \n");
        gpio_set(LED_PIN);
        xtimer_sleep(2);
        printf("Set led to LOW \n");
        gpio_clear(LED_PIN);
        xtimer_sleep(2);
        if (fire==0){
            thread_sleep();
        }
    }    
}

void* buzzer(void* arg){
    (void) arg;
    while(1){
        printf("Set buzzer to HIGH \n");
        gpio_set(BUZZER_PIN); 
        xtimer_sleep(2);
        printf("Set buzzer to LOW \n");
        gpio_clear(BUZZER_PIN);
        xtimer_sleep(2);  
        if (fire==0){
            thread_sleep();
        }
    }    
}

static void send_message(void)
{

    printf("Sending: %s\n", message);
    uint8_t ret = semtech_loramac_send(&loramac, (uint8_t *)message, strlen(message));
    while(ret!= SEMTECH_LORAMAC_TX_DONE){
    ret = semtech_loramac_send(&loramac, (uint8_t *)message, strlen(message));
    }
    printf("Message sent!");
    
    if (ret != SEMTECH_LORAMAC_TX_DONE)  {
        printf("Cannot send message '%s', ret: %d\n", message, ret);
        return;
    }
}


int main(void)
{
    printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);

    sx127x_setup(&sx127x, &sx127x_params[0], 0);
    loramac.netdev = &sx127x.netdev;
    loramac.netdev->driver = &sx127x_driver;

    semtech_loramac_init(&loramac);

    fmt_hex_bytes(deveui, CONFIG_LORAMAC_DEV_EUI_DEFAULT);
    fmt_hex_bytes(appeui, CONFIG_LORAMAC_APP_EUI_DEFAULT);
    fmt_hex_bytes(appkey, CONFIG_LORAMAC_APP_KEY_DEFAULT);
    semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);

    semtech_loramac_set_dr(&loramac, LORAMAC_DR_5);

    if (!semtech_loramac_is_mac_joined(&loramac)) {

        printf("Starting join procedure");
        if (semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED) {
            printf("Join procedure failed");
            return;
        }
    }
    printf("Join procedure succeeded");

    if(BOARD==1){

        thread_led=thread_create(stack_led, sizeof(stack_led),
                    THREAD_PRIORITY_MAIN, THREAD_CREATE_SLEEPING,
                    led, NULL, "led_service");

        thread_buzzer=thread_create(stack_buzzer, sizeof(stack_buzzer),
                    THREAD_PRIORITY_MAIN, THREAD_CREATE_SLEEPING,
                    buzzer, NULL, "buzzer_service");           
        
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
        if (gpio_init(BUTTON_PIN, GPIO_IN_PU)) {
            printf("Error to initialize GPIO_PIN(%d %d) failed\n", 0, BUTTON_PIN_NUMBER);
            return -1;
        } else {
            printf("Successfully initialized GPIO_PIN(%d %d)\n", 0, BUTTON_PIN_NUMBER);
        }
        
    }
    
    else if(BOARD==0){
        // initialize the FLAME_PIN (ADC) line
        if (adc_init(FLAME_PIN) < 0) {
            printf("Initialization of ADC_LINE(%u) failed\n", FLAME_PIN);
            return 1;
        } else {
            printf("Successfully initialized ADC_LINE(%u)\n", FLAME_PIN);
        }
        
        // initialize the CO_PIN (ADC) line
        if (adc_init(CO_PIN) < 0) {
            printf("Initialization of ADC_LINE(%u) failed\n", CO_PIN);
            return 1;
        } else {
            printf("Successfully initialized ADC_LINE(%u)\n", CO_PIN);
        }
        // initialize the TEMP_PIN (ADC) line
        if (adc_init(TEMP_PIN) < 0) {
            printf("Initialization of ADC_LINE(%u) failed\n", TEMP_PIN);
            return 1;
        } else {
            printf("Successfully initialized ADC_LINE(%u)\n", TEMP_PIN);
        }
    }
    
    periodic_time = xtimer_now();

    while(1){        
        if(BOARD==0){

            int sample_fire=0;        
            int flame_sensor=0;
            int sample_co=0;        
            int co_sensor=0;
            int sample_temp=0;        
            int temp_sensor=0;
            
            sample_co = adc_sample(CO_PIN,ADC_RES_CO);
            sample_fire = adc_sample(FLAME_PIN,ADC_RES_FIRE);
            sample_temp = adc_sample(TEMP_PIN,ADC_RES_TEMP);
            
            flame_sensor = adc_util_map(sample_fire, ADC_RES_FIRE, 85, -25);
            co_sensor = adc_util_map(sample_co, ADC_RES_CO, 10, 10000);
            temp_sensor = adc_util_map(sample_temp, ADC_RES_TEMP, 125, -55);
            
            if(flame_sensor>=50 && co_sensor>=2000){
                printf("\nFIRE --- FIRE --- FIRE\n");
                printf("Raw FLAME %i, value FLAME: %i \t Raw CO %i, value CO: %i \t Raw TEMP %i, value TEMP: %i \n", sample_fire, flame_sensor, sample_co, co_sensor, sample_temp, temp_sensor);
                
                sprintf(json, "{\"id\": \"%d\", \"flame\": \"%d\", \"co\": \"%d\", \"temp\": \"%d\"}",
                            1, flame_sensor, co_sensor, temp_sensor);
                message=json;            
                send_message();
                last_time = xtimer_now();
            }
                // ---- ONLY FOR DEBUG ---- //
            else{
                printf("Raw FLAME %i, value FLAME: %i \t Raw CO %i, value CO: %i \t Raw TEMP %i, value TEMP: %i \n", sample_fire, flame_sensor, sample_co, co_sensor, sample_temp, temp_sensor);
                sprintf(json, "{\"id\": \"%d\", \"flame\": \"%d\", \"co\": \"%d\", \"temp\": \"%d\"}", 0, flame_sensor, co_sensor, temp_sensor);
                message=json;            
                // NO MESSAGE - send_message();
                last_time = xtimer_now(); 
            }
            
            now_time=xtimer_now(); 
            diff_time=xtimer_diff(now_time, last_time);
            if(diff_time > DAILY_TIME){ //microseconds
                message="helo";   
                send_message();
                last_time = xtimer_now();
            }

            if (temp_sensor < 30){
                xtimer_periodic_wakeup(&periodic_time, SMALL_RISK_PERIODIC);
            }  
            else if (temp_sensor < 70){
                xtimer_periodic_wakeup(&periodic_time, MIDDLE_RISK_PERIODIC);
            }  
            else{
                xtimer_periodic_wakeup(&periodic_time, HIGH_RISK_PERIODIC);
            }  
        }
        
        else if(BOARD==1){
            if(fire==1){ //only if there is an alarm	
                value_button_pin = gpio_read(BUTTON_PIN);
                //printf("%d\n",value_button_pin);
                if(value_button_pin==0){
                    fire=0;
                 }
            }
            xtimer_sleep(1);
        }
    }
    
    return 0;

}
