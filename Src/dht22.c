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

#define GET_TIMER                   (TMR1L | (((uint16_t)TMR1H << 8)))
#define SET_TIMER(x)                {\
                                    TMR1L = x;\
                                    TMR1H = (x >> 8);\
}
/***************************************************************************************************************/
uint8_t T_Byte1, T_Byte2, RH_Byte1, RH_Byte2, CheckSum;
/***************************************************************************************************************/
static bool dht22_read_data(uint8_t* dht_data);
/***************************************************************************************************************/
void dht22_init()
{
    SET_BIT(DHT22_TRIS, DHT22_PIN);
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_2);
}
/***************************************************************************************************************/
void dht22_start_signal(void) 
{
    CLR_BIT(DHT22_TRIS, DHT22_PIN);
    CLR_BIT(DHT22_PORT, DHT22_PIN);
    delay_ms(25); // wait 25 ms;
    SET_BIT(DHT22_PORT, DHT22_PIN);
    delay_us(30); // wait 30 us
    SET_BIT(DHT22_TRIS, DHT22_PIN);
}
/***************************************************************************************************************/
bool dht22_check_response() 
{
    SET_TIMER(0);
    SET_BIT(T1CON, 0);

    while (!CHECK_BIT(DHT22_PORT, DHT22_PIN) && GET_TIMER < 100); // wait until DHT22_PIN becomes high (checking of 80µs low time response)

    if (GET_TIMER > 99) // if response time > 99µS  ==> Response error
        return FALSE; // return 0 (Device has a problem with response)

    else {
        SET_TIMER(0);

        while (CHECK_BIT(DHT22_PORT, DHT22_PIN) && GET_TIMER < 100); // wait until DHT22_PIN becomes low (checking of 80µs high time response)

        if (GET_TIMER > 99) // if response time > 99µS  ==> Response error
        {
            return FALSE; // return 0 (Device has a problem with response)
        }
            
        else
        {
            return TRUE;
        }
    }
    CLR_BIT(T1CON, 0);
}
/***************************************************************************************************************/
static bool dht22_read_data(uint8_t* dht_data) 
{
    *dht_data = 0;

    for (uint8_t i = 0; i < 8; i++) {
        SET_TIMER(0)  ;

        while (!CHECK_BIT(DHT22_PORT, DHT22_PIN)) // wait until DHT22_PIN becomes high
            if (GET_TIMER > 100) { // if low time > 100  ==>  Time out error (Normally it takes 50µs)
                return TRUE;
            }

        SET_TIMER(0);

        while (CHECK_BIT(DHT22_PORT, DHT22_PIN)) // wait until DHT22_PIN becomes low
            if (GET_TIMER > 100) { // if high time > 100  ==>  Time out error (Normally it takes 26-28µs for 0 and 70µs for 1)
                return TRUE; // return 1 (timeout error)
            }

        if (GET_TIMER > 50) // if high time > 50  ==>  Sensor sent 1
            *dht_data |= (1 << (7 - i)); // set bit (7 - i)
    }
    return FALSE; // return 0 (data read OK)
}
/***************************************************************************************************************/
bool dht22_read_temp_hummidity(uint8_t *temp_sign, uint16_t* temp, uint16_t* hummidity)
{
    bool result = FALSE;
    if (dht22_read_data(&RH_Byte1) || dht22_read_data(&RH_Byte2) || dht22_read_data(&T_Byte1) || dht22_read_data(&T_Byte2) || dht22_read_data(&CheckSum))
    {
        result = FALSE;
    }
    else
    {
        if (CheckSum == ((RH_Byte1 + RH_Byte2 + T_Byte1 + T_Byte2) & 0xFF)) // if there is no checksum error
        { 
        *hummidity = RH_Byte1;
        *hummidity = (*hummidity << 8) | RH_Byte2;
        *temp = T_Byte1;
        *temp = (*temp << 8) | T_Byte2;

        if (*temp > 0X8000) { // if temperature < 0
            *temp_sign = 1;
            *temp = *temp & 0X7FFF;
        } else // otherwise (temperature > 0)
            *temp_sign = 0;
        result = TRUE;
    }// if there is a checksum error
    else {
            result = FALSE;
        //checksum error
    }
    }
    return result;
}