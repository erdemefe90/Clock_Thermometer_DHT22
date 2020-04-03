#ifndef DS3231_H
#define DS3231_H

#ifdef REMOVE_CALENDER
typedef struct {
    uint8_t minute;
    uint8_t hour;
}time_t;

#else

typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t date;
    uint8_t month;
    uint8_t year;
}time_t;
#endif

void ds3231_init();
void ds3231_read_time(time_t *time);
void ds3231_write_time(time_t *time_to_write);

#endif