#ifndef DHT22_H
#define	DHT22_H

void dht22_init();
bool dht22_procsess();
int16_t dht22_read_temperature();
uint16_t dht22_read_hummidity();
#endif	/* DHT22_H */

