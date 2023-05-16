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
* Discarding lit cigarettes
* Improperly burning debris
* Playing with matches or fireworks
* Prescribed fires
However, sometimes Wildfires do sometimes occur naturally, either ignited by the sun's heat or a lightning strike.

Most fires have historically occurred between May and October. However, recent data has shown that the season is lengthening, with wildfires starting earlier in the year and lasting well into the fall and winter months.

Some ecosystems were hit harder by nighttime activity than others. For instance, nighttime fire detections were dominant in temperate evergreen forests, where 38 percent of fire detections occurred at night.

Wildfires spread at an average of 14.27 miles per hour.

seasons analysis??????

Daily analysis???????

## Actual forest fire system
The Copernicus Emergency Management Services is the fire forest system used in Europe. Similar to the other active fire maps, the European Forest Fire Information System (EFFIS) uses MODIS and VIIRS to track thermal anomalies. As part of their current situation viewer, it assesses the burnt area of the forest fire with a polygon extent. Also, it provides an analysis of seasonal trends and the number of fires by European Union countries. For fire prevention managers, this platform forecasts fire danger levels based on meteorological predictions. The program cost €5,421 billion (2021-2027), Copernicus (io non lo metterei). Copernicus initial operations began in 2011. Copernicus became fully operational in 2014.  

Values taht I will compare:
* 22 m resolution imagery over very large areas for the 2nd generation of this type of satellites, the first generation is in space from 2002 to 2009 (only 32 m) and it lived for 7 years
* Near real-time detection of fire, but it is not declared specifically
* A satellite has a useful lifetime of between 5 and 15 years depending on the satellite

Also Google Earth Pro and NASA’s Worldview and FIRMS.
They use Photo-interpretation, Semi-automatic extraction, Automatic extraction and Modelling.
These ones detect active fire data within three hours of satellite observation.

## My of the problem



## Sensors
Two different type of sensors are used:
 DHT11;
 Infrared Flame sensor;
 //and MQ9 sensor,to monitor CO level.

### DHT11 Sensor
The DHT11 is used to monitor temperature and relative humidity; the values of temperature and humidity are periodicaly sent to the Cloud.
According to the manufacturer's datasheet, the temperature range measured by the sensor is 0-50°C with an accuracy of ±2°C. The humidity range is 20-90% RH with an accuracy of ±5% RH. The unit of temperature measurement is Celsius (°C), and the unit for humidity measurement is percentage (%). The desired periodicity of measurements can be set by the user and depends on the application's requirements. However, the datasheet suggests a minimum time interval of 2 seconds between consecutive measurements to ensure accurate readings.
In general:
* 3 to 5V power and I/O
* 2.5mA max current use during conversion (while requesting data)
* Good for 20-80% humidity readings with 5% accuracy
* Good for 0-50 °C temperature readings +-2 °C accuracy
* No more than 1 Hz sampling rate (once every second)
* Body size 15.5mm x 12mm x 5.5mm
* 3 pins with 0.1" spacing
* Adafruit Learning Documentation for DHTxx Sensors
* RoHS compliant

### Infrared Flame Sensor
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

## Actuators
Two type of actuators are used:
Buzzer, which is activated when fire is detected;
Led, which is activated when fire is detected.

The values, taken by the sensors, are sended to the Cloud via LoRaWAN.

## Battery
I am trying to understand which are the consumption of the system. In general the firefighting devices work between 3 and 10 years. Due to the difficulty placement of my system, the device should work for 10 years.

## Circuit
![Circuit1](https://github.com/RicGobs/Fire-Alarm-System/blob/main/circuit.png) <br/>

## Architecture
Wildfires spread at an average of 14.27 miles per hour. However, this can vary hugely depending on a number of factors, such as weather conditions, fuel type, and terrain.
23 km/h worst case conditions. 6,38889 m/s. When is already big.

The detection distance of the flame of lighter is 100 cm.

My test are done with the lighter so I will use this distance.

I have thought an height of 1.73 but it is too much, fire too big.


