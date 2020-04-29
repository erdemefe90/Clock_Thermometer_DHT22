#include <16F88.h>
#include "register_addresses.h"
#include <stdint.h>
#include <stdbool.h>
#include "dht22.h"
/***************************************************************************************************************/
#use delay(crystal=8000000)
/***************************************************************************************************************/
#define DHT22_PORT  PORTB
#define DHT22_TRIS  TRISB
#define DHT22_PIN   6

#define GET_TIMER                   (TMR1L)
#define RESET_TIMER                 {\
                                    TMR1L = 0;\
                                    TMR1H = 0;\
}
/***************************************************************************************************************/
uint8_t correct_data[4];
/***************************************************************************************************************/
void dht22_init()
{
    SET_BIT(DHT22_TRIS, DHT22_PIN);
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_2);
}
/***************************************************************************************************************/
bool dht22_procsess() 
{
    uint8_t data[5]={0,0,0,0,0};
    uint8_t i,j;
    CLR_BIT(INTCON, 7);
    CLR_BIT(DHT22_TRIS, DHT22_PIN);
    CLR_BIT(DHT22_PORT, DHT22_PIN);         
    CLR_BIT(DHT22_PORT, DHT22_PIN);         
    delay_ms(20);
    SET_BIT(DHT22_PORT, DHT22_PIN); 
    delay_us(30);
    SET_BIT(DHT22_TRIS, DHT22_PIN);
    
    RESET_TIMER;
    while (!CHECK_BIT(DHT22_PORT, DHT22_PIN) && (GET_TIMER < 100)); // wait until DHT22_PIN becomes high (checking of 80?s low time response)

    if (GET_TIMER > 99) // if response time > 99?S  ==> Response error
    {
        SET_BIT(INTCON, 7);
        return FALSE; // return 0 (Device has a problem with response)
    }
    else 
    {
        RESET_TIMER;
        while (CHECK_BIT(DHT22_PORT, DHT22_PIN) && (GET_TIMER < 100)); // wait until DHT22_PIN becomes low (checking of 80?s high time response)
        if (GET_TIMER > 99)                                         // if response time > 99?S  ==> Response error
        {
            SET_BIT(INTCON, 7);
            return FALSE;                                       // return 0 (Device has a problem with response)
        }
        else
        {   
            for (j = 0; j < 5; j++)
            {
                RESET_TIMER;
                for (i = 0; i < 8; i++) 
                {
                    RESET_TIMER;
                    while (!CHECK_BIT(DHT22_PORT, DHT22_PIN))                              // wait until DHT22_PIN becomes high
                    if (GET_TIMER > 100)                            // if low time > 100  ==>  Time out error (Normally it takes 50?s)
                    { 
                        SET_BIT(INTCON, 7);
                        return FALSE;
                    }

                    RESET_TIMER;
                    while (CHECK_BIT(DHT22_PORT, DHT22_PIN))                               // wait until DHT22_PIN becomes low
                    if (GET_TIMER > 100)                            // if high time > 100  ==>  Time out error (Normally it takes 26-28?s for 0 and 70?s for 1)
                    { 
                        SET_BIT(INTCON, 7);
                        return FALSE;                           // return 1 (timeout error)
                    }

                    if (GET_TIMER > 50)                             // if high time > 50  ==>  Sensor sent 1
                    {
                        data[j] |= (1 << (7 - i));              // set bit (7 - i)
                    }
                }
            }
            
            if(((data[0] + data[1] + data[2] + data[3]) & 0xFF) == data[4])
            {
                for(i = 0 ; i < 4; i++) 
                {
                    correct_data[i] = data[i];
                }
                SET_BIT(INTCON, 7);
                return TRUE;
            }
            else
            {
                SET_BIT(INTCON, 7);
                return FALSE;
            }
            
        }
    }
}
/***************************************************************************************************************/
int16_t dht22_read_temperature()
{
    int16_t temp;
    temp = correct_data[2];
    temp = (temp <<8 ) | correct_data[3];
    if (temp & 0x8000) temp *= -1;
    return temp;
}
/***************************************************************************************************************/
uint16_t dht22_read_hummidity()
{
    uint16_t humm;
    humm = correct_data[0];
    humm = (humm << 8) | correct_data[1];
    return humm;
}
/***************************************************************************************************************/