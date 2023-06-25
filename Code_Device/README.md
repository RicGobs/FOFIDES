# FOFIDES Prototype - RIOT code

## Introduction
In this directory there is the RIOT code for the prototype. Before to do this tutorial, do the actions explained in the directory of Code_MQTT_Python_Bridge.
<br/>

Download RIOT from github:

    git clone https://github.com/RIOT-OS/RIOT.git

Go in RIOT folder and then in the directory /example, download here the code, go in the directory of the Code_Device:

    cd RIOT/examples/
    git clone https://github.com/RicGobs/FOFIDES.git
    cd /FOFIDES/Code_Device/
    
Now try to do make of the code with command (change type of board and port if needed):

    sudo BOARD=esp32-heltec-lora32-v2 BUILD_IN_DOCKER=1 DOCKER="sudo docker" PORT=/dev/ttyUSB0 make all

If all works, connect your board and do (repeat the same thing for the other board, changing the value of the variable "BOARD" to 0 for Sensor or 1 for Actuator):

    sudo BOARD=esp32-heltec-lora32-v2 BUILD_IN_DOCKER=1 DOCKER="sudo docker" PORT=/dev/ttyUSB0 make flash term

Now your ESP32 are working and are sending messages to the MQTT topic selected. Open another terminal, go in the correct directory and launch the command:

    cd /FOFIDES/Code_MQTT_Python_Bridge/
    python3 Code_MQTT_Python_bridge.py
    
This script take the values sent by the ESP32s and redirect them to AWS.

### Common errors
For this project you need two different boards, one for the Sensor and one for the Actuator. <br/>
Before launching the code, change the private values of the code like information of the WI-FI and AWS properties.
