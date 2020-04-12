#include <16F88.h>
#include "register_addresses.h"
#include <stdint.h>
#include <stdbool.h>
#include "led.h"
#include "ds3231.h"
/***************************************************************************************************************/
#define LED_TRIS    TRISA
#define LED_PORT    PORTA
#define LED_PIN     2
/***************************************************************************************************************/
typedef struct{
    uint16_t on_time;
    uint16_t off_time;
    uint16_t enable_timeout;
    uint16_t enable_time;
    uint16_t on_off_timeout;
    bool enable;
    
}led_t;
/***************************************************************************************************************/
volatile led_t led_struct;
/***************************************************************************************************************/
void led_timer()
{
    led_struct.on_off_timeout++;
    led_struct.enable_timeout++;
    
    if(led_struct.enable == TRUE)
    {
        if (FALSE == CHECK_BIT(LED_PORT, LED_PIN))
        {
            if (led_struct.on_off_timeout > led_struct.off_time)
            {
                led_struct.on_off_timeout = 0;
                SET_BIT(LED_PORT, LED_PIN);
            }
        }
        else if(FALSE != CHECK_BIT(LED_PORT, LED_PIN))
        {
            if (led_struct.on_off_timeout > led_struct.on_time)
            {
                led_struct.on_off_timeout = 0;
                CLR_BIT(LED_PORT, LED_PIN);
            }
        }
        if ((led_struct.enable_timeout > led_struct.enable_time) && (led_struct.enable_time != 0))
            {
                led_struct.enable = 0;
                CLR_BIT(LED_PORT, LED_PIN);
            }
    }
}
/***************************************************************************************************************/
void led_init()
{
    CLR_BIT(LED_TRIS, LED_PIN);
    CLR_BIT(LED_PORT, LED_PIN);
}
/***************************************************************************************************************/
void led (uint16_t on_time, uint16_t off_time, uint8_t cycle, bool state)
{
    led_struct.enable_time = ((on_time + off_time) * cycle);
    led_struct.on_time = on_time;
    led_struct.off_time =  off_time;
    led_struct.enable = state;
    led_struct.on_off_timeout = 0;
    led_struct.enable_timeout = 0;
    if (led_struct. enable == FALSE)
    {
        CLR_BIT(LED_PORT, LED_PIN);
    }
}
/***************************************************************************************************************/
bool led_check(void)
{
    return led_struct.enable;
}
/***************************************************************************************************************/