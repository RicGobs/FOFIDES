# Prototype Evaluation

## Data of the problem 
As discussed in the introduction, Wildfires can fizzle out quickly or spread uncontrolled, consuming thousands of acres of land in a matter of hours. But the intensity and movement of a wildfire ultimately depends on three factors: fuel, weather and topography (fire behavior triangle). 

High temperatures and low humidity also dry out fuel sources, causing them to ignite and burn faster. This is why wildfires typically become more intense and spread fastest in the afternoon, when the air is hottest.

Common causes for wildfires include are Arson, Campfires, Discarding lit cigarettes, Improperly burning debris, Playing with matches or fireworks and Prescribed fires.
However, sometimes Wildfires do sometimes occur naturally, either ignited by the sun's heat or a lightning strike.

<br>
Most fires have historically occurred between May and October. However, recent data has shown that the season is lengthening, with wildfires starting earlier in the year and lasting well into the fall and winter months. 

![fire-season](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/fire-season.png) <br>

Wildfires spread at an average of 14.27 miles per hour. This means 23 km/h (6,38889 m/s). This are American analysis and this is the worst case scenario.
However, this can vary hugely depending on a number of factors, such as weather conditions, fuel type, and terrain.  

## Daily and Seasonal analysis
From the data take before, I have observed that is not possible to set in a static way different analysis for the seasons. Indeed the forest fire are not linked anymore to the seasons. 

It is possible to do some daily analysis observing the value of the temperature. Doing this, it is possible to understand a probabily of forest fire and beacuse of this, it is posssible to have a dynamic duty cicle, in which there are different sleep time:

* 5 minutes, for high temperature (high risk)
* 10 minutes, for middle temperature (middle risk)
* 15 minutes, for low temperature (low risk)

It is not suitable to sleep for more time because, in the case there is a fire, the system becomes unreliable.

With this system, there is also a different analysis (on average) between the seasons, and in the winter there will be usually a long duty cycle while in summer there will be a short duty cycle.

## Detection Performance


## Battery Performance



## Reliable Architecture
My test are done with the lighter so I will use this distance.

![angle](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/angle.png) <br>

I have thought an height of 1.73 but it is too much, fire too big.

![scale1](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/scale1.png) <br>

The detection distance of the flame of lighter is 100 cm.

![scale2](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/scale2.png) <br>
 
In the end, I have scaled the architecture.
