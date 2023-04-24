#include "stdio.h"
#include "periph/gpio.h"
#include "periph/adc.h"
#include "analog_util.h"
#include "xtimer.h"

/*
void dec2bin(int n)
{
   int c;
   //int k;

   for (c = 15; c >= 0; c--)
    {
        k = n >> c;
        
        if (k & 1)
            printf("1"); //Serial.print("1");
        else
            printf("0"); //Serial.print("0");
        }
        
} 

void dec2bin8(int n)
{
   int c;
   //int k;
   
   for (c = 7; c >= 0; c--)
    {
        k = n >> c;
        
        if (k & 1)
            printf("1"); //Serial.print("1");
        else
            printf("0"); //Serial.print("0");
        }
        
    }
*/

void start_signal(uint8_t dht11_pin)
{
    if(gpio_init(dht11_pin,GPIO_OUT)==0) printf("ok ");//temperature pin initialize to be output

    gpio_clear(dht11_pin); //value to LOW

    xtimer_msleep(18); //delay of 18 milliseconds

    gpio_set(dht11_pin); //value to HIGH

    if(gpio_init(dht11_pin,GPIO_IN_PU)==0) printf("ok ");//temperature pin initialize to be input

    gpio_set(dht11_pin); //value to HIGH

    printf("Successfully initialized temperature_sensor\n");

}

void read_dht11(uint8_t dht11_pin)
{
    uint16_t rawHumidity = 0;
    uint16_t rawTemperature = 0;
    //uint8_t checkSum = 0;
    uint16_t data = 0;

    uint8_t humi;
    uint8_t humd;
    uint8_t tempi;
    uint8_t tempd; 

    xtimer_ticks32_t startTime; 
    
    for ( int8_t i = -3 ; i < 80; i++ ) {
        uint16_t live; 
        startTime = xtimer_now(); 

        do {
        live = (unsigned long)((xtimer_ticks32_t)xtimer_now() - startTime); 
        if ( live > 90 ) {
            printf("ERROR_TIMEOUT of temperature sensor\n"); 
            return;
        }
        }
        while ( gpio_read(dht11_pin) == (i & 1) ? 1 : 0 ); 

        if ( i >= 0 && (i & 1) ) {
        data <<= 1;

        // TON of bit 0 is maximum 30 usecs and of bit 1 is at least 68 usecs.
        if ( live > 30 ) {
            data |= 1; // we got a one
        }
        }

        switch ( i ) {
        case 31:
            rawHumidity = data;
            break;
        case 63:
            rawTemperature = data;
            break;
        case 79: 
            //checkSum = data;
            data = 0;
            break;
        }
    }

    printf("Humidity: "); 
    //dec2bin(rawHumidity);
    //printf("\t"); 
    humi = rawHumidity >> 8;
    //dec2bin8(humi);
    //printf("\t"); 
    rawHumidity = rawHumidity << 8;
    humd = rawHumidity >> 8;
    //dec2bin8(humd);
    //printf("\t");
    printf("%d.%d%%\t",humi,humd); 
    printf("Temperature: "); 
    //dec2bin(rawTemperature);
    //printf("\t"); 
    tempi = rawTemperature >> 8;
    //dec2bin8(tempi);
    //printf("\t"); 
    rawTemperature = rawTemperature << 8;
    tempd = rawTemperature >> 8;
    //dec2bin8(tempd);
    //printf("\t"); 
    printf("%d.%dC\t",tempi,tempd); 

    //printf("Checksum Byte: "); 
    //dec2bin8(checkSum);
    //printf("\n"); 
    //dec2bin8(tempi + tempd + humi + humd);
    //printf("\n");
    //if((byte)checkSum == (byte)(tempi + tempd + humi + humd)){printf("CHECKSUM_OK");}
    //if((uint16_t)checkSum == (uint16_t)(tempi + tempd + humi + humd)){printf("CHECKSUM_OK");}
    //else {printf("CHECKSUM_OK");}  
    printf("\n"); 

}
