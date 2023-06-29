#include <string.h>
#include <stdbool.h>
#include "timex.h"
#include "xtimer.h"
#include "ztimer.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "stdio.h"
#include "periph/gpio.h"
#include "periph/adc.h"
#include "thread.h"
#include "analog_util.h"

int BOARD = 0; // 0->sensor 1->actuator

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

char publish_stack[THREAD_STACKSIZE_MAIN];
kernel_pid_t publish_thread_aws;

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

//When Wi-Fi drivers are enabled ADC2 cannot be used. Only 8 channels of ADC1 can be used.
//mosquitto_sub -h 192.168.177.168 -p 1883 -t "mytopic"
//sudo BOARD=esp32-heltec-lora32-v2 BUILD_IN_DOCKER=1 DOCKER="sudo docker" PORT=/dev/ttyUSB0 make flash term

#define MAIN_QUEUE_SIZE     (8)

#define BUF_SIZE                        1024
#define MQTT_VERSION_v311               4       /* MQTT v3.1.1 version is 4 */
#define COMMAND_TIMEOUT_MS              4000

#ifndef DEFAULT_MQTT_CLIENT_ID
#define DEFAULT_MQTT_CLIENT_ID          ""
#endif

#ifndef DEFAULT_MQTT_USER
#define DEFAULT_MQTT_USER               ""
#endif

#ifndef DEFAULT_MQTT_PWD
#define DEFAULT_MQTT_PWD                ""
#endif

#define DEFAULT_MQTT_PORT               1883
#define DEFAULT_IP                      "192.168.177.168"
#define DEFAULT_TOPIC                   "mytopic"
#define DEFAULT_TOPIC_AWS               "home"

#define DEFAULT_KEEPALIVE_SEC           10

#ifndef MAX_LEN_TOPIC
#define MAX_LEN_TOPIC                   100
#endif

#ifndef MAX_TOPICS
#define MAX_TOPICS                      4
#endif

#define IS_CLEAN_SESSION                1
#define IS_RETAINED_MSG                 0

static MQTTClient client;
static Network network;

static unsigned char buf[BUF_SIZE];
static unsigned char readbuf[BUF_SIZE];

int ret = -1;
int daily_send=0;
char* mex;
int fire=0;
char json[200];
char* msg;
char* message_on_aws;
int value_button_pin;

