from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient
import paho.mqtt.client as mqtt
import json
import time
from datetime import datetime
import signal

# Set the callback function for MQTT messages
def on_message(_client, _userdata, message):
    """Handle messages"""

    print()

    date = datetime.now().strftime('%d/%m/%YT %H:%M:%S')

    print(date + ' - Received message: ' + str(message.payload))

    # Parse the incoming JSON string into a dictionary
    payload = json.loads(message.payload)

    if(payload['id']=='0'):
        #Convert the resulting dictionary back into a JSON string
        json_payload = json.dumps({
        'Datetime': date,
        'Flame': payload['flame'],
        'CO': payload['co'],
        'Temp': payload['temp']
        })

    # Topic will be MQTT_PUB_TOPIC_FIRE
    topic = MQTT_PUB_TOPIC_FIRE

    success = myMQTTClient.publish(topic, json_payload, 0)

    time.sleep(5)
    if(success):
        print("published",json_payload)

    
# On connect subscribe to topic
def on_connect(_client, _userdata, _flags, result):
    """Subscribe to input topic"""

    print('Connected ' + str(result))
    myMQTTClient.publish(MQTT_PUB_TOPIC_FIRE, "FIRST CONNECTION", 0)
    print("FIRST CONNECTION DONE")

    print('Subscribing to ' + MQTT_SUB_TOPIC)
    MQTT_CLIENT.subscribe(MQTT_SUB_TOPIC)

# Disconnect function
def disconnect_clients(signum, frame):
    MQTT_CLIENT.loop_stop()
    MQTT_CLIENT.disconnect()
    myMQTTClient.disconnect()
    print("DISCONNECTION")
    exit(0)

# Register signal handler for CTRL+C
signal.signal(signal.SIGINT, disconnect_clients)

# MQTT broker settings
MQTT_BROKER_ADDR = "192.168.177.168"
MQTT_BROKER_PORT = 1883
MQTT_BROKER_CLIENT_ID = "broker"

# AWS IoT settings
AWS_IOT_ENDPOINT ="a1qrujr4l90wwz-ats.iot.us-east-1.amazonaws.com"
AWS_IOT_PORT = 8883
AWS_IOT_CLIENT_ID = "basicPubSub"

# Set the relative path to the AWS IoT Root CA file
AWS_IOT_ROOT_CA = "./root-CA.crt"

# Set the relative path to the AWS IoT Private Key file
AWS_IOT_PRIVATE_KEY = "./fire-sensor.private.key"

# Set the relative path to the AWS IoT Certificate file
AWS_IOT_CERTIFICATE = "./fire-sensor.cert.pem"

# For certificate based connection
myMQTTClient = AWSIoTMQTTClient(AWS_IOT_CLIENT_ID)

# Configurations
# For TLS mutual authentication
myMQTTClient.configureEndpoint(AWS_IOT_ENDPOINT, 8883)
myMQTTClient.configureCredentials(AWS_IOT_ROOT_CA, AWS_IOT_PRIVATE_KEY, AWS_IOT_CERTIFICATE)

myMQTTClient.configureOfflinePublishQueueing(-1)  # Infinite offline Publish queueing
myMQTTClient.configureDrainingFrequency(2)  # Draining: 2 Hz
myMQTTClient.configureConnectDisconnectTimeout(10)  # 10 sec
myMQTTClient.configureMQTTOperationTimeout(5)  # 5 sec

#TOPIC
MQTT_SUB_TOPIC = "home"
MQTT_PUB_TOPIC_FIRE = "data/fire"

# Create a MQTT client instance
MQTT_CLIENT = mqtt.Client(client_id=MQTT_BROKER_CLIENT_ID)

# MQTT callback function
def main():
    MQTT_CLIENT.on_connect = on_connect
    MQTT_CLIENT.on_message = on_message
    MQTT_CLIENT.connect(MQTT_BROKER_ADDR, MQTT_BROKER_PORT)
    myMQTTClient.connect()
    MQTT_CLIENT.loop_forever()

if __name__ == '__main__':
    main()
