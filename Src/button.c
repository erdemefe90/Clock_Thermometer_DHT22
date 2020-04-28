#include <16F88.h>
#include "register_addresses.h"
#include <stdint.h>
#include <stdbool.h>
#include "button.h"

#define UP_DOWN_SENSE_TIME_MS   1000
#define UP_DOWN_SPEED_MS        50

#define UP_DOWN_COUNT           1

#define UP_TRIS     TRISB
#define UP_PORT     PORTB
#define UP_PIN      3

#define DOWN_TRIS   TRISB
#define DOWN_PORT   PORTB
#define DOWN_PIN    2

#define MODE_TRIS   TRISB
#define MODE_PORT   PORTB
#define MODE_PIN    7
/***************************************************************************************************************/
volatile uint16_t button_timeout[BUTTON_COUNT];
bool button_flag[BUTTON_COUNT];
uint16_t up_down_timeout;
/***************************************************************************************************************/
void button_timer()
{
    up_down_timeout++;
    button_timeout[UP]++;
    button_timeout[DOWN]++;
    button_timeout[MODE]++;
    //CLR_BIT(INTCON,7);
    if (FALSE == button_read(UP)) button_timeout[UP] = 0;
    if (FALSE == button_read(DOWN)) button_timeout[DOWN] = 0;
    if (FALSE == button_read(MODE)) button_timeout[MODE] = 0;
    //SET_BIT(INTCON,7);
}
/***************************************************************************************************************/
void button_init()
{
    SET_BIT(UP_TRIS, UP_PIN);
    SET_BIT(DOWN_TRIS, DOWN_PIN);
    SET_BIT(MODE_TRIS, MODE_PIN);
}
/***************************************************************************************************************/
bool button_read(button_types_t button)
{
    bool result = FALSE;
    switch (button)
    {
        case UP:
            result = CHECK_BIT(UP_PORT, UP_PIN) ? FALSE : TRUE;
        break;
        
        case DOWN:
            result = CHECK_BIT(DOWN_PORT, DOWN_PIN) ? FALSE : TRUE;
        break;
        
        case MODE:
            result = CHECK_BIT(MODE_PORT, MODE_PIN) ? FALSE : TRUE;
        break;
    }
    return result;
}
/***************************************************************************************************************/
bool button_read_with_timeout(button_types_t button, uint16_t time)
{
    bool result = FALSE;
    
    if (TRUE == button_read(button))
    {
        if (button_timeout[button] > time)
        {
            button_timeout[button] = (time + 1);
            result = TRUE;
        }
    }
    
    return result;          
}
/***************************************************************************************************************/
bool button_increase_number(uint8_t *number, uint8_t max)
{
    bool result = FALSE;
    int8_t temp = (int8_t)*number;
    if(TRUE == button_read(UP))
    {
        if (button_flag[UP])
        {
            temp++;
            button_flag[UP] = 0;
        }
    }
    else
    {
        button_flag[UP] = 1;
    }
    
    if (TRUE == button_read_with_timeout(UP, UP_DOWN_SENSE_TIME_MS))
    {
        if (up_down_timeout > UP_DOWN_SPEED_MS)
        {
            up_down_timeout = 0;
            temp = (temp + UP_DOWN_COUNT);
        }
    }
    if  (temp > max) 
    {
        temp = max;
        result = TRUE;
    }
    *number = temp;
    return result;
}
/***************************************************************************************************************/
bool button_decrease_number(uint8_t *number, uint8_t min)
{
    bool result = FALSE;
    int8_t temp = (int8_t)*number;
    if(TRUE == button_read(DOWN))
    {
        if (button_flag[DOWN])
        {
            temp--;
            button_flag[DOWN] = 0;
        } 
    }
    else
    {
        button_flag[DOWN] = 1;
    }
    
    if (TRUE == button_read_with_timeout(DOWN, UP_DOWN_SENSE_TIME_MS))
    {
        if (up_down_timeout > UP_DOWN_SPEED_MS)
        {
            up_down_timeout = 0;
            temp = (temp - UP_DOWN_COUNT);
        }
    }
    if  (temp < min)
    {
        temp = min;
        result  = TRUE;
    }
    *number = temp;
    return result;
}
/***************************************************************************************************************/