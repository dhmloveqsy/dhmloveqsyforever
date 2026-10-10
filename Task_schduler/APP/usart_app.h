#ifndef UART_APP_H
#define UART_APP_H
#include "mydefine.h"
int my_printf(UART_HandleTypeDef   *huart, const char *format, ...);
void uart_ringbuffer_task(void);
#endif


