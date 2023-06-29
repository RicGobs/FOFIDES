# Prototype Evaluation

## Consideration about the Forest Fire

### Data of the problem 
As discussed in the introduction, Wildfires can fizzle out quickly or spread uncontrolled, consuming thousands of acres of land in a matter of hours. But the intensity and movement of a wildfire ultimately depends on three factors: fuel, weather and topography (fire behavior triangle). 

High temperatures and low humidity also dry out fuel sources, causing them to ignite and burn faster. This is why wildfires typically become more intense and spread fastest in the afternoon, when the air is hottest.

Common causes for wildfires include are Arson, Campfires, Discarding lit cigarettes, Improperly burning debris, Playing with matches or fireworks and Prescribed fires.
However, sometimes Wildfires do sometimes occur naturally, either ignited by the sun's heat or a lightning strike.

<br>
Most fires have historically occurred between May and October. However, recent data has shown that the season is lengthening, with wildfires starting earlier in the year and lasting well into the fall and winter months. 

![fire-season](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/fire-season.png) <br>

Wildfires spread at an average of 14.27 miles per hour. This means 23 km/h (6,38889 m/s). This are American analysis and this is the worst case scenario.
However, this can vary hugely depending on a number of factors, such as weather conditions, fuel type, and terrain.  

## Actual forest fire system
The Copernicus Emergency Management Services is the fire forest system used in Europe. Similar to the other active fire maps, the European Forest Fire Information System (EFFIS) uses MODIS and VIIRS to track thermal anomalies. As part of their current situation viewer, it assesses the burnt area of the forest fire with a polygon extent. Also, it provides an analysis of seasonal trends and the number of fires by European Union countries. For fire prevention managers, this platform forecasts fire danger levels based on meteorological predictions. The program cost €5,421 billion (2021-2027), Copernicus (io non lo metterei). Copernicus initial operations began in 2011. Copernicus became fully operational in 2014.  

![fire-map](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/fire-map.png) <br>

The values of the actual system, with which I have to compare, are:
* 22 m resolution imagery over very large areas for the 2nd generation of this type of satellites (while the first generation is in space from 2002 to 2009 (only 32 m) and it lived for 7 years)
* Near real-time detection of fire, but it is not declared specifically
* A satellite has a useful lifetime of between 5 and 15 years depending on the satellite

### Daily and Seasonal analysis
From the data take before, I have observed that is not possible to set in a static way different analysis for the seasons. Indeed the forest fire are not linked anymore to the seasons. 

It is possible to do some daily analysis observing the value of the temperature. Doing this, it is possible to understand a probabily of forest fire and beacuse of this, it is posssible to have a dynamic duty cicle, in which there are different sleep time:

* 5 minutes, for high temperature (high risk)
* 10 minutes, for middle temperature (middle risk)
* 15 minutes, for low temperature (low risk)

It is not suitable to sleep for more time because, in the case there is a fire, the system becomes unreliable.

With this system, there is also a different analysis (on average) between the seasons, and in the winter there will be usually a long duty cycle while in summer there will be a short duty cycle.

## Testing the Prototype

### Detection Performance
For the project I have tested the different sensors to set a correct threshold.

For the temperature sensor (KY-028) I have done some test to convert the analog input in a temperature value (in °C). Secondly, For the flame sensor I have done tests for the sensivity to the flame. Finally, for the MQ7 I have translated the input analog in a ppm value. 

#### Temperature Sensor KY-028 Calibration

![calibration](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/temp-calibration.jpg) <br>

The first idea, to calibrate the sensor, was about using the DHT11. In the image below, there is the temperature detected by dht11 in the white window, while there is temperature of the KY-028 not already calibrated in the black window.

![calibration](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/dht11-temp.png) <br>

I have done another test using a thermometer, to be sure that the DHT11 Sensor has correctly and accuratly taken the temperature. I have not directly used the DHT11 for the project cause of some problems between RIOT and the Sensor. In the following image, the KY-028 has been calibrated.

![calibration](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/calibration_temp.png) <br>

With this approach I has been able to calibrate quite well the temperature sensor. In particular I have fixed the temperature for the different duty cycles said before:
* high risk - more than 35°C
* middle risk - between 20°C and 35°C
* low risk - less than 20°C

#### Infrared Flame Sensor Calibration
I have also done some tests for the Infrared Flame Sensor, to control if and how works. Below there is an image to show the value received in a situation of NO-FLAME. <br>

![no](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/noflame.png) <br>

Here, on the contrary, there is an image to show the value received in a situation of YES-FLAME. It is also possible to see that the temperature sensor is sensible to the flame, indeed the temperature is increase than before. <br>

![yes](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/yesflame.png) <br>

#### MQ7 Sensor Calibration


### Reliable Architecture
To create a reliable architecture that allow a correct detection of the fire in less than 15 minutes, it is important to analyse to distance parameters of the system.

My test are done with the lighter so I will use the distance obtained by these tests to implement the infrastructure. 

The detection distance of the flame of lighter is 100 cm if there is a fire around the sensor, while it is higher (up to 2 m) if it is up the sensor. 

![angle](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/angle1.png) <br>

So I will use the lower value to create the infrastructure. If a sensor covers a circle of diameter = 1 m, It is needed a device each 2 m.
There will be a small area in which there is not a proper coverage, but it is not relevant thankfull to the redundancy of the system and the size of this uncover area.

![scale1](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/scale1.png) <br>


### Energy Performance with MQTT Protocol
In this paragraph, there is the energy performance of only the Sensor Board which will be connected to a battery. The Actuator Board is connected via cables and also it consumes more or less depending on how many alarms there are.

