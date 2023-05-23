# Fire-Alarm-System

## Introduction
Forest fires pose a significant danger to both human settlements and forest ecosystems worldwide, with their occurrence largely linked to human activities. While many plants and animals need and benefit from wildfires, climate change has left some ecosystems more susceptible to flames. Warmer temperatures have intensified drought and dried out forests.

The resulting devastation often includes climatic changes and the exacerbation of the greenhouse effect. To mitigate these negative impacts, it is crucial to identify forest fires early on. This study suggests a wireless sensor network system that can detect forest fires at their initial stages.

## How forest fire works?
Wildfires can fizzle out quickly or spread uncontrolled, consuming thousands of acres of land in a matter of hours. But the intensity and movement of a wildfire ultimately depends on three factors:
* fuel
* weather
* topography
These factors are collectively known as the "fire behavior triangle".

High temperatures and low humidity also dry out fuel sources, causing them to ignite and burn faster. This is why wildfires typically become more intense and spread fastest in the afternoon, when the air is hottest.

Common causes for wildfires include:
* Arson
* Campfires 
![fire-percentage](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/fire-percentage.png) 
* Discarding lit cigarettes
* Improperly burning debris
* Playing with matches or fireworks
* Prescribed fires
However, sometimes Wildfires do sometimes occur naturally, either ignited by the sun's heat or a lightning strike.


Most fires have historically occurred between May and October. However, recent data has shown that the season is lengthening, with wildfires starting earlier in the year and lasting well into the fall and winter months.

![fire-season](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/fire-season.png) <br>

Some ecosystems were hit harder by nighttime activity than others. For instance, nighttime fire detections were dominant in temperate evergreen forests, where 38 percent of fire detections occurred at night.

![fire-region](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/fire-region.jpeg) <br>

Wildfires spread at an average of 14.27 miles per hour.

seasons analysis??????

Daily analysis???????

## Actual forest fire system
The Copernicus Emergency Management Services is the fire forest system used in Europe. Similar to the other active fire maps, the European Forest Fire Information System (EFFIS) uses MODIS and VIIRS to track thermal anomalies. As part of their current situation viewer, it assesses the burnt area of the forest fire with a polygon extent. Also, it provides an analysis of seasonal trends and the number of fires by European Union countries. For fire prevention managers, this platform forecasts fire danger levels based on meteorological predictions. The program cost €5,421 billion (2021-2027), Copernicus (io non lo metterei). Copernicus initial operations began in 2011. Copernicus became fully operational in 2014.  

![fire-map](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/fire-map.png) <br>

Values taht I will compare:
* 22 m resolution imagery over very large areas for the 2nd generation of this type of satellites, the first generation is in space from 2002 to 2009 (only 32 m) and it lived for 7 years
* Near real-time detection of fire, but it is not declared specifically
* A satellite has a useful lifetime of between 5 and 15 years depending on the satellite

Also Google Earth Pro and NASA’s Worldview and FIRMS.
They use Photo-interpretation, Semi-automatic extraction, Automatic extraction and Modelling.
These ones detect active fire data within three hours of satellite observation.

## Board

![esp32](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/esp32.png) <br>

## Sensors
Two different type of sensors are used:
* KY-028, temperature sensor module
* KY-026, infrared flame sensor
* MQ7 sensor, CO detector

### KY-028, temperature sensor module

![temp](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/temp.jpeg) <br>

Calibration of the sensor, using the DHT11.

<img src="https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/dht11-temp.png" width="440" height="320"> <br>

### Infrared Flame Sensor

![flame](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/flame.jpeg) <br>
Infrared Flame sensor is used to monitor light intensity; the values of light intensity, equal to detection of fire, are periodicaly sent to the Cloud.
This sensor is available in small size and is used to detect a source of fire or any other clear light source. Basically, this kind of sensor detects infrared light with 760 nm to 1100 nm range wavelength that is generated from the light source or fire or flame. This IR flame sensor includes a YG1006 Phototransistor sensor which has high sensitivity & high speed.
The specifications of the flame sensor include the following:
* The range of operating voltage ranges from 3.3V to 5V
* The operating current is 15 mA
* The comparator chip used is LM393
* The type of sensor is YG1006 Photo Transistor
* Sensitivity can be adjusted by a potentiometer
* The output type is Digital o/p or Digital & Analog output
* Red LED is for power and green LED is for output
* The range of the spectrum is from 760nm to 1100nm
* The detection angle is from 0 to 60 degrees
* Operating temperature ranges from -25℃ to 85℃
* The size of PCB is 3cm X 1.6cm
* you can detect Infrared Light up to a distance of 100cm within its 60 degrees of detection angle
* While capable of detecting fires and explosions within 3–4 milliseconds, a time delay of 2–3 seconds is often included to minimize false alarms which can be triggered by other UV sources such as lightning, arc welding, radiation, and sunlight

### MQ7 Sensor
<img src="https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/mq7.jpg" width="150" height="120"> <br>

## Actuators
Two type of actuators are used:
Buzzer, which is activated when fire is detected;
Led, which is activated when fire is detected.

The values, taken by the sensors, are sended to the Cloud via LoRaWAN.

## Battery
I am trying to understand which are the consumption of the system. In general the firefighting devices work between 3 and 10 years. Due to the difficulty placement of my system, the device should work for 10 years.

## Circuit

## Architecture
Wildfires spread at an average of 14.27 miles per hour. However, this can vary hugely depending on a number of factors, such as weather conditions, fuel type, and terrain.
23 km/h worst case conditions. 6,38889 m/s. When is already big.
My test are done with the lighter so I will use this distance.

![angle](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/angle.png)

I have thought an height of 1.73 but it is too much, fire too big.

![scale1](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/scale1.png)

The detection distance of the flame of lighter is 100 cm.

![scale2](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/scale2.png)

In the end, I have scaled the architecture.

