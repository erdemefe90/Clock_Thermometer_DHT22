#include <16F88.h>
#include "register_addresses.h"
/***************************************************************************************************************/
#device ADC=16

#FUSES INTRC_IO, NOMCLR, NOWDT, NOBROWNOUT, NOLVP, CCPB0, NOWDT

#use delay(crystal=8000000)
/***************************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "defines.h"
#include "hc595.h"
#include "dht22.h"
#include "ds3231.h"
#include "led.h"
#include "button.h"
/***************************************************************************************************************/
uint16_t clock_screen_time;
uint16_t temperature_screen_time;
uint16_t hummidity_screen_time;
uint8_t hc595_intensity;
uint8_t screen_mode = 0;
volatile uint16_t screen_timeout;
volatile uint16_t dht22_read_timeout;
volatile uint16_t flash_timeout;
#ifndef REMOVE_CALENDAR
uint16_t date_screen_time;
#endif
/***************************************************************************************************************/
#INT_RTCC
void RTCC_isr(void) 
{
    button_timer();
    led_timer();  
    dht22_read_timeout++;
    screen_timeout++;
    flash_timeout++;
    set_timer0(5);
}
/***************************************************************************************************************/
void screen_time()
{
    time_t current_time;
    ds3231_read_time(&current_time);
    
    hc595_write_number(current_time.hour, 0, TRUE, 2);
    hc595_write_number(current_time.minute, 2, TRUE, 2);
    
#ifndef REMOVE_CALENDAR
    hc595_write_number(current_time.second, 4, TRUE,  2);
#endif
    hc595_show_screen();
}
/***************************************************************************************************************/
#ifndef REMOVE_CALENDAR
void screen_date()
{
    time_t current_time;
    ds3231_read_time(&current_time);
    hc595_write_number(current_time.date, 0, TRUE, 2);
    hc595_write_number(current_time.month, 2, TRUE, 2);
    hc595_write_number(current_time.year, 4, TRUE, 2);
    hc595_show_screen();
}
#endif
/***************************************************************************************************************/
void screen_temperature()
{
    int16_t temperature = dht22_read_temperature();
    //temperature = -96;
#ifndef REMOVE_CALENDAR
    hc595_write_special_char((temperature < 0) ? SPECIAL_CHAR_MINUS : SPECIAL_CHAR_BLANK, 0);
    hc595_write_number(abs(temperature), 1, FALSE, 3);
    hc595_point(TRUE, 2);
    hc595_write_special_char(SPECIAL_CHAR_CELC, 4);
    hc595_write_special_char(SPECIAL_CHAR_C, 5);
#else
    hc595_write_number((abs(temperature) / 10),0, FALSE, 2);
    hc595_write_special_char(SPECIAL_CHAR_CELC, 2);
    hc595_write_special_char(SPECIAL_CHAR_C, 3);
    hc595_point((temperature < 0) ? TRUE : FALSE , 3);
#endif
    hc595_show_screen();
}
/***************************************************************************************************************/
void screen_hummidity()
{
    uint16_t hummidity = dht22_read_hummidity();
    //hummidity = 36;
    hc595_write_special_char(SPECIAL_CHAR_r , 0);
    hc595_write_special_char(SPECIAL_CHAR_H , 1);
#ifndef REMOVE_CALENDAR
    hc595_write_number(hummidity, 2, FALSE, 3);
    hc595_write_special_char(SPECIAL_CHAR_BLANK , 5);
    hc595_point(TRUE, 3);
#else
    hc595_write_number((hummidity / 10), 2, FALSE, 2);
#endif
    hc595_show_screen();
}
/***************************************************************************************************************/
void initializing_setting()
{
#ifdef REMOVE_CALENDAR
    #define AYAR_COUNT  4
#else
    #define AYAR_COUNT  5
#endif
    static uint8_t setting_order;
    uint8_t special_char;
    uint16_t number[AYAR_COUNT];
    uint8_t digit_count = hc595_get_digit_count();
    uint8_t i;
    
    number[0] = hc595_intensity;
    number[1] = clock_screen_time / 1000;
    number[2] = temperature_screen_time / 1000;
    number[3] = hummidity_screen_time / 1000;
#ifndef REMOVE_CALENDAR
    number[4] = date_screen_time / 1000;
#endif
    
    hc595_write_special_char(SPECIAL_CHAR_A, 0);
    hc595_write_number(number[setting_order], (digit_count - 2), FALSE, 2);
    hc595_show_screen();
    while (button_read(MODE));

    while (true)
    {
        switch (setting_order)
    {
        case 0:
            special_char = SPECIAL_CHAR_A;
            hc595_set_intensity(hc595_intensity);
            break;
        case 1:
            special_char = SPECIAL_CHAR_b;
            break;
        case 2:
            special_char = SPECIAL_CHAR_C;
            break;
        case 3:
            special_char = SPECIAL_CHAR_d;
            break;
        case 4:
            special_char = SPECIAL_CHAR_E;
            break;
    }
        
        button_increase_number((uint8_t*)&number[setting_order], (setting_order != 0 ? 60 : 10));
        button_decrease_number((uint8_t*)&number[setting_order], 0);
        
        hc595_intensity = number[0];
        clock_screen_time = number[1] * 1000;
        temperature_screen_time = number[2] * 1000;
        hummidity_screen_time = number[3] * 1000;
#ifndef REMOVE_CALENDAR
        date_screen_time = number[4] * 1000;
#endif        

        if (button_read(MODE) == TRUE)
        {
            while (button_read(MODE) == TRUE)
            {
                if (button_read_with_timeout(MODE, BUTTON_HOLD_TIME_MS))
                {
                    led (250, 100, 3, TRUE);
                    for (i = 0 ; i < digit_count ; i++)
                    {
                        hc595_write_special_char(SPECIAL_CHAR_MINUS, i);
                    }          
                    hc595_show_screen();
                    while (led_check());
                    return;
                }
            }     
            if (setting_order == 0) hc595_set_intensity(number[0]);
            write_eeprom(setting_order, number[setting_order]);
            setting_order++;
            if (setting_order == AYAR_COUNT) setting_order = 0;
        }
 
        hc595_write_special_char(special_char, 0);
        hc595_write_number(number[setting_order], (digit_count - 2), FALSE, 2);
        hc595_show_screen();
    }
    
}
/***************************************************************************************************************/
#ifndef REMOVE_CALENDAR
void clock_date_setting()
{
    uint8_t i;
    time_t current_time;
    time_t to_set_time;
    bool flash;
    bool clock_date = FALSE;
    uint8_t setting_order = 0;
    ds3231_read_time(&current_time);
    memcpy(&to_set_time , &current_time , sizeof(time_t));
    
    led(500,500,0,FALSE);
    for (i = 0; i < hc595_get_digit_count() ; i++)
    {
        hc595_write_special_char(SPECIAL_CHAR_MINUS, i);
    }
    hc595_show_screen();
    while (button_read(MODE) == TRUE);
    
    while (TRUE)
    {
        
        if (button_read(MODE))
        {
            while(button_read(MODE))
            {
                if (button_read_with_timeout(MODE , BUTTON_HOLD_TIME_MS))
                {
                    ds3231_write_time(&to_set_time);
                    led(100,100,5,TRUE);
                    while (led_check());
                    screen_mode = 0;
                    screen_timeout = 0;
                    led(500, 500, 0, TRUE);
                    return;
                }
            }
            setting_order++;
            if (setting_order == 6) setting_order = 0;
            if (setting_order > 2)
            {
                led(1000,0,0,TRUE);
                clock_date = TRUE ;
            }
            else 
            {
                led(1000,0,0,FALSE);
                clock_date = FALSE; 
            }
        }
        
        switch (setting_order)
        {
            case 0:
                if (TRUE == button_increase_number(&to_set_time.hour,23))
                {
                    to_set_time.hour = 0;
                }
                if (TRUE == button_decrease_number(&to_set_time.hour,0))
                {
                    to_set_time.hour = 23;
                }
            break;
            
            case 1:
                if (TRUE == button_increase_number(&to_set_time.minute,59))
                {
                    to_set_time.minute = 0;
                }
                if (TRUE == button_decrease_number(&to_set_time.minute,0))
                {
                    to_set_time.minute = 59;
                }
            break;
            
            case 2:
                if (TRUE == button_increase_number(&to_set_time.second,59))
                {
                    to_set_time.second = 0;
                }
                if (TRUE == button_decrease_number(&to_set_time.second,0))
                {
                    to_set_time.second = 59;
                }
            break;
            
            case 3:
                if (TRUE == button_increase_number(&to_set_time.date,31))
                {
                    to_set_time.date = 1;
                }
                if (TRUE == button_decrease_number(&to_set_time.date,1))
                {
                    to_set_time.date = 31;
                }
            break;
            
            case 4:
                if (TRUE == button_increase_number(&to_set_time.month,12))
                {
                    to_set_time.month = 1;
                }
                if (TRUE == button_decrease_number(&to_set_time.month,1))
                {
                    to_set_time.month = 12;
                }
            break;
            
            case 5:
                if (TRUE == button_increase_number(&to_set_time.year,99))
                {
                    to_set_time.year = 1;
                }
                if (TRUE == button_decrease_number(&to_set_time.year,1))
                {
                    to_set_time.year = 99;
                }
            break;
        }
        
        if (flash_timeout > 250) {flash_timeout = 0; flash = !flash;}
        
        if (clock_date == FALSE)
        {
            if (flash)
            {

                hc595_write_number(to_set_time.hour,0,TRUE,2);
                hc595_write_number(to_set_time.minute,2,TRUE,2);
                hc595_write_number(to_set_time.second,4,TRUE,2);
            }
            else
            {
                hc595_write_special_char(SPECIAL_CHAR_BLANK,setting_order*2);
                hc595_write_special_char(SPECIAL_CHAR_BLANK,setting_order*2+1);
            }
        }
        else
        {
            if (flash)
            {

                hc595_write_number(to_set_time.date,0,TRUE,2);
                hc595_write_number(to_set_time.month,2,TRUE,2);
                hc595_write_number(to_set_time.year,4,TRUE,2);
            }
            else
            {
                hc595_write_special_char(SPECIAL_CHAR_BLANK,(setting_order*2)-6);
                hc595_write_special_char(SPECIAL_CHAR_BLANK,(setting_order*2+1)-6);
            }
        }
        
        hc595_show_screen();
    }
}
#else
/***************************************************************************************************************/
void clock_setting()
{
    uint8_t i;
    time_t current_time;
    time_t to_set_time;
    bool flash;
    bool setting_order = 0;
    ds3231_read_time(&current_time);
    memcpy(&to_set_time , &current_time , sizeof(time_t));
    
    led(500,500,0,FALSE);
    
    for (i = 0; i < hc595_get_digit_count() ; i++)
    {
        hc595_write_special_char(SPECIAL_CHAR_MINUS, i);
    }
    hc595_show_screen();
    while (button_read(MODE) == TRUE);
    
    while (TRUE)
    {
        
        if (button_read(MODE))
        {
            while(button_read(MODE))
            {
                if (button_read_with_timeout(MODE , BUTTON_HOLD_TIME_MS))
                {
                    ds3231_write_time(&to_set_time);
                    led(100,100,5,TRUE);
                    while (led_check());
                    screen_mode = 0;
                    screen_timeout = 0;
                    led(500, 500, 0, TRUE);
                    return;
                }
            }
            setting_order = !setting_order;
        }
        
        if (setting_order == FALSE)
        {
            if (TRUE == button_increase_number(&to_set_time.hour,23))
            {
                to_set_time.hour = 0;
            }
            if (TRUE == button_decrease_number(&to_set_time.hour,0))
            {
                to_set_time.hour = 23;
            }
        }
        else if (setting_order == TRUE)
        {
            if (TRUE == button_increase_number(&to_set_time.minute,59))
            {
                to_set_time.minute = 0;
            }
            if (TRUE == button_decrease_number(&to_set_time.minute,0))
            {
                to_set_time.minute = 59;
            }
        }
        
        if (flash_timeout > 250) {flash_timeout = 0; flash = !flash;}
        
        if (flash)
        {

            hc595_write_number(to_set_time.hour,0,TRUE,2);
            hc595_write_number(to_set_time.minute,2,TRUE,2);
        }
        else
        {
            hc595_write_special_char(SPECIAL_CHAR_BLANK,setting_order*2);
            hc595_write_special_char(SPECIAL_CHAR_BLANK,setting_order*2+1);
        }
        hc595_show_screen();
    }
}
#endif
/***************************************************************************************************************/
void main() {
    setup_oscillator(OSC_8MHZ);
    //OSCCON = 0x70;
    setup_adc(ADC_OFF);
    setup_timer_0(RTCC_INTERNAL | RTCC_DIV_8 | T0_8_BIT);
    set_timer0(5);    
    hc595_init();
    dht22_init();
    ds3231_init();
    button_init();
    led_init();
    enable_interrupts(INT_RTCC);
    enable_interrupts(GLOBAL);    
    
    hc595_intensity = read_eeprom(0); 
    if (hc595_intensity == 0xFF) hc595_intensity = DEFAULT_HC595_INTENSITY;
    
    clock_screen_time = read_eeprom(1); 
    if (clock_screen_time == 0xFF) clock_screen_time = DEFAULT_CLOCK_TIME_SEC * 1000; else clock_screen_time *= 1000;
    
    temperature_screen_time = read_eeprom(2); 
    if (temperature_screen_time == 0xFF) temperature_screen_time = DEFAULT_TEMPERATURE_TIME_SEC * 1000; else temperature_screen_time *= 1000;
    
    
    hummidity_screen_time = read_eeprom(3); 
    if (hummidity_screen_time == 0xFF) hummidity_screen_time = DEFAULT_HUMMIDITY_TIME_SEC * 1000; else hummidity_screen_time *= 1000;
    
#ifndef REMOVE_CALENDAR
    date_screen_time = read_eeprom(4); 
    if (date_screen_time == 0xFF) date_screen_time = DEFAULT_DATE_TIME_SEC * 1000; else date_screen_time *= 1000;
#endif
    hc595_set_intensity(hc595_intensity);
    if (button_read(MODE) == TRUE) initializing_setting();
    screen_mode = 0;
    screen_timeout = 0;
    led(500,500,0,TRUE);
    
    while(TRUE)
    {
        if (dht22_read_timeout > 3000)
        {
            dht22_read_timeout = 0;
            dht22_procsess();
        }
        
        if (button_read_with_timeout(MODE,BUTTON_HOLD_TIME_MS) == TRUE)
        {
#ifdef REMOVE_CALENDAR
            clock_setting();
#else
            clock_date_setting();
#endif
        }
        
        switch (screen_mode)
            {
                case 0:
                    if (clock_screen_time != 0)
                    {
                        screen_time();
                        if (screen_timeout > clock_screen_time)
                        {
                            screen_timeout = 0;
                            screen_mode = 1;   
                            led(500, 500, 0, FALSE);
                        }
                    }
                    else
                    {
                        screen_mode = 1;
                        led(500, 500, 0, FALSE);
                    }
                    
                break;
                case 1:
                    if (temperature_screen_time != 0)
                    {
                        screen_temperature();
                        if (screen_timeout > temperature_screen_time)
                        {
                            screen_timeout = 0;
                            screen_mode = 2;                      
                        }
                    }
                    else
                    {
                        screen_mode = 2; 
                    }
                break;    
                case 2:
                    if (hummidity_screen_time != 0)
                    {
                        screen_hummidity();
                        if (screen_timeout > hummidity_screen_time)
                        {
                            screen_timeout = 0;
#ifdef REMOVE_CALENDAR
                            screen_mode = 0; 
                            led(500, 500, 0, TRUE);
#else
                            screen_mode = 3;
                            led(500,0,0,TRUE);
#endif
                        }
                    }
                    else
                    {
#ifdef REMOVE_CALENDAR
                            screen_mode = 0;
                            led(500, 500, 0, TRUE);
#else
                            screen_mode = 3;
                            led(500,0,0,TRUE);
#endif
                    }
                break;   
#ifndef REMOVE_CALENDAR
                case 3:
                    if (date_screen_time != 0)
                    {
                        screen_date();
                        if (screen_timeout > date_screen_time)
                        {
                            screen_timeout = 0;
                            screen_mode = 0;  
                            led(500,0,0,FALSE);
                            led(500, 500, 0, TRUE);
                        }
                    }
                    else
                    {
                        screen_mode = 0;  
                        led(500,0,0,FALSE);
                        led(500, 500, 0, TRUE);
                    }
                break;     
#endif
            }      
    }

}
