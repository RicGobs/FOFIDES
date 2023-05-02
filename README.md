# Fire-Alarm-System

## Introduction
Forest fires pose a significant danger to both human settlements and forest ecosystems worldwide, with their occurrence largely linked to human activities. The resulting devastation often includes climatic changes and the exacerbation of the greenhouse effect. To mitigate these negative impacts, it is crucial to identify forest fires early on. This study suggests a wireless sensor network system that can detect forest fires at their initial stages.

You need to identify an application scenario that you would like to address. What are you going to learn by monitoring the environment, which type of sensors will help you? How will you act into the environment and by what actuators?

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

## Architecture
![Circuit1](https://github.com/RicGobs/Fire-Alarm-System/blob/main/circuit.png) <br/>


