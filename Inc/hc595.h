#ifndef HC595_H
#define	HC595_H

#define SPECIAL_CHAR_BLANK  0x00
#define SPECIAL_CHAR_CELC   0x1B
#define SPECIAL_CHAR_r      0x81
#define SPECIAL_CHAR_C      0XCA
#define SPECIAL_CHAR_MINUS  0X01

void hc595_init();
void hc595_write_special_char(uint8_t character, uint8_t pos);
bool hc595_write_single_digit(uint8_t digit, uint8_t pos);
void hc595_write_number(uint16_t number, uint8_t from, uint8_t size);
void hc595_point(bool state, uint8_t position);
void hc595_set_intensity(uint8_t value);
void hc595_show_screen();

#endif	/* HC595_H */

