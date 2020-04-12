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
volatile uint16_t dht22_read_timeout = 0;
/***************************************************************************************************************/
#INT_RTCC
void RTCC_isr(void) 
{
    button_timer();
    led_timer();  
    dht22_read_timeout++;
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
    
    while(TRUE)
    {
        if (dht22_read_timeout > 2000)
        {
            dht22_read_timeout = 0;
            dht22_procsess();
        }
        
        //screen_time();
        if (TRUE == button_read(DOWN)) screen_hummidity();
        if (TRUE == button_read_with_timeout(MODE,2000)) screen_temperature();
    }   //if (TRUE == button_read(UP)) screen_time();
}

