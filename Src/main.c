#include <16F88.h>
#include "register_addresses.h"
/***************************************************************************************************************/
#device ADC=16

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES CCPB0

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
bool normal_setting = 0;
volatile uint16_t screen_timeout;
volatile uint16_t dht22_read_timeout;
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
    hc595_write_number(current_time.day, 0, TRUE, 2);
    hc595_write_number(current_time.month, 2, TRUE, 2);
    hc595_write_number(current_time.year, 4, TRUE, 2);
    hc595_show_screen();
}
#endif
/***************************************************************************************************************/
void screen_temperature()
{
    int16_t temperature = dht22_read_temperature();
    temperature = -96;
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
    hummidity = 36;
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
    
    uint8_t digit_order = hc595_get_digit_count() - 2;
    
    number[0] = hc595_intensity;
    number[1] = clock_screen_time / 1000;
    number[2] = temperature_screen_time / 1000;
    number[3] = hummidity_screen_time / 1000;
#ifndef REMOVE_CALENDAR
    number[4] = date_screen_time / 1000;
#endif
    
    hc595_write_special_char(SPECIAL_CHAR_A, 0);
    hc595_write_number(number[setting_order], digit_order, FALSE, 2);
    hc595_show_screen();
    while (button_read(MODE));

    while (true)
    {
        switch (setting_order)
    {
        case 0:
            special_char = SPECIAL_CHAR_A;
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
        
        button_increase_number((uint8_t*)&number[setting_order], (setting_order != 0 ? 60 : 99));
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
                if (button_read_with_timeout(MODE, 500))
                {
                    led (250, 100, 3, TRUE);
                    return;
                }
            }     
            if (setting_order == 0) hc595_set_intensity(number[0]);
            write_eeprom(setting_order, number[setting_order++]);
            if (setting_order == AYAR_COUNT) setting_order = 0;
        }
 
        hc595_write_special_char(special_char, 0);
        hc595_write_number(number[setting_order], digit_order, FALSE, 2);
        hc595_show_screen();
    }
    
}
/***************************************************************************************************************/
void main() {
    setup_oscillator(OSC_8MHZ);
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
    
    for(uint8_t i = 0 ; i<10; i++) write_eeprom(i,0xff);
    
    
    hc595_intensity = read_eeprom(0); if (hc595_intensity == 0xFF) hc595_intensity = DEFAULT_HC595_INTENSITY;
    clock_screen_time = read_eeprom(1); if (clock_screen_time == 0xFF) clock_screen_time = DEFAULT_CLOCK_TIME_SEC * 1000;
    temperature_screen_time = read_eeprom(2); if (temperature_screen_time == 0xFF) temperature_screen_time = DEFAULT_TEMPERATURE_TIME_SEC * 1000;
    hummidity_screen_time = read_eeprom(3); if (hummidity_screen_time == 0xFF) hummidity_screen_time = DEFAULT_HUMMIDITY_TIME_SEC * 1000;
#ifndef REMOVE_CALENDAR
    date_screen_time = read_eeprom(4); if (date_screen_time == 0xFF) date_screen_time = DEFAULT_DATE_TIME_SEC * 1000;
#endif
    
    if (button_read(MODE) == TRUE) initializing_setting();
    
    while(TRUE)
    {
        if (dht22_read_timeout > 2000)
        {
            dht22_read_timeout = 0;
            dht22_procsess();
        }
        
        if (normal_setting == FALSE)
        {
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
                        }
                    }
                    else
                    {
                        screen_mode = 1;
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
#else
                            screen_mode = 3;
#endif
                        }
                    }
                    else
                    {
#ifdef REMOVE_CALENDAR
                            screen_mode = 0;     
#else
                            screen_mode = 3;
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
                        }
                    }
                    else
                    {
                        screen_mode = 0;  
                    }
                break;     
#endif
            }
        }
        
        
}

}
