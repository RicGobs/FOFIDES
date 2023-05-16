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

char stack_pub[THREAD_STACKSIZE_MAIN];
kernel_pid_t thread_pub;

char stack_led[THREAD_STACKSIZE_MAIN];
kernel_pid_t thread_led;

char stack_buzzer[THREAD_STACKSIZE_MAIN];
kernel_pid_t thread_buzzer;

#define CO_PIN ADC_LINE(2) //pin37
#define ADC_RES_CO ADC_RES_12BIT

#define FLAME_PIN ADC_LINE(0) //pin36
#define ADC_RES_FIRE ADC_RES_12BIT

#define TEMP_PIN ADC_LINE(1) //pin39
#define ADC_RES_TEMP ADC_RES_12BIT

gpio_t LED_PIN = GPIO_PIN(0, 2); //pin2
#define LED_PIN_NUMBER 2

gpio_t BUZZER_PIN = GPIO_PIN(0, 23); //pin23
#define BUZZER_PIN_NUMBER 23

//When Wi-Fi drivers are enabled ADC2 cannot be used. Only 8 channels of ADC1 can be used.
//mosquitto_sub -h 192.168.69.168 -p 1883 -t "mytopic"
//sudo BOARD=esp32-heltec-lora32-v2 BUILD_IN_DOCKER=1 DOCKER="sudo docker" PORT=/dev/ttyUSB0 make flash

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

/**
 * @brief Default MQTT port
 */
#define DEFAULT_MQTT_PORT               1883
#define DEFAULT_IP                      "192.168.92.168"
#define DEFAULT_TOPIC                   "mytopic"
#define DEFAULT_TOPIC_HOME              "home"

/**
 * @brief Keepalive timeout in seconds
 */
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
char* mex;
int ret = -1;
int fire=0;
char json[128];

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
/*
void* publish(void* arg){
    while(1){
    enum QoS qos = QOS2;
    char* topic=DEFAULT_TOPIC_HOME;
    MQTTMessage message;
    message.qos = qos;
    message.retained = IS_RETAINED_MSG;
    message.payload = mex;
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
*/
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
        printf("Set buzzer to LOW \n");
        gpio_clear(BUZZER_PIN);
        xtimer_sleep(2);
        printf("Set buzzer to HIGH \n");
        gpio_set(BUZZER_PIN);   
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
        fire=0;
        printf("Ciao Huston, ti sto inviando il saluto giornaliero così non ti senti solo\n");
    }
    else{
        printf("Oii ce sta er fogo\n");
        fire=1;    
        thread_wakeup(thread_buzzer);
        thread_wakeup(thread_led);
    }
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

/*
void unsub_to_topic(void)
{
    int ret = MQTTUnsubscribe(&client, DEFAULT_TOPIC);

    if (ret < 0) {
        printf("mqtt_example: Unable to unsubscribe from topic: %s\n", DEFAULT_TOPIC);
        //_cmd_discon(0, NULL);
    }
    else {
        printf("\nmqtt_example: Unsubscribed from topic:%s\n", DEFAULT_TOPIC);

    }
    return;
}
*/

int main(void)
{
    printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
    

    NetworkInit(&network);

    xtimer_sleep(3);

    MQTTClientInit(&client, &network, COMMAND_TIMEOUT_MS, buf, BUF_SIZE,
                   readbuf,
                   BUF_SIZE);

    printf("Running fire-alarm example.\n");
  
    xtimer_sleep(3);

    MQTTStartTask(&client);
 
    while(ret<0){
        connect();
        xtimer_sleep(1);
    }
/*
	thread_pub=thread_create(stack_pub, sizeof(stack_pub),
                  THREAD_PRIORITY_MAIN + 1, THREAD_CREATE_SLEEPING,
                  publish, NULL, "publish_service");
*/
    thread_led=thread_create(stack_led, sizeof(stack_led),
                  THREAD_PRIORITY_MAIN , THREAD_CREATE_SLEEPING,
                  led, NULL, "led_service");

    thread_buzzer=thread_create(stack_buzzer, sizeof(stack_buzzer),
                  THREAD_PRIORITY_MAIN , THREAD_CREATE_SLEEPING,
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


    sub_to_topic();

    //unsub_to_topic(); usefull if find a method to disconnect wii-fi

    while(1){
        xtimer_sleep(5);
    }

    return 0;

}




    