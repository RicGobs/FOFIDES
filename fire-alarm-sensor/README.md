# Fire-Alarm-System

## Introduction
Forest fires pose a significant danger to both human settlements and forest ecosystems worldwide, with their occurrence largely linked to human activities. The resulting devastation often includes climatic changes and the exacerbation of the greenhouse effect. To mitigate these negative impacts, it is crucial to identify forest fires early on. This study suggests a wireless sensor network system that can detect forest fires at their initial stages.

Wildfires can fizzle out quickly or spread uncontrolled, consuming thousands of acres of land in a matter of hours. But the intensity and movement of a wildfire ultimately depends on three factors: fuel, weather and topography. These factors are collectively known as the “fire behavior triangle.”

High temperatures and low humidity also dry out fuel sources, causing them to ignite and burn faster. This is why wildfires typically become more intense and spread fastest in the afternoon, when the air is hottest

Wildfires can start with a natural occurrence—such as a lightning strike—or a human-made spark. However, it is often the weather conditions that determine how much a wildfire grows. Wind, high temperatures, and little rainfall can all leave trees, shrubs, fallen leaves, and limbs dried out and primed to fuel a fire.

While many plants and animals need and benefit from wildfires, climate change has left some ecosystems more susceptible to flames. Warmer temperatures have intensified drought and dried out forests.

Lastly, the Copernicus Emergency Management Services targets users in Europe. Similar to the other active fire maps, the European Forest Fire Information System (EFFIS) uses MODIS and VIIRS to track thermal anomalies.

As part of their current situation viewer, it assesses the burnt area of the forest fire with a polygon extent. Also, it provides an analysis of seasonal trends and the number of fires by European Union countries. For fire prevention managers, this platform forecasts fire danger levels based on meteorological predictions.
Also Google Earth Pro and NASA’s Worldview and FIRMS.
They use Photo-interpretation, Semi-automatic extraction, Automatic extraction and Modelling.
The program cost €5,421 billion (2021-2027), Copernicus (io non lo metterei). Copernicus initial operations began in 2011. Copernicus became fully operational in 2014. 
22 m resolution imagery over very large areas (2nd generation of this type of satellites). I will use 2 m.
active fire data within three hours of satellite observation.

the first generation is in space from 2002 to 2009 (only 32 m).
7 years (1st). 
A satellite has a useful lifetime of between 5 and 15 years depending on the satellite. It's hard to design them to last much longer than that, either because the solar arrays stop working or because they run out of fuel to allow them to maintain the orbit that they're supposed to be in.

So it is ok to do an average, I want that my system works for 10 years.

Common causes for wildfires include:
Arson
Campfires
Discarding lit cigarettes
Improperly burning debris
Playing with matches or fireworks
Prescribed fires
However, sometimes Wildfires do sometimes occur naturally, either ignited by the sun's heat or a lightning strike.

Prime time for wildfire depends on summer temperatures, rainfall, and wind. In California, most fires have historically occurred between May and October. However, recent data has shown that the season is lengthening, with wildfires starting earlier in the year and lasting well into the fall and winter months.

seasons analysis??????

Ddaily analysis???????
Some ecosystems were hit harder by nighttime activity than others. For instance, nighttime fire detections were dominant in temperate evergreen forests, where 38 percent of fire detections occurred at night. Cropland fires, however, mainly occurred during the daytime.
Nighttime fires have become more intense and more frequent in recent decades, as hot, dry nights are more commonplace.

mex lora 1 al giorno con temperatura a 13.00. Come analisi di pericolo e controllo della situazione, allerta.

Ogni quanto misurare? 
Wildfires spread at an average of 14.27 miles per hour. However, this can vary hugely depending on a number of factors, such as weather conditions, fuel type, and terrain.
23 km/h worst case conditions. 6,38889 m/s. When is already big.
It can be good do 5 min of duty cicle.
quindi bisogna vedere bene ogni quanto misurare, se fai ogni 5 min?
Meglio comunque di Copernicus, quindi va bene.


There are different type of flame sensors:
Infrared Flame Sensors
UV Flame Sensors
Ultraviolet/Infrared Flame Detectors
Multi-Spectrum Infrared Flame Detectors
IR3 Flame Detector
I have used an Infrared Flame Sensors, because in the forest with the fire there is always the flame.
-------------

https://cnr.ncsu.edu/news/2021/12/explainer-how-wildfires-start-and-spread/#:~:text=Wildfires%20do%20sometimes%20occur%20naturally,discarded%20cigarettes%2C%20arson%20and%20more.

The detection distance of the flame of lighter is 100 cm.
My test are done with the lighter so I will use this distance.

I have thought an height of 1.73 but it is too much, fire too big.



## Sensors
Two different type of sensors are used:
 DHT11;
 Infrared Flame sensor;
 //and MQ9 sensor,to monitor CO level.

### DHT11 Sensor
The DHT11 is used to monitor temperature and relative humidity; the values of temperature and humidity are periodicaly sent to the Cloud.
According to the manufacturer's datasheet, the temperature range measured by the sensor is 0-50°C with an accuracy of ±2°C. The humidity range is 20-90% RH with an accuracy of ±5% RH. The unit of temperature measurement is Celsius (°C), and the unit for humidity measurement is percentage (%). The desired periodicity of measurements can be set by the user and depends on the application's requirements. However, the datasheet suggests a minimum time interval of 2 seconds between consecutive measurements to ensure accurate readings.

### Infrared Flame Sensor
Infrared Flame sensor is used to monitor light intensity; the values of light intensity, equal to detection of fire, are periodicaly sent to the Cloud.
This sensor is available in small size and is used to detect a source of fire or any other clear light source. Basically, this kind of sensor detects infrared light with 760 nm to 1100 nm range wavelength that is generated from the light source or fire or flame. This IR flame sensor includes a YG1006 Phototransistor sensor which has high sensitivity & high speed.
The specifications of the flame sensor include the following.
The range of operating voltage ranges from 3.3V to 5V.
The operating current is 15 mA.
The comparator chip used is LM393.
The type of sensor is YG1006 Photo Transistor.
Sensitivity can be adjusted by a potentiometer.
The output type is Digital o/p or Digital & Analog output.
Red LED is for power and green LED is for output.
The range of the spectrum is from 760nm to 1100nm
The detection angle is from 0 to 60 degrees.
Operating temperature ranges from -25℃ to 85℃.
The size of PCB is 3cm X 1.6cm.

## Actuators
Two type of actuators are used:
Buzzer, which is activated when fire is detected;
Led, which is activated when fire is detected.

The values, taken by the sensors, are sended to the Cloud via LoRaWAN.


