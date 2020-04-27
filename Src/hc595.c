#include <16F88.h>
#include "register_addresses.h"
#include <stdint.h>
#include <stdbool.h>
#include "defines.h"
#include "hc595.h"
/***************************************************************************************************************/
#use delay(crystal=8000000)
/***************************************************************************************************************/
#ifdef REMOVE_CALENDAR
    #define DIGIT_COUNT 4
#else
    #define DIGIT_COUNT 6
#endif
/***************************************************************************************************************/
#define CLK_PORT    PORTB
#define CLK_PIN     5
#define CLK_TRIS    TRISB    

#define DATA_PORT   PORTA
#define DATA_PIN    0
#define DATA_TRIS   TRISA

#define LD_PORT     PORTA
#define LD_PIN      1
#define LD_TRIS     TRISA

#define OE_PORT     PORTB
#define OE_PIN      0
#define OE_TRIS     TRISB
/***************************************************************************************************************/
/*                          E	D	C	B	A   DP  F	G   */
const uint8_t font[13] = {0xFA, 0x30, 0xD9, 0x79, 0x33, 0x6B, 0xEB, 0x38, 0xFB, 0x7B};
static uint8_t hc595_buff[DIGIT_COUNT];
/***************************************************************************************************************/
static void pulse_out();
static void hc595_write_data();
static uint8_t map(uint8_t x, uint8_t in_min, uint8_t in_max, uint8_t out_min, uint8_t out_max);
static bool number_to_digit(uint16_t number, uint8_t *digit, uint8_t digit_pos);
static uint8_t count_digit(uint32_t n) ;
/***************************************************************************************************************/
void hc595_init()
{    
    CLR_BIT(CLK_TRIS, CLK_PIN);
    CLR_BIT(DATA_TRIS, DATA_PIN);
    CLR_BIT(LD_TRIS, LD_PIN);
    CLR_BIT(OE_TRIS, OE_PIN);
    
    CLR_BIT(CLK_PORT, CLK_PIN);
    CLR_BIT(DATA_PORT, DATA_PIN);
    CLR_BIT(LD_PORT, LD_PIN);
    SET_BIT(OE_PORT, OE_PIN);
    
    setup_timer_2(T2_DIV_BY_16, 24, 1); //200 us overflow, 200 us interrupt
    setup_ccp1(CCP_PWM);
}
/***************************************************************************************************************/
void hc595_set_intensity(uint8_t value)
{
    uint8_t pwm;
    pwm = map(value,0,99,50,0);
    CCPR1L = (pwm >> 2);
    CCP1CON &= ~0x30;
    CCP1CON |= (pwm << 4) & 0x30;
}
/***************************************************************************************************************/
void hc595_write_special_char(uint8_t character, uint8_t pos)
{
    hc595_buff[(DIGIT_COUNT - pos) - 1] = character;
    //hc595_write_data();
}
/***************************************************************************************************************/
bool hc595_write_single_digit(uint8_t digit, uint8_t pos)
{
    uint8_t result = FALSE;
    if (pos < DIGIT_COUNT)
    {
        hc595_buff[(DIGIT_COUNT - pos) - 1] = font[digit];
        //hc595_write_data();
        result = TRUE;
    }
    return result;
}
/***************************************************************************************************************/
void hc595_write_number(uint16_t number, uint8_t from, bool zeros, uint8_t size)
{
    uint8_t i;
    uint8_t temp;
    const uint8_t max = DIGIT_COUNT - 1;
    
    while ((from + size) > DIGIT_COUNT);
    for (i = 0 ; i < size ; i++)
    {
        if (number_to_digit(number , &temp , (size - 1 - i)))
        {
            hc595_buff[max - from - i] = font[temp];
        }
        else
        {
            hc595_buff[max - from - i] = zeros ?  font[0] : SPECIAL_CHAR_BLANK;
        }
        
        
    }
    //hc595_write_data();
}
/***************************************************************************************************************/
void hc595_point(bool state, uint8_t position)
{
    if (state)
    {
        hc595_buff[DIGIT_COUNT - 1 - position] |= 0x04;
    }
    else
    {
        hc595_buff[DIGIT_COUNT - 1 - position] &= ~0x04;
    }
    //hc595_write_data();
}
/***************************************************************************************************************/
void hc595_show_screen()
{
    hc595_write_data();
    pulse_out();
}
/***************************************************************************************************************/
static uint8_t map(uint8_t x, uint8_t in_min, uint8_t in_max, uint8_t out_min, uint8_t out_max) 
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
/***************************************************************************************************************/
static void hc595_write_data()
{
    uint8_t i,j;
    
    for (i = 0 ; i < DIGIT_COUNT ; i++)
    {
        for(j = 0 ; j < 8 ; j++)
        {
            (hc595_buff[i] & (0x80 >> j)) ? SET_BIT(DATA_PORT, DATA_PIN) : CLR_BIT(DATA_PORT, DATA_PIN);
            SET_BIT(CLK_PORT, CLK_PIN);
            delay_us(1);
            CLR_BIT(CLK_PORT, CLK_PIN);
        }
    }
    //pulse_out();
}
/***************************************************************************************************************/
static void pulse_out()
{
    SET_BIT(LD_PORT, LD_PIN);
    delay_us(2);
    CLR_BIT(LD_PORT, LD_PIN);
}
/***************************************************************************************************************/
static bool number_to_digit(uint16_t number, uint8_t *digit, uint8_t digit_pos)
{
    
    uint16_t bolen;
    uint32_t modulo=10;
    uint8_t i;
    uint8_t count = count_digit(number);
    uint8_t result = FALSE;
    
    if (digit_pos > (count-1))
    {
        *digit = 0;
        result = FALSE;
    }
    else
    {
        for (i = 0 ; i < digit_pos; i++)
        {
            modulo *= 10;
        }
        bolen = modulo/10; 
        *digit = (uint8_t)((number % modulo) / bolen);
        result = TRUE;
    }
    return result;
}
/***************************************************************************************************************/
static uint8_t count_digit(uint32_t n) 
{ 
    uint8_t count = 0; 
    while (n != 0) { 
        n = n / 10; 
        ++count; 
    } 
    return count; 
} 
/***************************************************************************************************************/
uint8_t hc595_get_digit_count()
{
    return DIGIT_COUNT;
}