#include <16F88.h>
#include "register_addresses.h"
#include <stdint.h>
#include <stdbool.h>
#include "defines.h"
#include "ds3231.h"

/***************************************************************************************************************/
#use delay(crystal=8000000)
#use i2c(Master,Fast,sda=PIN_B1,scl=PIN_B4,force_sw)
 /***************************************************************************************************************/
#define DS3231_I2C_ADDR_WRITE 0xD0
#define DS3231_I2C_ADDR_READ  0xD1

#define DS3231_REG_SECOND   0x00
#define DS3231_REG_MINUTE   0x01
#define DS3231_REG_HOUR     0x02
#define DS3231_REG_DAY      0x03
#define DS3231_REG_DATE     0x04
#define DS3231_REG_MONTH    0x05
#define DS3231_REG_YEAR     0x06
#define DS3231_REG_CONTROL  0x0E
#define DS3231_REG_STATUS   0x0F
/***************************************************************************************************************/
static uint8_t read_register(uint8_t register_address);
static void write_register(uint8_t register_address, uint8_t data);
static void write_bytes(uint8_t register_address, uint8_t *buffer , uint8_t length);
static void read_bytes(uint8_t register_address, uint8_t *buffer, uint8_t length);
static uint8_t bcd_to_decimal(uint8_t d);
static uint8_t decimal_to_bcd(uint8_t d);
/***************************************************************************************************************/
void ds3231_init()
{
    SET_BIT(TRISB, 1);
    SET_BIT(TRISB, 2);
    write_register(DS3231_REG_CONTROL, 0x00);
    write_register(DS3231_REG_STATUS, 0x08);
}
/***************************************************************************************************************/
void ds3231_read_time(time_t *time) 
{
    uint8_t *ptr;
#ifdef REMOVE_CALENDAR
    uint8_t buff[2];
    const uint8_t start_register = DS3231_REG_MINUTE;
    read_bytes(start_register, &buff, sizeof(buff));
    ptr = &buff;
    time->minute = bcd_to_decimal(*ptr++ & 0x7F);
    time->hour = bcd_to_decimal(*ptr++ & 0x3F);
#else
    uint8_t buff[7];
    const uint8_t start_register = DS3231_REG_SECOND;
    read_bytes(start_register, &buff, sizeof(buff));
    ptr = &buff;
    time->second = bcd_to_decimal(*ptr++ & 0x7F);
    time->minute = bcd_to_decimal(*ptr++ & 0x7F);
    time->hour = bcd_to_decimal(*ptr++ & 0x3F);
    time->day = bcd_to_decimal(*ptr++ & 0x03);
    time->date = bcd_to_decimal(*ptr++ & 0x3F);
    time->month = bcd_to_decimal(*ptr++ & 0x1F);
    time->year = bcd_to_decimal(*ptr & 0xFF);
#endif 
}

void ds3231_write_time(time_t *time_to_write)
{
    uint8_t *ptr;
#ifdef REMOVE_CALENDAR
    uint8_t buff[2];
    const uint8_t start_register = DS3231_REG_MINUTE;
    ptr = &buff;
    *ptr++ = decimal_to_bcd(time_to_write->minute) & 0x7F;
    *ptr++ = decimal_to_bcd(time_to_write->hour) & 0x3F;
#else
    uint8_t buff[7];
    const uint8_t start_register = DS3231_REG_SECOND;
    ptr = &buff;
    *ptr++ = decimal_to_bcd(time_to_write->second) & 0x7F;
    *ptr++ = decimal_to_bcd(time_to_write->minute) & 0x7F;
    *ptr++ = decimal_to_bcd(time_to_write->hour) & 0x3F;
    *ptr++ = decimal_to_bcd(time_to_write->day) & 0x03;
    *ptr++ = decimal_to_bcd(time_to_write->date) & 0x3F;
    *ptr++ = decimal_to_bcd(time_to_write->month) & 0x1F;
    *ptr++ = decimal_to_bcd(time_to_write->year) & 0xFF;
#endif
    write_bytes(start_register, &buff, sizeof(buff));
}
/***************************************************************************************************************/
static uint8_t read_register(uint8_t register_address) 
{
    unsigned char value = 0;
    i2c_start();
    i2c_write(DS3231_I2C_ADDR_WRITE);
    i2c_write(register_address);
    i2c_start();
    i2c_write(DS3231_I2C_ADDR_READ);
    value = i2c_read(0);
    i2c_stop();
    return value;
}
/***************************************************************************************************************/
static void write_register(uint8_t register_address, uint8_t data) 
{
    i2c_start();
    i2c_write(DS3231_I2C_ADDR_WRITE);
    i2c_write(register_address);
    i2c_write(data);
    i2c_stop();
}
/***************************************************************************************************************/
static void write_bytes(uint8_t register_address, uint8_t *buffer , uint8_t length) 
{
    i2c_start();
    i2c_write(DS3231_I2C_ADDR_WRITE);
    i2c_write(register_address);
    while (length--) 
    {
        i2c_write(*buffer++);
    }
    i2c_stop();
}
/***************************************************************************************************************/
static void read_bytes(uint8_t register_address, uint8_t *buffer, uint8_t length) 
{
    i2c_start();
    i2c_write(DS3231_I2C_ADDR_WRITE);
    i2c_write(register_address);
    i2c_start();
    i2c_write(DS3231_I2C_ADDR_READ);
    while (length--) 
    {
        *buffer++ = i2c_read(length ? 1 : 0);
    }
    i2c_stop();
}
/***************************************************************************************************************/
static uint8_t bcd_to_decimal(uint8_t d) 
{
    return ((d & 0x0F) + (((d & 0xF0) >> 4) * 10));
}
/***************************************************************************************************************/
static uint8_t decimal_to_bcd(uint8_t d) 
{
    return (((d / 10) << 4) & 0xF0) | ((d % 10) & 0x0F);
}