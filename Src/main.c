#include <16F88.h>
#include "register_addresses.h"
#include <stdint.h>
#include <stdbool.h>
#include "defines.h"
#include "hc595.h"
#include "dht22.h"
#include "ds3231.h"


#device ADC=16

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(crystal=8000000)

volatile uint16_t dht22_read_timeout = 0;

#INT_RTCC
void RTCC_isr(void) {
    dht22_read_timeout++;
    set_timer0(5);
}


void main() {
    uint16_t temp;
    uint16_t humm;
    uint8_t sign;
    bool dht_result;
    time_t time_to_write;
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
        if (dht22_check_response())
        {
            dht_result = dht22_read_temp_hummidity(&sign, &temp, &humm);
            if (FALSE == dht_result)
            {
                sign = 0;
                temp = 0;
                humm = 0;
            }
            else
            {
                hc595_write_number(humm,0,3);
            }
        } 
        
        ds3231_read_time(&right_time);
        delay_ms(100);
        
#ifdef REMOVE_CALENDER
        //hc595_write_number(right_time.hour,0,2);
        //hc595_write_number(right_time.minute,2,2);
#else
        hc595_write_number(right_time.hour,0,2);
        hc595_write_number(right_time.minute,2,2);
        hc595_write_number(right_time.second,4,2);
#endif
    }   

}