static int connect(void)
{

    /* ensure client isn't connected in case of a new connection */
    if (client.isconnected) {
        printf("mqtt_example: client already connected, disconnecting it\n");
        MQTTDisconnect(&client);
        NetworkDisconnect(&network);
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = MQTT_VERSION_v311;

    data.clientID.cstring = DEFAULT_MQTT_CLIENT_ID;
    data.username.cstring = DEFAULT_MQTT_USER;
    data.password.cstring = DEFAULT_MQTT_PWD;
    data.keepAliveInterval = DEFAULT_KEEPALIVE_SEC;
    data.cleansession = IS_CLEAN_SESSION;
    data.willFlag = 0;

    printf("mqtt_example: Connecting to MQTT Broker from %s %d\n",
            DEFAULT_IP, DEFAULT_MQTT_PORT);
    printf("mqtt_example: Trying to connect to %s, port: %d\n",
            DEFAULT_IP, DEFAULT_MQTT_PORT);
    ret = NetworkConnect(&network, DEFAULT_IP, DEFAULT_MQTT_PORT);
    if (ret < 0) {
        printf("mqtt_example: Unable to connect\n");
        return ret;
    }

    printf("user:%s clientId:%s password:%s\n", data.username.cstring,
             data.clientID.cstring, data.password.cstring);
    ret = MQTTConnect(&client, &data);
    if (ret < 0) {
        printf("mqtt_example: Unable to connect client %d\n", ret);
        return ret;
    }
    else {
        printf("mqtt_example: Connection successfully\n");
    }

    return (ret > 0) ? 0 : 1;
}

void* publish_on_aws(void* arg){
    while(1){
        enum QoS qos = QOS2;
        char* topic=DEFAULT_TOPIC_AWS;
        MQTTMessage message;
        message.qos = qos;
        message.retained = IS_RETAINED_MSG;
        message.payload = message_on_aws;
        message.payloadlen = strlen(message.payload);

        int rc;
        if ((rc = MQTTPublish(&client, topic, &message)) < 0) {
            printf("mqtt_example: Unable to publish (%d)\n", rc);
        }
        else {
            printf("mqtt_example: Message (%s) has been published to topic %s"
            "with QOS %d\n",
            (char *)message.payload, topic, (int)message.qos);
        }
        thread_sleep();
    }
    return 0;
}

static int publish(char* mex){
    enum QoS qos = QOS2;
    char* topic=DEFAULT_TOPIC;
    MQTTMessage message;
    message.qos = qos;
    message.retained = IS_RETAINED_MSG;
    message.payload = mex;
    message.payloadlen = strlen(message.payload);

    int rc;
    if ((rc = MQTTPublish(&client, topic, &message)) < 0) {
        printf("mqtt_fire_sensor: Unable to publish (%d)\n", rc);
    }
    else {
        printf("mqtt_fire_sensor: Message (%s) has been published to topic %s"
         "with QOS %d\n",
        (char *)message.payload, topic, (int)message.qos);
    }
    return 0;
}

void* led(void* arg){
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

static void _on_msg_received(MessageData *data)
{
    printf("\npaho_mqtt_example: message received on topic"
           " %.*s: %.*s\n",
           (int)data->topicName->lenstring.len,
           data->topicName->lenstring.data, (int)data->message->payloadlen,
           (char *)data->message->payload);

    mex = (char *)data->message->payload;
    //thread_wakeup(thread_pub);
    printf("%s\n",mex);
    if((strcmp (mex, "helo"))==0){
        printf("received helo message, the system works\n");
        fire=0;
    }
    else if((strcmp (mex, "fire"))==0){
        printf("received fire message, there is a fire\n");
        fire=1;    
        thread_wakeup(thread_buzzer);
        thread_wakeup(thread_led);
    }
    else{
        printf("received NO-fire message, there is nothing\n");
        fire=0;
        printf("\n");
    }
    memset(mex,0,strlen(mex));

}

void sub_to_topic(void)
{
    enum QoS qos = QOS0;
    printf("usage: %s <topic name> [QoS level]\n",DEFAULT_TOPIC);

    printf("mqtt_example: Subscribing to %s\n", DEFAULT_TOPIC);
    int ret = MQTTSubscribe(&client,DEFAULT_TOPIC, qos, _on_msg_received);
    if (ret < 0) {
        printf("mqtt_example: Unable to subscribe to %s (%d)\n",
               DEFAULT_TOPIC, ret);
        //_cmd_discon(0, NULL);
    }
    else {
        printf("\nmqtt_example: Now subscribed to %s, QOS %d\n",
               DEFAULT_TOPIC, (int) qos);
     
    }

    return;
}

int main(void)
{
    printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);

    NetworkInit(&network);

    xtimer_sleep(3);

    MQTTClientInit(&client, &network, COMMAND_TIMEOUT_MS, buf, BUF_SIZE,
                   readbuf,
                   BUF_SIZE);

    printf("Running fire-alarm-sensor example.\n");
  
    xtimer_sleep(3);

    MQTTStartTask(&client);

    xtimer_sleep(2);

    connect();
    
    if(ret<0){
        connect();
        xtimer_sleep(1);
    }

    if(BOARD==1){

        sub_to_topic();

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
        // initialize the BUZZER_PIN line
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

        publish_thread_aws=thread_create(publish_stack, sizeof(publish_stack),
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_SLEEPING,
                  publish_on_aws, NULL, "publish_on_received");   

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
            if(ret<0){
                connect();
                xtimer_sleep(1);
            }    

            int sample_fire=0;        
            int flame_sensor=0;
            int sample_co=0;        
            int co_sensor=0;
            int sample_temp=0;        
            int temp_sensor=0;
            
            sample_co = adc_sample(CO_PIN,ADC_RES_CO);
            xtimer_sleep(1); 
            sample_fire = adc_sample(FLAME_PIN,ADC_RES_FIRE);
            xtimer_sleep(1);
            sample_temp = adc_sample(TEMP_PIN,ADC_RES_TEMP);
            
            flame_sensor = adc_util_map(sample_fire, ADC_RES_FIRE, 85, -25);
            co_sensor = adc_util_map(sample_co, ADC_RES_CO, 10, 10000);
            temp_sensor = adc_util_map(sample_temp, ADC_RES_TEMP, 125, -55);

            if(flame_sensor>=50 && co_sensor>=2000){ //sistemare threshold
                printf("\nFIRE --- FIRE --- FIRE\n");
                printf("Raw FLAME %i, value FLAME: %i \t Raw CO %i, value CO: %i \t Raw TEMP %i, value TEMP: %i \n", sample_fire, flame_sensor, sample_co, co_sensor, sample_temp, temp_sensor);
                
                sprintf(json, "{\"id\": \"%d\", \"flame\": \"%d\", \"co\": \"%d\", \"temp\": \"%d\"}",
                            1, flame_sensor, co_sensor, temp_sensor);
                message_on_aws=json;
                thread_wakeup(publish_thread_aws);
              
                msg = "fire";
                publish(msg);

                last_time = xtimer_now();
            }
                // ---- ONLY FOR DEBUG ---- //
            else{
                printf("Raw FLAME %i, value FLAME: %i \t Raw CO %i, value CO: %i \t Raw TEMP %i, value TEMP: %i \n", sample_fire, flame_sensor, sample_co, co_sensor, sample_temp, temp_sensor);
                sprintf(json, "{\"id\": \"%d\", \"flame\": \"%d\", \"co\": \"%d\", \"temp\": \"%d\"}", 0, flame_sensor, co_sensor, temp_sensor);
                message_on_aws=json;
                thread_wakeup(publish_thread_aws);

                msg = "no fire";
                publish(msg);
  
                //last_time = xtimer_now(); 
            }
            
            now_time=xtimer_now(); 
            diff_time=xtimer_diff(now_time, last_time);
            if(diff_time > DAILY_TIME){ //microsecondi
                msg="helo";
                publish(msg);  
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
            if(ret<0){
                connect();
                xtimer_sleep(1);
            }  
        }
    }
    
    return 0;

}




    
