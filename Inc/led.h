#ifndef LED_H
#define LED_H
/***************************************************************************************************************/
void led_timer();
void led_init();
void led (uint16_t on_time, uint16_t off_time, uint8_t cycle, bool state);
bool led_check(void);
/***************************************************************************************************************/
#endif