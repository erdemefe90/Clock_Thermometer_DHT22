#ifndef HC595_H
#define	HC595_H

void hc595_init();
bool hc595_write_single_digit(uint8_t digit, uint8_t pos);
void hc595_write_number(uint16_t number, uint8_t from, uint8_t size);
void hc595_point(bool state, uint8_t position);
void hc595_set_intensity(uint8_t value);

#endif	/* HC595_H */

