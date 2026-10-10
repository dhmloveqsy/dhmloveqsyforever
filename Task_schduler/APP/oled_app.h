#ifndef OLED_APP_H
#define OLED_APP_H
#include "mydefine.h"
int my_oled_printf(uint8_t x,uint8_t y,const char *format, ...);
void oled_task(void);
void OLED_SendBuff(uint8_t buff[4][128]);


#endif 




