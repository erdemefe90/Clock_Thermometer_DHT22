#ifndef DHT22_H
#define	DHT22_H

void dht22_init();
void dht22_start_signal(void);
bool dht22_check_response();
bool dht22_read_temp_hummidity(uint8_t *temp_sign, uint16_t* temp, uint16_t* hummidity);
#endif	/* DHT22_H */

