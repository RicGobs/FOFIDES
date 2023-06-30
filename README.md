# FOFIDES - A Forest Fire Detection System for our world

## Prototype

![presentation](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/presentation.JPG) <br>

## Index of the project 
In the repository all the parts of the project are explained. 

There are some explanations:
* **PrototypeDesign**, with all the properties of the Prototype
* **ProtoypeEvaluation**, with the performance of the system
* **PrototypeScalability**, with an analysis for the the future and the scalability of the project
* A small **Introduction** to the problem and the current situation (below)

There are also the codes:
* **Code_Device_MQTT Directory**, with all the RIOT code for the Esp32 with MQTT Protocol
* **Code_Device_Lorawan Directory**, with all the RIOT code with LoRa Protocol
* **Code_Lambda_AWS Directory**, with all the code for the lambda functions
* **Code_MQTT_Python_Bridge Directory**, with the script for creating a MQTT Bridge
* **Images Directory**, with all the images

## Video And Tutorial
See the [Video Presentation](https://youtu.be/uon6HLHgz9U) on YouTube.

See the [Project Presentation](https://www.hackster.io/gobbatoriccardo96/fofides-forest-fire-detection-system-acda9b) on Hackster.io.

See the [Tutorial](https://www.hackster.io/gobbatoriccardo96/build-an-iot-infrastructure-from-the-thing-to-aws-8f370b) on Hackster.io.

## Introduction of the problem
Forest fires pose a significant danger to both human settlements and forest ecosystems worldwide, with their occurrence largely linked to human activities. While many plants and animals need and benefit from wildfires, climate change has left some ecosystems more susceptible to flames. Warmer temperatures have intensified drought and dried out forests.

The resulting devastation often includes climatic changes and the exacerbation of the greenhouse effect. To mitigate these negative impacts, it is crucial to identify forest fires early on. This study suggests a wireless sensor network system that can detect forest fires at their initial stages.

### How forest fire works?
Wildfires can fizzle out quickly or spread uncontrolled, consuming thousands of acres of land in a matter of hours. But the intensity and movement of a wildfire ultimately depends on three factors:
* fuel
* weather
* topography
These factors are collectively known as the "fire behavior triangle".

High temperatures and low humidity also dry out fuel sources, causing them to ignite and burn faster. This is why wildfires typically become more intense and spread fastest in the afternoon, when the air is hottest.

Common causes for wildfires include:
* Arson
* Campfires 
* Discarding lit cigarettes
* Improperly burning debris
* Playing with matches or fireworks
* Prescribed fires
However, sometimes Wildfires do sometimes occur naturally, either ignited by the sun's heat or a lightning strike. 

![wildfires](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/wildfires.png) <br>

Some ecosystems were hit harder by nighttime activity than others. For instance, nighttime fire detections were dominant in temperate evergreen forests, where 38 percent of fire detections occurred at night. 

![fire-region](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/fire-region.jpeg) <br>

## My solution

The Project aims to develop an IoT-based forest fire alarm system that utilizes a network of strategically placed sensors to continuously monitor environmental factors. When a potential fire hazard is detected, the system will send real-time notifications to authorities, automate emergency protocols, enable remote monitoring, and enhance coordination of firefighting efforts. This comprehensive solution enhances fire detection capabilities and facilitates swift and targeted response to combat forest fires. 
