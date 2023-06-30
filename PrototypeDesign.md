# Prototype Design

![cable](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/cables.JPG) <br>

This is the prototype of the infrastructure that I have created to allow firefighting. In the image there are two separate elements: Sensor Board (in the right side)and Actuator Board (in the left side). In the file, it is possible to understand all the parts of the system. There is also a circuit presentation to better explain how to create the system.

## Components
Our infrastructure is composed of:

* Esp32-heltec-lora32-v2, for Sensor Board
* Esp32-wroom-32, for Actuator Board
* KY-028, temperature sensor module
* KY-026, infrared flame sensor
* MQ7 sensor, CO detector
* Buzzer, which is activated when fire is detected
* Led, which is activated when fire is detected
* Button, to stop the alarm

### Esp32 Sensor Board 
<img src="https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/esp32_heltec_lora.png" width="160" height="130"> <br>

Here there is the **Esp32-heltec-lora32-v2**. <br>

Sensor Board: it does the samplings (detecting possible fires), using KY-028 (temperature sensor module), KY-026 (infrared flame sensor) and MQ7 sensor (CO detector).

### Esp32 Actuator Board
<img src="https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/esp32_wroom.jpeg" width="160" height="130"> <br>

Here there is the **Esp32-wroom-32** . It is possible to use another Esp32-heltec-lora32-v2 if you want. <br>

Actuator Board: it receives the alert of fire and starts the alarm, using Buzzer and Led; it is possible to stop the alarm using a Button.

### KY-028, temperature sensor module
<img src="https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/temp.jpeg" width="160" height="130"> <br>
The KY-028 Digital Temperature Sensor measures temperature changes based on thermistor resistance. This module has both digital and analog outputs, there’s a potentiometer to adjust the detection threshold on the digital interface. This module consists of an NTC thermistor, an LM393 dual differential comparator, a 3296W trimmer potentiometer, 6 resistors, 2 LEDs, and 4 male header pins.
The specifications of the flame sensor include the following:
* Operating Voltage : 3.3V to 5.5V
* Temperature Measurement Range : -55°C to 125°C
* Measurement Accuracy : ±0.5°C
* Board Dimensions : 15mm x 36mm 
* The output type is Digital o/p or Digital & Analog output <br>

### Infrared Flame Sensor
<img src="https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/flame.jpeg" width="160" height="130"> <br>

Infrared Flame sensor is used to monitor light intensity; the values of light intensity, equal to detection of fire, are periodicaly sent to the Cloud.
This sensor is available in small size and is used to detect a source of fire or any other clear light source. Basically, this kind of sensor detects infrared light with 760 nm to 1100 nm range wavelength that is generated from the light source or fire or flame. This IR flame sensor includes a YG1006 Phototransistor sensor which has high sensitivity & high speed.
The specifications of the flame sensor include the following:
* Operating Voltage: 3.3V to 5V
* Operating current: 15 mA
* Comparator chip used: LM393
* Type of sensor: YG1006 Photo Transistor
* The output type is Digital o/p or Digital & Analog output
* Red LED is for power and green LED is for output
* Range of the spectrum: from 760nm to 1100nm
* Detection angle: from 0 to 60 degrees
* Operating temperature ranges: -25℃ to 85℃

### MQ7 Sensor
<img src="https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/mq7.jpg" width="160" height="130"> <br>
Carbon Monoxide Gas Sensor MQ-7 detects the concentrations of CO in the air and ouputs its reading as an analog voltage. The sensor can measure concentrations of 10 to 10,000 ppm.The sensor can operate at temperatures from -10 to 50°C and consumes less than 150 mA at 5 V.
The specifications of the flame sensor include the following:
* Detection Gas: Carbon Monoxide
* Concentration: 10-10000ppm
* Supply Voltage: < 10V
* Heater Voltage: 5.0V ± 0.2V
* Load Resistance: Adjustable
* Heater Resistance: 31Ω ± 3Ω
* Heater Consumption: < 350mW

## Circuit
There are two different circuits for the system, one for the actuator board and one for the sensor board.
### Circuit Actuator Board
![esp32_wroom](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/board_actuator.png) <br>
### Circuit Sensor Board
![esp32_wroom](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/board_sensor.png) <br>

## Network Architecture for Prototype with MQTT Protocol
The network architecture is focused on checking the actual state of the fire detection system.

![net_arch](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/Network_Architecture.png) <br>

The AWS architecture above shows the network design with MQTT Protocol. In detail, data are generated from the prototype and sent to AWS through a MQTT-Bridge, that is a my personal computer. Meanwhile, always using MQTT, Sensor Board sends a message to Actuator Board, which starts the alarm (the alarm can be stopped with a Button). With a proper AWS Lambda function and an IoT-Rule, the data sent to AWS from the MQTT-Bridge are stored in two NoSQL DB. With another Lambda function, data are retrieved form DBs and associated to an API deployed with AWS API Gateway. The website frontend, taken data dynamically from API endpoint, has been deployed with AWS Amplify. 

## Network Architecture for Prototype with LoRa Protocol
In a second part of the project I have worked with LoRa, which will be the protocol that I will use also in the future development of the project.
LoRa is a suitable technology for different reasons:
* It uses low bandwidth. In fact, I need to exchange simple data
* It can send data on long ranges. It is needed to allow the scalability of the project
* It works with low power consumption, and this is a crucial added value since the project is based on a battery-powered approach

The network architecture is focused on checking the actual state of the fire detection system.

![net_arch](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/lora_ttn_aws.png) <br>

The AWS architecture above shows the network design with LoRa Protocol. In detail, data are generated from the prototype and sent to AWS through The Things Network, that is the LoRa Gateway. 

In this part I have not already implemented the sending to the Actuator, because in my system the second Esp32, that I own, has not the Antenna LoRa. To solve this issue, you can change the Lambda function that takes the data from TTN adding a mqtt publish to the topic in which the Actuator is subscribed. 

Then, as before, with a proper AWS Lambda function and an IoT-Rule, the data sent to AWS from TTN are stored in two NoSQL DB. With another Lambda function, data are retrieved form DBs and associated to an API deployed with AWS API Gateway. The website frontend, taken data dynamically from API endpoint, has been deployed with AWS Amplify. 

## Website Page
![website1](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/website1.png) <br>

![website2](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/website2.png) <br>

The website shows the data sent to AWS, in particular it tells what happens during the actual fire showing graphs about Flame, Carbon Monoxide and Temp. There are also some aggregated data like Average, Min and Max of all the data. Here it is a demonstration of website content.
