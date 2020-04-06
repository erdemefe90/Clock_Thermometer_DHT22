#include <16F88.h>
#include "register_addresses.h"
/***************************************************************************************************************/
#device ADC=16

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(crystal=8000000)
/***************************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "defines.h"
#include "hc595.h"
#include "dht22.h"
#include "ds3231.h"
/***************************************************************************************************************/
volatile uint16_t dht22_read_timeout = 0;
/***************************************************************************************************************/
#INT_RTCC
void RTCC_isr(void) {
    dht22_read_timeout++;
    set_timer0(5);
}
/***************************************************************************************************************/
void screen_time()
{
    time_t current_time;
    ds3231_read_time(&current_time);
    
    hc595_write_number(current_time.hour, 0, 2);
    hc595_write_number(current_time.minute, 2, 2);
    
#ifndef REMOVE_CALENDAR
    hc595_write_number(current_time.second, 4, 2);
#endif
    hc595_show_screen();
}
/***************************************************************************************************************/
void screen_temperature()
{
    uint16_t temperature;
    uint8_t temperature_sign;
    uint8_t temp;
    if (dht22_check_response())
    {
        dht22_read_temp_hummidity(&temperature_sign, &temperature, &temp);           
    } 
#ifndef REMOVE_CALENDAR
    hc595_write_special_char(temperature_sign ? SPECIAL_CHAR_MINUS : SPECIAL_CHAR_BLANK, 0);
    hc595_write_number(temperature,1,3);
    hc595_point(TRUE, 2);
    hc595_write_special_char(SPECIAL_CHAR_CELC, 4);
    hc595_write_special_char(SPECIAL_CHAR_C, 5);
#else
    hc595_write_number((temperature/10),0,2);
    hc595_write_special_char(SPECIAL_CHAR_CELC, 2);
    hc595_write_special_char(SPECIAL_CHAR_C, 3);
    hc595_point(temperature_sign ? TRUE : FALSE , 3);
#endif
    hc595_show_screen();
}
/***************************************************************************************************************/
void main() {
    setup_timer_0(RTCC_INTERNAL | RTCC_DIV_8 | T0_8_BIT);
    set_timer0(5);
    setup_oscillator(OSC_8MHZ);
    setup_adc(ADC_OFF);
    hc595_init();
    dht22_init();
    ds3231_init();
    hc595_set_intensity(75);
    
    enable_interrupts(INT_RTCC);
    enable_interrupts(GLOBAL);
      
    while(TRUE)
    {
        time_t right_time;
        if (dht22_read_timeout > 2000)
        {
            dht22_read_timeout = 0;
            dht22_start_signal();
        }
        
        
        ds3231_read_time(&right_time);
        screen_temperature();
        

    }   
}

