# FOFISET Prototype - MQTT code

## Introduction
In this directory there is the code for the MQTT Bridge, this has to be executed in the device that you want to use as bridge for AWS. <br/>

## How to run the code
Firstly, you have to download mosquitto, go to the site and follow the instructions:  [Link Here](https://github.com/eclipse/mosquitto) 

Secondly, you have to connect your computer and AWS following this guide: [Link Here](https://aws.amazon.com/it/blogs/iot/how-to-bridge-mosquitto-mqtt-broker-to-aws-iot/). 

Launch the program of your ESP32 like it is written in Code_Device Directory. Open another terminal, go in the correct directory and launch the command:

    cd /FOFIDES/Code_MQTT_Python_Bridge/
    python3 Code_MQTT_Python_bridge.py
    
This script take the values sent by the ESP32s and redirect them to AWS.
