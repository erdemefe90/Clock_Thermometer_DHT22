#ifndef BUTTON_H
#define BUTTON_H

typedef enum{
    DOWN,
    UP,
    MODE,
    BUTTON_COUNT
}button_types_t;

void button_timer();
void button_init();
bool button_read(button_types_t button);
bool button_read_with_timeout(button_types_t button, uint16_t time);
void button_increase_number(uint8_t *number, uint8_t max);
void button_decrease_number(uint8_t *number, uint8_t min);

#endif