To obtain the energy performance of the Sensor Board it is needed to analyse the consumptions of the different parts of the system:
* KY-028 (temperature sensor): 5 V - 15mA = 0.075 W
* Infrared Flame sensor: 5 V - 15 mA = 0.075 W
* MQ7 sensor: 5 V - 150 mA = 0.75 W
* Esp32 in active mode: 5 V - 180 mA = 0.9 W
* Esp32 Heltec in deep sleep mode: 5V - 800µA = 0.004 W
* mex AWS consumption, counted in the Active mode
* mex Alarm consumption, counted in the Active mode

$$E_{tot} =  KY028comsumption + FlameConsumption + MQ7consumption + ActivemodeConsumption + DeepSleepModeConsumption $$

$$E_{tot} =  0.075 W \cdot TimeActiveMode + 0.075 W \cdot TimeActiveMode + 0.750 W \cdot TimeActiveMode + 0.9 W \cdot Time ActiveMode + 0.004 W \cdot Time DeepSleepMode$$

It is important to analyse the system in the worst case scenario, so it always wakes up every 5 minute: it does 288 samplings per day.

On average it takes 3 seconds to do the test, so 864 sec (0,24 h) in active mode and 85536 (23.76 h) sec in deep sleep mode.

$$E_{tot} daily =  0.075 W \cdot 0,24 h + 0.075 W \cdot 0,24 h + 0.750 W \cdot 0,24 h + 0.9 W \cdot 0,24 h + 0.004 W \cdot 23.76 h $$ 

$$E_{tot} daily =  0.018 Wh + 0.018 Wh + 0.18 Wh + 0.216 Wh + 0.095 Wh = 0.527 Wh $$ 

So, 0.289 Wh is the consumption per day of the system. It is interesting to understand for how much time it can survive. 

$$E_{tot} monthly = 0.527 Wh \cdot 30 days = 15.81 Wh $$ 


But in the datasheet of [ESP32 Series](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf), the Deep Sleep Mode is less, in particular it  is 0.00075 W. So we do again the operations:

$$E_{tot} daily =  0.018 Wh + 0.018 Wh + 0.18 Wh + 0.216 Wh + 0.00075 W \cdot 23.76 h = 0.449 Wh $$

$$E_{tot} monthly = 0.383 Wh \cdot 30 days = 13.47 Wh $$ 

In conclusion, also with this other deep sleep, due to the three sensors it uses and the Wi-Fi connection, the system is highly energy consuming. To be used in larger environments, it is necessary to improve the energy efficiency. 

The energy efficiency of the system can be improved using LoRa protocol (instead of MQTT and Wi-Fi) for the communication between nodes and with AWS.In particular it is possible to reduce the Esp32 consumption in ActiveMode.

### Energy Performance with LoRa Protocol

To obtain the energy performance of the Sensor Board it is needed to analyse the consumptions of the different parts of the system. The consumption of the sensors remain the same:
* KY-028 (temperature sensor): 5 V - 15mA = 0.075 W 
* Infrared Flame sensor: 5 V - 15 mA = 0.075 W 
* MQ7 sensor: 5 V - 150 mA = 0.750 W 

Also the time is the same:
* 864 sec (0,24 h) in active mode
* 85536 (23.76 h) sec in deep sleep mode

Meanwhile the consumption of the Esp32 is diffent. To understand what is the consumption of the board, it is useful the Energy Monitoring of Iot-Lab. Indeed the images below shows the consumption:<br>

This is the consumption of the board in a situation in which is not sleeping and is not doing anything. The segmented lines are the rumors of the test. The peak is the flash of the firmware.

![normal](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/normal.png) <br>

This is the consumption of the sleep mode. It is the same of the Active Mode. This means that it is not implemented the Deep Sleep. We will implemented so the Deep Sleep in the future calculus.

![sleep](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/sleep.png) <br>

This is the consumption of the sending of a LoRa message.

![sending](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/sending.png) <br>

This is the consumption of the sending of more LoRa messages. There will be this situation only when there is a fire.

![more_sending](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/more_sending.png) <br>

Using the graph above it is possible to say:
* Esp32 sending: 0.455 W 
* Esp32 in deep sleep mode: 0.00075 W (Sleep of Iot-lab is not implementing Deep Sleep Mode)
* Esp32 in ActiveMode: 0.28 W
* The listening situation is not needed due to the system implementation

So we do again the operations, using the lowest deep sleep possible:

$$TimeSending = (0.17 - 0.080) s = 0.090 s = 2.5 \cdot 10^{-5} h$$

$$E_{tot} =  KY028comsumption + FlameConsumption + MQ7consumption + ActivemodeConsumption + DeepSleepModeConsumption + MessageConsumption$$

$$E_{tot} daily =  0.075 W \cdot 0,24 h + 0.075 W \cdot 0,24 h + 0.750 \cdot 0,24 h + 0.28 W \cdot 0,24 h + 0.00075 W \cdot 23.76 h + 0.455 W \cdot 2.5 \cdot 10^{-5} h $$ 

$$E_{tot} daily =  0.018 Wh + 0.018 Wh + 0.18 Wh + 0.0672 Wh + 0.0178 Wh + 1.14 \cdot 10^{-5} Wh = 0.28 Wh $$ 

So, 0.28 Wh is the consumption per day of the system. Again it is interesting to understand for how much time it can survive. 

$$E_{tot} monthly = 0.28 Wh \cdot 30 days = 8.4 Wh $$ 

The conclusion is that the bottleneck is the MQ7 Sensor which is extremely energy demanding.
